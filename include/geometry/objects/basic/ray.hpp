#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "geometry/geometry_define.hpp"
#include "point.hpp"
#include <array>
#include <type_traits>
#include <typeinfo>
#include "math.h"

namespace carpio {
struct TagRay: public TagGeometry {
	TagRay() {
	};
};

template<typename TYPE, St DIM>
class Ray_: public std::array< Point_<TYPE, DIM>, 2>{
public:
	static const St Dim = DIM;
	typedef TagRay Tag;
	typedef TYPE Vt;
	typedef Vt& ref_Vt;
	typedef const Vt& const_ref_Vt;
	typedef Ray_<TYPE, DIM> Self;
	typedef Ray_<TYPE, DIM>& ref_Self;
	typedef const Ray_<TYPE, DIM>& const_ref_Self;
	typedef Point_<TYPE, DIM> Point;
	typedef Point* pPoint;
	typedef Point& ref_Point;
	typedef const Point& const_ref_Point;

};

}

#endif
