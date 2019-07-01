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


	typedef typename Domain::Laplacian                Laplacian;
protected:
	typedef void (Self::*FunOneStep)(St);
	FunOneStep     _fun_one_step;
public:
	Laplace_(spGrid spg, spGhost spgh, spOrder spo):
		Equation(spg, spgh, spo){
		// new scalars
		this->new_scalar("phi");
		// default one step function
		_fun_one_step = &Self::_one_step_fou_explicit;
	}

	int initialize() {
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
		return -1;
	}

	int solve() {
		std::cout << "  Equation: solve \n";
		return -1;
	}

	void set_boundary_index_phi(spBoundaryIndex spbi){
		this->set_boundary_index("phi", spbi);
	}

	void set_initial_phi(FunXYZT_Value fun){
		this->set_function("initial_phi", fun);
	}

protected:

	int _solve(){

	}

};


}

#endif
