#ifndef _EQUATION_DEFINE_HPP_
#define _EQUATION_DEFINE_HPP_


#include "type_define.hpp"

namespace carpio {

template<St DIM, class DOMAIN>
class Equation_{
public:
	typedef DOMAIN Domain;
	typedef typename Domain::SizeType  St;
	typedef typename Domain::ValueType Vt;
	typedef typename Domain::Grid      Grid;
	typedef typename Domain::Ghost     Ghost;
	typedef typename Domain::Order     Order;
	typedef typename Domain::Scalar    Scalar;

	typedef typename Domain::VectorCenter VectorCenter;
	typedef typename Domain::VectorFace   VectorFace;

protected:


};



}

#endif
