#ifndef _INTERPOLATE_HPP_
#define _INTERPOLATE_HPP_

#include <type_define.hpp>
#include <utility>
#include <map>

namespace carpio {

// 1D Interpolation
//   linear                           OK
//   quadratic                        ....
//   cubic spline                     ....
//   monotonic spline interpolation   ....
// 2D Interpolation
//   bilinear                         ....
//   bicubic                          ....
//   thin plate spline                ....


// linear interpolation
// https://en.wikipedia.org/wiki/Linear_interpolation
template<class NUMX, class NUMY>
NUMY LinearInterplate(const NUMX& x0, const NUMY& y0,
		              const NUMX& x1, const NUMY& y1,
					  const NUMX& x){
	ASSERT(x0 != x1);
	return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}



}

#endif
