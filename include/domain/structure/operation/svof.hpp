#ifndef _S_VOF_HPP
#define _S_VOF_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "algebra/misc/interpolate.hpp"

#include "geometry/geometry.hpp"

#include <array>

namespace carpio{

template<St DIM, class FIELD>
class SVOF_{
public:
    typedef FIELD  Field;
    typedef typename Field::Grid  Grid;
    typedef typename Field::Ghost Ghost;
    typedef typename Field::Order Order;
    typedef SField_<1, Grid, Ghost, Order>  Field1;
    typedef SField_<2, Grid, Ghost, Order>  Field2;
    typedef SField_<3, Grid, Ghost, Order>  Field3;
    typedef Field*       pField;
    typedef SIndex_<DIM>  Index;
    typedef std::shared_ptr<Field> spField;

    typedef SVectorCenter_<DIM, Grid, Ghost, Order> VectorCenter;
    typedef SVectorFace_<DIM, Grid, Ghost, Order>   VectorFace;
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryIndex* pBI;
    typedef BoundaryCondition BC;

    typedef CuboidTool_<Vt, DIM> CuboidTool;
    typedef CuboidToolPL2_<Vt>   CuboidToolPL;
    typedef Point_<Vt, DIM>   Point;
    typedef Line_<Vt>          Line;

    typedef AInterpolate_<Vt, Vt> Interpolate;
public:
    SVOF_(){
    }

    static void SetColor(Field2& field, const Line& line){
    	CuboidToolPL tool;
        auto& grid = field.grid();
        for (auto& idx : field.order()) {
            Point pmin = grid.v(0, idx);
            Point pmax = grid.v(3, idx);
            field(idx) = tool.cal_color(line, pmin, pmax);
        }
    }




};

}
#endif
