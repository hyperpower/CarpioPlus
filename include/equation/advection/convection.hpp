#ifndef _CONVECTION_HPP_
#define _CONVECTION_HPP_

#include "type_define.hpp"
#include "equation/equation_define.hpp"
#include "equation/event/event.hpp"
#include <vector>

namespace carpio {

//This file use to solve convection equation
// 1 non-conservation from
//
//       d(phi)              d( phi)                2D --version
//    u ------------ +  v -------------  = 0
//          dx                 dy
//
//       d(phi)       d(phi)       d(phi)            3D --version
//    u -------- + v -------  + w -------  = 0
//        dx           dy          dz
//
// 2 conservation from
//
//      d( u phi)            d( v phi)              2D --version
//     --------------- +   --------------  = 0
//          dx                 dy
//
//      d(u phi)     d( v phi)       d(w phi)       3D --version
//     ----------- + ---------  +  ----------  = 0
//            dx        dy             dz
//
// 2 with time
//
//   d(phi)    d(u phi)    d(v phi)                 2D --version
//   ------ +  ------- +  -------  = 0
//     dt         dx         dy
//
//   d(phi)     d(u phi)  d(v phi)   d(w phi)       3D --version
//   ------ +  ------- +  -------  + -------  = 0
//     dt         dx         dy          dz




template<St DIM, class D>
class Convection_: public Equation_<DIM, D>{
public:
	typedef Convection_<DIM, D>        Self;
	typedef D                          Domain;
	typedef Equation_<DIM, D>          Equation;
	typedef typename Domain::SizeType  St;
	typedef typename Domain::ValueType Vt;
	typedef typename Domain::Grid      Grid;
	typedef typename Domain::Ghost     Ghost;
	typedef typename Domain::Order     Order;
	typedef typename Domain::Field     Field;
    typedef typename Domain::ExpField  ExpField;
	typedef Event_<DIM,Domain>         Event;
	typedef TimeTerm_<DIM>             TimeTerm;

	typedef typename Domain::VectorCenter VectorCenter;
	typedef typename Domain::VectorFace   VectorFace;

	typedef std::shared_ptr<Event>    spEvent;
	typedef std::shared_ptr<Field>    spField;
    typedef std::shared_ptr<ExpField> spExpField;
	typedef std::shared_ptr<Grid>     spGrid;
	typedef std::shared_ptr<Ghost>    spGhost;
	typedef std::shared_ptr<Order>    spOrder;
	typedef std::shared_ptr<VectorCenter>  spVectorCenter;
	typedef std::shared_ptr<VectorFace>    spVectorFace;
	typedef std::shared_ptr<TimeTerm>      spTimeTerm;
	typedef std::shared_ptr<BoundaryIndex> spBoundaryIndex;

	typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;

	typedef std::map<std::string, Any>                AFlags;
	typedef std::map<std::string, spField>            Fields;
	typedef std::map<std::string, spBoundaryIndex>    BIs;
	typedef std::unordered_map<std::string, spEvent>  Events;
	typedef std::unordered_map<std::string, FunXYZT_Value> Functions;
	typedef std::unordered_map<std::string, Vt>       Values;

	typedef typename Domain::UdotNabla     UdotNabla;
    typedef	std::shared_ptr<UdotNabla>   spUdotNabla;
	typedef typename Domain::UdotNabla_FOU UdotNabla_FOU;
	typedef typename Domain::UdotNabla_TVD UdotNabla_TVD;
	typedef typename Domain::Interpolate   Interpolate;

	typedef MatrixSCR_<Vt>    Mat;
    typedef ArrayListV_<Vt>   Arr;
    typedef Solver_<Vt>               Solver;
    typedef std::shared_ptr<Solver> spSolver;
    typedef Jacobi_<Vt>               Solver_Jacobi;
    typedef SOR_<Vt>                  Solver_SOR;
    typedef CG_<Vt>                   Solver_CG;

    typedef typename Domain::BuildMatrix              BuildMatrix;
protected:
	spVectorCenter _vc;
	spVectorFace   _vf;

	typedef void (Self::*FunOneStep)(St);
	FunOneStep     _fun_one_step;

	spUdotNabla    _spUdN_H;  // high order UdotNabla
public:
	Convection_(spGrid spg, spGhost spgh, spOrder spo):
		Equation(spg, spgh, spo){
		// new scalars
		_vc = spVectorCenter(new VectorCenter());
		_vf = spVectorFace(new VectorFace(spg, spgh, spo));
		_new_uvw();

		this->new_scalar("phi");
		// default one step function
		_fun_one_step = &Self::_one_step_fou_explicit;
	}

	int initialize() {
		FunXYZT_Value funs[] = {this->get_funtion("initial_u"),
							    this->get_funtion("initial_v"),
				                this->get_funtion("initial_w")};
		FOR_EACH_DIM{
			(*_vc)[d].assign(funs[d], 0.0);
		}

		this->_scalars["phi"]->assign(this->get_funtion("initial_phi"));

		if(this->_time == nullptr){
		    this->_aflags["solver"] = this->_init_solver();
		}
		std::cout << "  Convection: initialize \n";
		return -1;
	}

	int finalize() {
		std::cout << "  Convection: finalize \n";
		return -1;
	}

	int run_one_step(St step) {
		(this->*_fun_one_step)(step);
		return -1;
	}

    int solve() {
        _solve_fou();
        return -1;
    }

	void set_scheme(const std::string& name){
		if(name == "FOU"){
			_fun_one_step = &Self::_one_step_fou_explicit;
		}else if(_has_TVD_scheme(name)){
		    _spUdN_H = spUdotNabla(new UdotNabla_TVD(this->_bis["phi"], name));
		    _fun_one_step = &Self::_one_step_tvd;
		}else{
		    std::cerr << " !> No such scheme : " << name << std::endl;
		    SHOULD_NOT_REACH;
		}
	}

	void set_boundary_index_phi(spBoundaryIndex spbi){
		this->set_boundary_index("phi", spbi);
	}

	void set_boundary_index_velocity(Axes a, spBoundaryIndex spbi){
		switch (a) {
		case _X_: {
			this->set_boundary_index("u", spbi);
			break;
		}
		case _Y_: {
			this->set_boundary_index("v", spbi);
			break;
		}
		case _Z_: {
			this->set_boundary_index("w", spbi);
			break;
		}
		}
	}

	void set_initial_phi(FunXYZT_Value fun){
		this->set_function("initial_phi", fun);
	}

	void set_initial_velocity(Axes a, FunXYZT_Value fun){
		switch (a) {
		case _X_: {
			this->set_function("initial_u", fun);
			break;
		}
		case _Y_: {
			this->set_function("initial_v", fun);
			break;
		}
		case _Z_: {
			this->set_function("initial_w", fun);
			break;
		}
		}
	}

protected:
	bool _has_TVD_scheme(std::string name){
	    UdotNabla_TVD tvd;
	    return tvd.has_scheme(name);
	}

	void _new_uvw(){
		std::vector<std::string> vname = {"u", "v", "w"};
		for(St d = 0; d< DIM; ++d){
			this->new_scalar(vname[d]);
			_vc->set(ToAxes(d), this->_scalars[vname[d]]);
		}
	}

	void _one_step_fou_explicit(St step){
        UdotNabla_FOU FOU(this->_bis["phi"]);
        VectorFace&   vf = *(this->_vf);
        VectorCenter& vc = *(this->_vc);
        Field& phi = *(this->_scalars["phi"]);
        Vt      dt = this->_time->dt();
        Vt       t = this->_time->current_time();

        Interpolate::VectorCenterToFace(vc, vf,
                this->get_boundary_index("u"),
                this->get_boundary_index("v"),
                this->get_boundary_index("w"));
        if(step == 312){
            typename Domain::Index index(0, 0);
            std::cout << "Index = " << index << std::endl;
            std::cout << "u     = " << vc[_X_](index) << " v = " << vc[_Y_](index) << std::endl;
        }

        Vt dts = dt / DIM;
        for(St d = 0 ; d<DIM; d++){
            phi = phi - FOU.cal(vf, phi, d, t) * dts;
        }
	}

	void _one_step_tvd(St step) {
        UdotNabla_FOU   FOU(this->_bis["phi"]);
        this->_spUdN_H->set_boundary_index(this->_bis["phi"]);
        // default if quick scheme
        VectorFace& vf = *(this->_vf);
        VectorCenter& vc = *(this->_vc);
        Field& phi = *(this->_scalars["phi"]);
        Vt dt = this->_time->dt();

        Interpolate::VectorCenterToFace(vc, vf,
                this->get_boundary_index("u"),
                this->get_boundary_index("v"),
                this->get_boundary_index("w"));
        // half step FOU
        Vt dth    = dt * 0.5;
        auto phih = FOU.cal(vf, phi) * (-dth) + phi;
        phi       = this->_spUdN_H->cal(vf, phih) * (-dt) + phi;
    }


	int _solve_fou() {
        UdotNabla_FOU FOU(this->_bis["phi"]);
        VectorFace&   vf = *(this->_vf);
        VectorCenter& vc = *(this->_vc);
        Field&       phi = *(this->_scalars["phi"]);

        auto    spsolver = any_cast<spSolver>(this->_aflags["solver"]);
        ASSERT(spsolver != nullptr);
//        auto      spphif = any_cast<spExpField>(this->_aflags["field_exp"]);

        Interpolate::VectorCenterToFace(vc, vf,
                this->get_boundary_index("u"),
                this->get_boundary_index("v"),
                this->get_boundary_index("w"));

        auto      fouexp = FOU.cal_exp(vf, phi);

        Mat a;
        Arr b;
        BuildMatrix::Get(fouexp, a, b);
        Arr x(phi.order().size());
        BuildMatrix::CopyToArray(phi, x);
        this->_aflags["solver_rcode"] = spsolver->solve(a, x, b);
        BuildMatrix::CopyToField(x, phi);
    }
};


}

#endif
