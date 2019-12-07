#ifndef _BOOLEAN_LINE_BOX_HPP_
#define _BOOLEAN_LINE_BOX_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "geometry/objects/analytic/line.hpp"
#include "geometry/objects/basic/point_chain.hpp"

#include "algebra/algebra.hpp"

#include <memory>
#include <array>

namespace carpio {

// Line : ax + by = alpha
// Box  : Pmin Pmax


// Point min is (0, 0)
template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineBox(const NUM& xmax,const NUM& ymax,                         // Point max
				 const NUM& a,   const NUM& b, const NUM& alpha){         // Line
	std::array<Axes, 4> ao = {_Y_  , _Y_,  _X_,  _X_};
	std::array<Vt, 4>   av = {0.0  , ymax, 0.0,  xmax};
	std::array<Vt, 4>   rv = {xmax , xmax, ymax, ymax};
	std::list<std::shared_ptr<Point_<NUM, 2> > > res;
	for(int i = 0; i < 4; i++){
		Vt cv = Calculate(a, b, alpha, ao[i], av[i]);
		if(IsInRange(0.0, cv, rv[i], _cc_)){
			std::shared_ptr<Point_<NUM, 2> > spp(new Point_<NUM, 2>);
			spp->x() = (ao[i] == _Y_) ? cv : av[i];
			spp->y() = (ao[i] == _X_) ? cv : av[i];
			res.push_back(spp);
		}
//		if(res.size() > 1){
//			break;
//		}
	}
	// orientation
	return res;
}


template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineBox(const NUM& xmin,const NUM& ymin,  // Point min
		         const NUM& xmax,const NUM& ymax,  // Point max
				 const NUM& a,   const NUM& b, const NUM& alpha){         // Line
	// transfer to (0,0)
	auto listspp = IntersectLineBox(xmax - xmin, ymax - ymin,
			                        a, b, alpha - (a * xmin) - (b * ymin));
	for(auto& sp : listspp){
		sp->transfer(xmin, ymin);
	}
	return listspp;
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLinePositiveBox(const NUM& xmin,const NUM& ymin,  // Point min
		                 const NUM& xmax,const NUM& ymax,  // Point max
				         const NUM& a,   const NUM& b, const NUM& alpha){         // Line

}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineNagativeBox(const NUM& xmin,const NUM& ymin,  // Point min
		                 const NUM& xmax,const NUM& ymax,  // Point max
				         const NUM& a,   const NUM& b, const NUM& alpha){         // Line

}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineBox(const Point_<NUM, 2>& pmax,          // Point max
				 const Line_<NUM>&     line){         // Line
	return IntersectLineBox(pmax.x(), pmax.y(), line.a(), line.b(), line.alpha());
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineBox(const Point_<NUM, 2>& pmin,          // Point min
		         const Point_<NUM, 2>& pmax,          // Point max
				 const Line_<NUM>&     line){         // Line
	return IntersectLineBox(pmin.x(), pmin.y(),
			                pmax.x(), pmax.y(),
							line.a(), line.b(), line.alpha());
}

template<class NUM>
std::list<std::shared_ptr<Point_<NUM, 2> > >
IntersectLineBox(const Box_<NUM,2>&  box,           // Box
				 const Line_<NUM>&   line){         // Line
	return IntersectLineBox(box.min(), box.max(), line);
}


}

#endif
