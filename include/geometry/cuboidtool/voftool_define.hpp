#ifndef _VOFTOOL_DEFINE_HPP_
#define _VOFTOOL_DEFINE_HPP_

#include "type_define.hpp"
#include "geometry/objects/objects.hpp"
#include <type_traits>  //include this

namespace carpio {
template <typename TYPE, St DIM>
class VOFTool_ {
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


public:
	VOFTool_() {
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
	virtual ~VOFTool_() {
}
};


}

#endif
