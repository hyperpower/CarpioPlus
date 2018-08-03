#ifndef _CONVECTION_HPP_
#define _CONVECTION_HPP_

#include "equation/equation_define.hpp"

namespace carpio{

template<St DIM, class DOMAIN>
class Convection_ : public Equation_<DIM, DOMAIN>{
public:
	typedef DOMAIN Domain;
	typedef Equation_<DIM, DOMAIN>     Equation;
	typedef typename Domain::SizeType  St;
	typedef typename Domain::ValueType Vt;
	typedef typename Domain::Grid      Grid;
	typedef typename Domain::Ghost     Ghost;
	typedef typename Domain::Order     Order;
	typedef typename Domain::Scalar    Scalar;
	typedef Event_<DIM>                Event;
	typedef TimeTerm_<DIM>             TimeTerm;

	typedef typename Domain::VectorCenter VectorCenter;
	typedef typename Domain::VectorFace   VectorFace;

	typedef std::shared_ptr<Event>  spEvent;
	typedef std::shared_ptr<Scalar> spScalar;
	typedef std::shared_ptr<Grid>   spGrid;
	typedef std::shared_ptr<Ghost>  spGhost;
	typedef std::shared_ptr<Order>  spOrder;
	typedef std::shared_ptr<VectorCenter>  spVectorCenter;
	typedef std::shared_ptr<VectorFace>    spVectorFace;
	typedef std::shared_ptr<TimeTerm>      spTimeTerm;
	typedef std::shared_ptr<BoundaryIndex> spBoundaryIndex;

	typedef std::function<Vt(Vt, Vt, Vt, Vt)> Function;

	typedef std::map<std::string, Any>                AFlags;
	typedef std::map<std::string, spScalar>           Scalars;
	typedef std::map<std::string, spBoundaryIndex>    BIs;
	typedef std::unordered_map<std::string, spEvent>  Events;
	typedef std::unordered_map<std::string, Function> Functions;
	typedef std::unordered_map<std::string, Vt>       Values;

protected:

public:
	Convection_(spGrid spg, spGhost spgh):
		Equation(spg, spgh){
	}

	Convection_(spGrid spg, spGhost spgh,
			    spScalar phi,
				spScalar u,
				spScalar v = nullptr,
				spScalar w = nullptr):
		Equation(spg, spgh){

	}



};


}

#endif
