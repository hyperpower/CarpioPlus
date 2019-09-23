#ifndef _POISSON_HPP_
#define _POISSON_HPP_

#include "type_define.hpp"
#include "equation/equation_define.hpp"
#include "equation/event/event.hpp"
#include "laplace.hpp"
#include <vector>

namespace carpio {

//This file use to solve poisson equation
//
//   ▽•(beta(x,y)  ▽phi(x,y)  ) = 0   2D --version
//   ▽•(beta(x,y,z)▽phi(x,y,z)) = 0   3D --version
template<St DIM, class D>
class Poisson_: public Laplace_<DIM, D>{
public:
    typedef Poisson_<DIM, D>           Self;
    typedef D                          Domain;
    typedef Equation_<DIM, D>          Equation;
    typedef Laplace_<DIM, D>           Base;
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

    typedef MatrixSCR_<Vt>    Mat;
    typedef ArrayListV_<Vt>   Arr;
    typedef Solver_<Vt>       Solver;
    typedef std::shared_ptr<Solver> spSolver;
    typedef Jacobi_<Vt>       Solver_Jacobi;
    typedef SOR_<Vt>          Solver_SOR;
    typedef CG_<Vt>           Solver_CG;

    typedef typename Domain::Laplacian                Laplacian;
    typedef typename Domain::BuildMatrix              BuildMatrix;
protected:
public:
    Poisson_(spGrid spg, spGhost spgh, spOrder spo):
        Base(spg, spgh, spo){
        this->new_scalar("source");
    }

    virtual int initialize() {
        this->_scalars["phi"]->assign(this->get_funtion("initial_phi"));
        this->_scalars["source"]->assign(this->get_funtion("set_source"));
        this->_aflags["solver"] = this->_init_solver();
        std::cout << "  Poisson: initialize \n";
        return -1;
    }

    virtual ~Poisson_(){};

//  virtual int finalize() {}

//  virtual int run_one_step(St step) {}

//  virtual int solve() {
//    _solve();
//    return -1;
//  }

//  vitual void set_boundary_index_phi(spBoundaryIndex spbi){

//  void set_initial_phi(FunXYZT_Value fun){

//  void set_solver(
//              const std::string&    name,
//              const int& max_iter = 1000,
//              const Vt&  tol      = 1e-4,
//              const Any& any      = 1.0
//              ) {};

//    void set_time_scheme(
//            const std::string& name,
//            const Vt&          v = 0.5){}
    void set_source(FunXYZT_Value fun){
        this->set_function("set_source", fun);
    }
protected:
    virtual int _one_step_explicit(St step){
    	SHOULD_NOT_REACH;
        Laplacian Lap(this->_bis["phi"]);
        Field&    phi  = *(this->_scalars["phi"]);
        Field     v    = phi.volume_field();
        Vt        dt   = this->_time->dt();

        phi = (Lap(phi) * dt) / v + phi;

        return 1;
    }

    // Implicit
    virtual int _one_step_implicit(St step){
    	SHOULD_NOT_REACH;
        Laplacian Lap(this->_bis["phi"]);
        Field& phi     = *(this->_scalars["phi"]);
        auto  spsolver = any_cast<spSolver>(this->_aflags["solver"]);

        auto spphif    = any_cast<spExpField>(this->_aflags["field_exp"]);
        auto v         = any_cast<spField>(this->_aflags["field_volume"]);
        auto  Lapexp   = Lap.expression_field(phi);
        Vt    dt       = this->_time->dt();

        auto expf = (Lapexp * dt) / (*v) - (*spphif) + phi;

        Mat a;
        Arr b;
        BuildMatrix::Get(expf, a, b);
        // prepare x
        Arr x(phi.order().size());
        BuildMatrix::CopyToArray(phi, x);
        this->_aflags["solver_rcode"] = spsolver->solve(a, x, b);
        BuildMatrix::CopyToField(x, phi);

        return 1;
    }

    // Crank–Nicolson method
    virtual int _one_step_cn(St step){
    	SHOULD_NOT_REACH;
        Laplacian Lap(this->_bis["phi"]);
        Field& phi    = *(this->_scalars["phi"]);
        auto spsolver = any_cast<spSolver>(this->_aflags["solver"]);
        auto Lapv     = Lap(phi);
        auto Lapexp   = Lap.expression_field(phi);
        auto spphif   = any_cast<spExpField>(this->_aflags["field_exp"]);
        auto v        = any_cast<spField>(this->_aflags["field_volume"]);
        Vt dt         = this->_time->dt();

        auto expf = (Lapexp + Lapv) * (dt * 0.5) / (*v) - (*spphif) + phi;

        Mat a;
        Arr b;
        BuildMatrix::Get(expf, a, b);
        // prepare x
        Arr x(phi.order().size());
        BuildMatrix::CopyToArray(phi, x);
        this->_aflags["solver_rcode"] = spsolver->solve(a, x, b);
        BuildMatrix::CopyToField(x, phi);

        return 1;
    }

    virtual int _one_step_cng(St step){
    	SHOULD_NOT_REACH;
        Laplacian Lap(this->_bis["phi"]);
        Field& phi    = *(this->_scalars["phi"]);
        auto spsolver = any_cast<spSolver>(this->_aflags["solver"]);
        auto Lapv     = Lap(phi);
        auto Lapexp   = Lap.expression_field(phi);
        auto spphif   = any_cast<spExpField>(this->_aflags["field_exp"]);
        auto v        = any_cast<spField>(this->_aflags["field_volume"]);
        auto omega    = any_cast<Vt>(this->_aflags["cn_omega"]);
        Vt dt = this->_time->dt();

        auto expf = (Lapexp*(omega) + Lapv * (1.0 - omega)) * (dt) / (*v) - (*spphif) + phi;

        Mat a;
        Arr b;
        BuildMatrix::Get(expf, a, b);
        // prepare x
        Arr x(phi.order().size());
        BuildMatrix::CopyToArray(phi, x);
        this->_aflags["solver_rcode"] = spsolver->solve(a, x, b);
        BuildMatrix::CopyToField(x, phi);

        return 1;
    }

    virtual int _solve(){
    	std::cout << "Possion _solve()" << std::endl;
        Laplacian lap(this->_bis["phi"]);
        Field&    phi  = *(this->_scalars["phi"]);
        Field& source  = *(this->_scalars["source"]);
        auto  spsolver = any_cast<spSolver>(this->_aflags["solver"]);

        auto   expf    = lap.expression_field(phi);
        // build matrix
        Mat a;
        Arr b;
        BuildMatrix::Get(expf, a, b);
        // prepare x
        Arr x(phi.order().size());
        BuildMatrix::CopyToArray(phi, x);
        this->_aflags["solver_rcode"] = spsolver->solve(a, x, b);
        BuildMatrix::CopyToField(x, phi);
    }

//  spSolver _init_solver() {}

};


}

#endif
