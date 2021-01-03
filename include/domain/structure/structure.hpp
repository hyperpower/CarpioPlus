#ifndef _STRUCTURE_HPP_
#define _STRUCTURE_HPP_

#include "index.hpp"
#include "grid/sgrid.hpp"
#include "grid/non_uniform.hpp"
#include "grid/uniform.hpp"

#include "ghost/ghost.hpp"
#include "ghost/regular.hpp"
#include "ghost/regular_subdivision.hpp"
#include "ghost/mask.hpp"
#include "ghost/linear_cut.hpp"

#include "order/order.hpp"
#include "order/xyz.hpp"

#include "field/sfield.hpp"
#include "field/svector_center.hpp"
#include "field/svector_face.hpp"
#include "field/sexpression.hpp"
#include "field/scorner.hpp"

#include "operation/su_dot_nabla_phi.hpp"
#include "operation/soperation.hpp"
#include "operation/svof.hpp"
#include "operation/slaplacian.hpp"
#include "operation/sbuild_matrix.hpp"
#include "operation/svalue_cut.hpp"

#include "domain/structure/io/sio_file.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

#include "creation/sghost_by_function.hpp"

namespace carpio {

template<St DIM, class GRID, class GHOST, class ORDER>
class StructureDomain_{
public:
    typedef St SizeType;
    typedef Vt ValueType;
    typedef SIndex_<DIM>                Index;

    typedef GRID                        Grid;
    typedef Grid&                       ref_Grid;
    typedef const Grid&                 const_ref_Grid;
    typedef Grid*                       pGrid;
    typedef const Grid*                 pcGrid;
    typedef std::shared_ptr<Grid>       spGrid;
    typedef std::shared_ptr<const Grid> spcGrid;

    typedef GHOST                        Ghost;
    typedef Ghost&                       ref_Ghost;
    typedef const Ghost&                 const_ref_Ghost;
    typedef Ghost*                       pGhost;
    typedef const Ghost*                 pcGhost;
    typedef std::shared_ptr<Ghost>       spGhost;
    typedef std::shared_ptr<const Ghost> spcGhost;

    typedef ORDER                        Order;
    typedef Order&                       ref_Order;
    typedef const Order&                 const_ref_Order;
    typedef Order*                       pOrder;
    typedef const Order*                 pcOrder;
    typedef std::shared_ptr<Order>       spOrder;
    typedef std::shared_ptr<const Order> spcOrder;

    typedef SField_<DIM, GRID, GHOST, ORDER> Field;
    typedef Field&                       ref_Field;
    typedef const Field&                 const_ref_Field;
    typedef Field*                       pField;
    typedef const Field*                 pcField;
    typedef std::shared_ptr<Field>       spField;
    typedef std::shared_ptr<const Field> spcField;

    typedef SExpField_<DIM, GRID, GHOST, ORDER> ExpField;

    typedef SVectorCenter_<DIM, GRID, GHOST, ORDER> VectorCenter;
    typedef SVectorFace_<DIM, GRID, GHOST, ORDER>   VectorFace;

    typedef SUdotNabla_<DIM, Field>    UdotNabla;
    typedef SUdotNabla_FOU<DIM, Field> UdotNabla_FOU;
    typedef SUdotNabla_TVD<DIM, Field> UdotNabla_TVD;

    // creation

    // function
    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;


    // operation
    typedef SValue_<DIM, Field> Value;
    typedef SInterpolate_<DIM, Field>  Interpolate;
    typedef SLaplacian_<DIM, Field>    Laplacian;
    typedef SLaplacianCut_<DIM, Field> LaplacianCut;
    typedef SBuildMatrix_<DIM, Field>  BuildMatrix;
    // io file
    typedef SIOFile_<DIM, Grid, Ghost, Order> IOFile;

    // plot
    typedef SGnuplotActor_<DIM> GnuplotActor;
};



}

#endif
