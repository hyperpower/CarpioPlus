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
	}

	int initialize() {
		this->_scalars["phi"]->assign(this->get_funtion("initial_phi"));
		this->_aflags["solver"] = _init_solver();
		std::cout << "  Convection: initialize \n";
		return -1;
	}

	int finalize() {
		std::cout << "  Convection: finalize \n";
		return -1;
	}

	int run_one_step(St step) {
		std::cout << "    Convection: One Step "<< step <<" \n";
		return -1;
	}

	int solve() {
		_solve();
		std::cout << "  Equation: solve \n";
		return -1;
	}

	void set_boundary_index_phi(spBoundaryIndex spbi){
		this->set_boundary_index("phi", spbi);
	}

	void set_initial_phi(FunXYZT_Value fun){
		this->set_function("initial_phi", fun);
	}

	void set_solver( //
				const std::string&    name,                 //
				const int& max_iter = 1000,      //
				const Vt&  tol      = 1e-4,       //
				const Any& any      = 1.0   //
				) {
		// name should be
		ASSERT(name == "Jacobi" //
		    || name == "CG" //
		    || name == "SOR");
		this->_aflags["set_solver"]           = name;
		this->_aflags["set_solver_max_iter"]  = max_iter;
		this->_aflags["set_solver_tolerence"] = tol;
		if (name == "SOR") {
			this->_aflags["SOR_omega"] = any;
		}
	}

protected:
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
//		x.show();
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