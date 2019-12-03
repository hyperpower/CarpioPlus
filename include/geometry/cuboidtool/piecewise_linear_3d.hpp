#ifndef _PIECEWISE_LINEAR_3D_HPP_
#define _PIECEWISE_LINEAR_3D_HPP_

#include "cuboidtool_define.hpp"
#include "geometry/geometry_define.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/box.hpp"
#include "algebra/algebra.hpp"
#include "algebra/array/array_list.hpp"
#include "piecewise_linear_2d.hpp"

#include <memory>

namespace carpio {


template<class TYPE>
class CuboidToolPL3_:public CuboidTool_<TYPE, 3>{
public:
	static const St Dim = 3;
	static const St NumFaces = 6;
	static const St NumVertexes = 8;

	typedef TYPE Vt;
	typedef CuboidTool_<TYPE, Dim>              Base;
	typedef CuboidToolPL2_<TYPE>               Tool2;
	typedef Point_<TYPE, Dim>                  Point;
	typedef Point_<TYPE, Dim>&             ref_Point;
	typedef const Point_<TYPE, Dim>& const_ref_Point;

	typedef Line_<Vt>                           Line;
	typedef Line*                              pLine;
	typedef std::shared_ptr<Line>             spLine;
	typedef const pLine                  const_pLine;

	typedef Box_<Vt, Dim>                      Box;
	typedef Box*                              pBox;
	typedef std::shared_ptr<Box>             spBox;
	typedef const pBox                  const_pBox;

	typedef Segment_<Vt, Dim>                Segment;
	typedef Segment*                        pSegment;
	typedef std::shared_ptr<Segment>       spSegment;
	typedef const pSegment            const_pSegment;

	typedef PointChain_<Vt, Dim>          PointChain;

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;

public:
    CuboidToolPL3_():Base(){}

    std::array<Vt, NumFaces> cut_cell_aperture_ratios(
    			const Vt& xo,
    			const Vt& yo,
    			const Vt& dx,
    			const Vt& dy,
    			const Vt& time,
    			const Vt& th,
    			FunXYZT_Value fun,
    			const Vt& tol,
    			const std::array<Vt, NumVertexes>& arrv) const{
    		// v0 - 2 => e0
    		// v1 - 3 => e1
    		// v0 - 1 => e2
    		// v2 - 3 => e3
    		std::array<Vt, NumFaces> arrres;
//    		auto se0  = _aperture_state(arrv[0], arrv[2], th); //2D
//    		arrres[0] = edge_aperture_ratio(xo,  yo, _Y_, dy,
//    				                            time, th, fun, tol, se0);
//    		auto se1  = _aperture_state(arrv[1], arrv[3], th);
//    		arrres[1] = edge_aperture_ratio(xo + dx, yo, _Y_, dy,
//    				                            time, th, fun, tol, se1);
//
//    		auto se2  = _aperture_state(arrv[0], arrv[1], th);
//    		arrres[2] = edge_aperture_ratio(xo, yo, _X_, dx,
//    				                            time, th, fun, tol, se2);
//
//    		auto se3  = _aperture_state(arrv[2], arrv[3], th);
//    		arrres[3] = edge_aperture_ratio(xo, yo + dy, _X_, dx,
//    				                            time, th, fun, tol, se3);
//    		return arrres;
    }

};
}
#endif
