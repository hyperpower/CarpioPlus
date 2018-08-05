#ifndef _STRUCTURE_HPP_
#define _STRUCTURE_HPP_

#include "index.hpp"
#include "grid/sgrid.hpp"
#include "grid/non_uniform.hpp"
#include "grid/uniform.hpp"

#include "ghost/ghost.hpp"
#include "ghost/regular.hpp"

#include "order/order.hpp"
#include "order/xyz.hpp"

#include "field/sscalar.hpp"
#include "field/svector_center.hpp"
#include "field/svector_face.hpp"

#include "operation/u_dot_nabla_phi.hpp"


namespace carpio {

template<St DIM>
class StructureDomain_{
public:
	typedef St SizeType;
	typedef Vt ValueType;
	typedef SGrid_<DIM>                 Grid;
	typedef Grid&                       ref_Grid;
	typedef const Grid&                 const_ref_Grid;
	typedef Grid*                       pGrid;
	typedef const Grid*                 pcGrid;
	typedef std::shared_ptr<Grid>       spGrid;
	typedef std::shared_ptr<const Grid> spcGrid;

	typedef SGhost_<DIM>                 Ghost;
	typedef Ghost&                       ref_Ghost;
	typedef const Ghost&                 const_ref_Ghost;
	typedef Ghost*                       pGhost;
	typedef const Ghost*                 pcGhost;
	typedef std::shared_ptr<Ghost>       spGhost;
	typedef std::shared_ptr<const Ghost> spcGhost;

	typedef SOrder_<DIM>                 Order;
	typedef Order&                       ref_Order;
	typedef const Order&                 const_ref_Order;
	typedef Order*                       pOrder;
	typedef const Order*                 pcOrder;
	typedef std::shared_ptr<Order>       spOrder;
	typedef std::shared_ptr<const Order> spcOrder;

	typedef SScalar_<DIM>                 Scalar;
	typedef Scalar&                       ref_Scalar;
	typedef const Scalar&                 const_ref_Scalar;
	typedef Scalar*                       pScalar;
	typedef const Scalar*                 pcScalar;
	typedef std::shared_ptr<Scalar>       spScalar;
	typedef std::shared_ptr<const Scalar> spcScalar;

	typedef SVectorCenter_<DIM> VectorCenter;
	typedef SVectorFace_<DIM>   VectorFace;

	typedef SUdotNabla_<DIM>        UdotNabla;
	typedef SUdotNabla_FOU<DIM>     UdotNabla_FOU;


};



}

#endif
