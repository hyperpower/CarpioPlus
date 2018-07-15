#ifndef _S_U_DOT_NABLA_PHI_HPP
#define _S_U_DOT_NABLA_PHI_HPP


#include "structure_define.hpp"
#include "domain/structure/grid/grid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/scalar.hpp"
#include "domain/structure/field/vector_center.hpp"
#include "domain/structure/field/vector_face.hpp"

#include <array>

namespace carpio{


template<St DIM>
class UdotNabla_{
public:
	typedef SGrid_<DIM>   Grid;
	typedef SGhost_<DIM>  Ghost;
	typedef SOrder_<DIM>  Order;
	typedef SScalar_<DIM> Scalar;

	typedef SVectorCenter_<DIM> VectorCenter;
	typedef SVectorFace_<DIM>   VectorFace;


	UdotNabla_(){

	}

	virtual ~UdotNabla_(){

	}

	virtual Scalar operator()(const VectorCenter& U, const Scalar& phi){

	}

};


template<St DIM>
class UdotNabla_FOU: public UdotNabla_<DIM>{
public:
	typedef SGrid_<DIM>   Grid;
	typedef SGhost_<DIM>  Ghost;
	typedef SOrder_<DIM>  Order;
	typedef SScalar_<DIM> Scalar;

	typedef SVectorCenter_<DIM> VectorCenter;
	typedef SVectorFace_<DIM>   VectorFace;


	UdotNabla_FOU(){

	}

	virtual ~UdotNabla_FOU(){

	}

	virtual Scalar operator()(const VectorCenter& U, const Scalar& phi){

	}

};


}


#endif
