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
//       d(phi)              d( phi)       2D --version
//    u ------------ +  v -------------  = 0
//          dx                 dy
//
//      d(phi)       d(phi)       d(phi)       3D --version
//    u -------- + v -------  + w -------  = 0
//        dx           dy          dz
//
// 2 conservation from
//
//      d( u phi)            d( v phi)       2D --version
//     --------------- +   --------------  = 0
//          dx                 dy
//
//      d(u phi)     d( v phi)       d(w phi)       3D --version
//     ----------- + ---------  +  ----------  = 0
//            dx        dy             dz
//
// 2 with time
//
//   d(phi)    d(u phi)    d(v phi)       2D --version
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
	typedef Event_<DIM,Domain>         Event;
	typedef TimeTerm_<DIM>             TimeTerm;

	typedef typename Domain::VectorCenter VectorCenter;
	typedef typename Domain::VectorFace   VectorFace;

	typedef std::shared_ptr<Event>  spEvent;
	typedef std::shared_ptr<Field>  spField;
	typedef std::shared_ptr<Grid>   spGrid;
	typedef std::shared_ptr<Ghost>  spGhost;
	typedef std::shared_ptr<Order>  spOrder;
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
	typedef typename Domain::UdotNabla_FOU UdotNabla_FOU;
	typedef typename Domain::Interpolate   Interpolate;

protected:
	spVectorCenter _vc;
	spVectorFace   _vf;

	typedef void (Self::*FunOneStep)(St);
	FunOneStep     _fun_one_step;
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
		std::cout << "  Convection: initialize \n";
		return -1;
	}

	int finalize() {
		std::cout << "  Convection: finalize \n";
		return -1;
	}

	int run_one_step(St step) {
		std::cout << "    Convection: One Step "<< step <<" \n";
		(this->*_fun_one_step)(step);
		return -1;
	}

	void set_space_scheme(const std::string& name){
		if(name == "fou"){
			_fun_one_step = Self::_one_step_fou_explicit;
		}else{
			_fun_one_step = Self::_one_step_fou_explicit;
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
	void _new_uvw(){
		std::vector<std::string> vname = {"u", "v", "w"};
		for(St d = 0; d< DIM; ++d){
			this->new_scalar(vname[d]);
			_vc->set(ToAxes(d), this->_scalars[vname[d]]);
		}
	}

	void _one_step_fou_explicit(St step){
		UdotNabla_FOU FOU(this->_bis["phi"]);
		VectorFace&   vf  = *(this->_vf);
		VectorCenter& vc  = *(this->_vc);
		Field&       phi  = *(this->_scalars["phi"]);
		Vt            dt  = this->_time->dt();

		Interpolate::VectorCenterToFace(vc, vf,
		    this->get_boundary_index("u"),
			this->get_boundary_index("v"),
			this->get_boundary_index("w"));
		phi = FOU(vf, phi) * dt + phi;
	}
};


}

#endif
