#ifndef _LAPLACE_HPP_
#define _LAPLACE_HPP_

#include "type_define.hpp"
#include "equation/equation_define.hpp"
#include "equation/event/event.hpp"
#include <vector>

namespace carpio {
//This file use to solve poisson equation
//
//   ▽•(beta(x,y)  ▽phi(x,y)  ) = 0   2D --version
//   ▽•(beta(x,y,z)▽phi(x,y,z)) = 0   3D --version
template<St DIM, class D>
class Laplace_: public Equation_<DIM, D>{
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
    Laplace_(spGrid spg, spGhost spgh, spOrder spo):
        Equation(spg, spgh, spo){
        // new scalars
        this->new_scalar("phi");
        // default one step function
        std::string name_time_scheme = "explicit";
        this->_aflags["set_time_scheme"] = name_time_scheme;
    }

    int initialize() {
        this->_scalars["phi"]->assign(this->get_funtion("initial_phi"));
        this->_aflags["solver"] = _init_solver();
        std::cout << "  Laplace: initialize \n";
        return -1;
    }

    int finalize() {
        std::cout << "  Laplace: finalize \n";
        return -1;
    }

    int run_one_step(St step) {
        auto name = any_cast<std::string>(this->_aflags["set_time_scheme"]);
        if(name == "explicit"){
            _one_step_explicit(step);
        }else if(name == "implicit"){
            _one_step_implicit(step);
        }else if(name == "CN"){
            _one_step_cn(step);
        }else if(name == "CNgeneral"){
            _one_step_cng(step);
        }else{
            std::cout <<" >! Unknown time scheme " << name << std::endl;
            SHOULD_NOT_REACH;
        }
        std::cout << "     Laplace: One Step "<< step <<" \n";
        return -1;
    }

    int solve() {
        _solve();
        std::cout << "  Laplace: solve \n";
        return -1;
    }

    void set_boundary_index_phi(spBoundaryIndex spbi){
        this->set_boundary_index("phi", spbi);
    }

    void set_initial_phi(FunXYZT_Value fun){
        this->set_function("initial_phi", fun);
    }

    void set_solver( 
                const std::string&    name,  
                const int& max_iter = 1000,  
                const Vt&  tol      = 1e-4,  
                const Any& any      = 1.0   
                ) {
        // name should be
        ASSERT(name == "Jacobi" 
            || name == "CG"
            || name == "SOR");
        this->_aflags["set_solver"]           = name;
        this->_aflags["set_solver_max_iter"]  = max_iter;
        this->_aflags["set_solver_tolerence"] = tol;
        if (name == "SOR") {
            this->_aflags["SOR_omega"] = any;
        }
    }

    void set_time_scheme(
            const std::string& name,
            const Vt&          v = 0.5){
        ASSERT(name == "explicit"
            || name == "implicit"
            || name == "CN"
            || name == "CNgeneral");
        this->_aflags["set_time_scheme"] = name;
        if(name != "explicit"){
            Field& phi = *(this->_scalars["phi"]);
            this->_aflags["field_volume"] = std::make_shared<Field>(phi.volume_field());
            this->_aflags["field_exp"]    = std::make_shared<ExpField>(ExpressionField(phi));
        }
        if(name == "CNgeneral"){
            this->_aflags["cn_omega"] = v;
        }
    }

protected:
    int _one_step_explicit(St step){
        Laplacian Lap(this->_bis["phi"]);
        Field&    phi  = *(this->_scalars["phi"]);
        Field     v    = phi.volume_field();
        Vt        dt   = this->_time->dt();

        phi = (Lap(phi) * dt) / v + phi;

        return 1;
    }

    // Implicit
    int _one_step_implicit(St step){
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
    int _one_step_cn(St step){
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

    int _one_step_cng(St step){
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


    int _solve(){
        Laplacian lap(this->_bis["phi"]);
        Field&    phi  = *(this->_scalars["phi"]);
        auto  spsolver = any_cast<spSolver>(this->_aflags["solver"]);
        auto   expf    = lap.expression_field(phi);
        Mat a;
        Arr b;
        BuildMatrix::Get(expf, a, b);
        // prepare x
        Arr x(phi.order().size());
        BuildMatrix::CopyToArray(phi, x);
        this->_aflags["solver_rcode"] = spsolver->solve(a, x, b);
        BuildMatrix::CopyToField(x, phi);
//        x.show();
    }

    spSolver _init_solver() {
        // initial solver
        spSolver spsolver;
        if (this->has_flag("set_solver")) {
            std::string sn = any_cast<std::string>(
                    this->_aflags["set_solver"]);
            Vt  tol      = any_cast<Vt>(this->_aflags["set_solver_tolerence"]);
            int max_iter = any_cast<int>(this->_aflags["set_solver_max_iter"]);
            if (sn == "Jacobi") {
                spsolver = spSolver(new Solver_Jacobi(max_iter, tol));
            } else if (sn == "CG") {
                spsolver = spSolver(new Solver_CG(max_iter, tol));
            } else if (sn == "SOR") {
                ASSERT(this->has_flag("SOR_omega"));
                Vt omega = any_cast<Vt>(this->_aflags["SOR_omega"]);
                spsolver = spSolver(new Solver_SOR(max_iter, tol, omega));
            }
        } else {
            // default solver
            spsolver = spSolver(new Solver_CG(5000, 1e-4));
        }
        return spsolver;
    }

};


}

#endif
