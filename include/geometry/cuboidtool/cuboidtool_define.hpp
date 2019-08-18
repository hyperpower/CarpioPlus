#ifndef _VOFTOOL_DEFINE_HPP_
#define _VOFTOOL_DEFINE_HPP_

#include "type_define.hpp"
#include "geometry/objects/objects.hpp"
#include <tuple>
#include <type_traits>  //include this

namespace carpio {

// cuboid 1d is a line
//               *----------->*
// vertex number 0            1
// edge   number       0
// face   number       0

// cuboid 2d is a rectangle
// vertex order in z order
// 2------>3
// ^       ^
// |       |
// |       |
// 0------>1
const std::array<const std::array<Orientation, 2>, 4> VORDER2O2= {{
	{ _M_, _M_ }, // 0
    { _P_, _M_ }, // 1
    { _M_, _P_ }, // 2
    { _P_, _P_ }  // 3
}};
// edge order
//     3
// *------>*
// ^       ^
// |0      |1
// |       |
// *------>*
//     2
const std::array<const std::array<int, 2>, 2> FAO2ORDER2 = {{
	{0, 1}, // _X_
	{2, 3}  // _Y_
}};
// face order == edge order

// cuboid 3D
// vertex order
const std::array<const std::array<Orientation, 3>, 8> VORDER2O3= {{
	{ _M_, _M_, _M_ }, // 0
	{ _P_, _M_, _M_ }, // 1
	{ _M_, _P_, _M_ }, // 2
	{ _P_, _P_, _M_ }, // 3
	{ _M_, _M_, _P_ }, // 4
	{ _P_, _M_, _P_ }, // 5
	{ _M_, _P_, _P_ }, // 6
	{ _P_, _P_, _P_ }  // 7
}};
// face order
const std::array<const std::array<int, 2>, 3> FAO2ORDER3 = {{
	{0, 1},
	{2, 3},
	{4, 5}
}};
const std::array<Axes, 6> FORDER2A3 = {
	_X_, _X_, _Y_, _Y_, _Z_, _Z_
};
const std::array<Orientation, 6> FORDER2O3 = {
	_M_, _P_, _M_, _P_, _M_, _P_
};

// edge order



template <typename TYPE, St DIM>
class CuboidTool_ {
public:
	typedef typename std::conditional<DIM == 2,
			                          Line_<TYPE>,
									  Plane_<TYPE> >::type Interface;

	typedef Interface*                  pInterface;
	typedef std::shared_ptr<Interface> spInterface;
	typedef const pInterface      const_pInterface;
	typedef typename std::conditional<DIM == 2,
				                      Segment_<TYPE, 2>,
									  Segment_<TYPE, 3> >::type Piecewise; //wrong!!!
	typedef Piecewise*                    pPiecewise;
	typedef std::shared_ptr<Piecewise>   spPiecewise;
	typedef const pPiecewise        const_pPiecewise;

	typedef Point_<TYPE, DIM>		Point;

	typedef std::tuple<Axes, Orientation> TupleAO;
public:
	CuboidTool_() {
	}
	int vertex_order(Orientation ox, Orientation oy, Orientation oz){
		SHOULD_NOT_REACH;
	}
	Orientation vertex_orientation_x(int order){
		if(DIM == 1){
			ASSERT(order < 2);
			return (order == 0) ? _M_ : _P_;
		}
		if(DIM == 2){
			ASSERT(order < 4);
			return VORDER2O2[order][_X_];
		}
		if(DIM == 3){
			ASSERT(order < 8);
			return VORDER2O3[order][_X_];
		}
		return _M_;
	}
	Orientation vertex_orientation_y(const int& order) {
		if (DIM == 1) {
			return _M_;
		}
		if (DIM == 2) {
			ASSERT(order < 4);
			return VORDER2O2[order][_Y_];
		}
		if (DIM == 3) {
			ASSERT(order < 8);
			return VORDER2O3[order][_Y_];
		}
		return _M_;
	}
	Orientation vertex_orientation_z(const int& order) {
		if (DIM == 3) {
			ASSERT(order < 8);
			return VORDER2O3[order][_Z_];
		} else {
			return _M_;
		}
	}
	Orientation vertex_orientation(const int& order, Axes a){
		if(a == _X_){
			return vertex_orientation_x(order);
		}
		if(a == _Y_){
			return vertex_orientation_y(order);
		}
		if(a == _Z_) {
			return vertex_orientation_z(order);
		}
		return _M_;
	}
	virtual Vt cal_area(const Interface&, const Point&, const Point&) const {
		SHOULD_NOT_REACH;
		return 0.0;
	}
	virtual Vt cal_area(const Interface&, const Vt&, const Vt&) const {
		SHOULD_NOT_REACH;
		return 0.0;
	}
	virtual spInterface construct_interface(Vt n1, Vt n2, Vt C, Vt c1,
			Vt c2) const {
		SHOULD_NOT_REACH;
		return nullptr;
	}

	virtual spInterface construct_interface(Vt n1, Vt n2, Vt C) const {
		SHOULD_NOT_REACH;
		return nullptr;
	}

	virtual spPiecewise cal_segment(const Interface& line, Vt c1,
			Vt c2) const {
		SHOULD_NOT_REACH;
		return nullptr;
	}

	virtual Vt aperture_positive(const Interface&, const Point&, const Point&, int& axe, int& ori) const{
		SHOULD_NOT_REACH;
		return 0.0;
	}
	virtual Vt aperture_negative(const Interface&, const Point&, const Point&, int& axe, int& ori) const{
		SHOULD_NOT_REACH;
		return 0.0;
	}
	virtual Point centroid_positive(const Interface&, const Point&, const Point&) const{
		SHOULD_NOT_REACH;
		return Point();
	}
	virtual Point centroid_negative(const Interface&, const Point&, const Point&) const{
		SHOULD_NOT_REACH;
		return Point();
	}
	virtual ~CuboidTool_() {
}
};


}

#endif
