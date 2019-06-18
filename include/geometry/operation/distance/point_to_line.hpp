#ifndef GEOMETRY_POINT_TO_LINE_HPP_
#define GEOMETRY_POINT_TO_LINE_HPP_

#include "geometry/geometry_define.hpp"

namespace carpio {
// The distance d from a point (x0, y0) to the line ax + by + c = 0
//
template <typename NUM>
NUM Distance_PointToLine(const NUM& x0, const NUM& y0,
		                 const NUM& a,  const NUM& b,  const NUM& c){
	NUM s = a * a + b * b;
	s = (s==0)?SMALL:s;
	return std::abs(a * x0 + b * y0 + c) / std::sqrt(s);
}
template <typename NUM>
NUM Distance_PointToRay(const NUM& x0, const NUM& y0,
		                 const NUM& a,  const NUM& b,  const NUM& c){

}

template <typename NUM>
NUM Distance_PointToSegment(const NUM& x0, const NUM& y0,
		                    const NUM& a,  const NUM& b,  const NUM& c){

}


}

#endif
