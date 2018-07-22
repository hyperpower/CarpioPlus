#ifndef _S_U_DOT_NABLA_PHI_HPP
#define _S_U_DOT_NABLA_PHI_HPP


#include "structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/sscalar.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"

#include <array>

namespace carpio{


template<St DIM>
class SUdotNabla_{
public:
	typedef SGrid_<DIM>   Grid;
	typedef SGhost_<DIM>  Ghost;
	typedef SOrder_<DIM>  Order;
	typedef SScalar_<DIM> Scalar;

	typedef SVectorCenter_<DIM> VectorCenter;
	typedef SVectorFace_<DIM>   VectorFace;


	SUdotNabla_(){

	}

	virtual ~SUdotNabla_(){

	}

	virtual Scalar operator()(const VectorFace& U, const Scalar& phi){

	}

};


template<St DIM>
class SUdotNabla_FOU: public SUdotNabla_<DIM>{
public:
	typedef SGrid_<DIM>   Grid;
	typedef SGhost_<DIM>  Ghost;
	typedef SOrder_<DIM>  Order;
	typedef SScalar_<DIM> Scalar;

	typedef SVectorCenter_<DIM> VectorCenter;
	typedef SVectorFace_<DIM>   VectorFace;


	SUdotNabla_FOU(){

	}

	virtual ~SUdotNabla_FOU(){

	}

	virtual Scalar operator()(const VectorFace& U, const Scalar& phi){

	}

};


}


#endif
