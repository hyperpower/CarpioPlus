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

template<St DIM>
class SVOF_{
public:
    typedef SGrid_<DIM>   Grid;
    typedef SGhost_<DIM>  Ghost;
    typedef SOrder_<DIM>  Order;
    typedef SField_<DIM>  Field;
    typedef SField_<1>  Field1;
    typedef SField_<2>  Field2;
    typedef SField_<3>  Field3;
    typedef Field*       pField;
    typedef SIndex_<DIM>  Index;
    typedef std::shared_ptr<Field> spField;

    typedef SVectorCenter_<DIM> VectorCenter;
    typedef SVectorFace_<DIM>   VectorFace;
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryIndex* pBI;
    typedef BoundaryCondition BC;

    typedef CuboidTool_<Vt, DIM> CuboidTool;
    typedef CuboidToolPL_<Vt>    CuboidToolPL;
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
