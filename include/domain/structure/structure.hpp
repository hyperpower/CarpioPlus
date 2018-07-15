#ifndef _STRUCTURE_HPP_
#define _STRUCTURE_HPP_

#include "index.hpp"
#include "grid/grid.hpp"
#include "grid/non_uniform.hpp"
#include "grid/uniform.hpp"

#include "ghost/ghost.hpp"
#include "ghost/regular.hpp"

#include "order/order.hpp"
#include "order/xyz.hpp"

#include "field/scalar.hpp"
#include "field/vector_center.hpp"
#include "field/vector_face.hpp"

namespace carpio {

template<St DIM>
class StructureDomain_{
public:
	typedef SGrid_<DIM>   Grid;
	typedef SGhost_<DIM>  Ghost;
	typedef SOrder_<DIM>  Order;
	typedef SScalar_<DIM> Scalar;

	typedef SVectorCenter_<DIM> VectorCenter;
	typedef SVectorFace_<DIM>   VectorFace;


};



}

#endif
