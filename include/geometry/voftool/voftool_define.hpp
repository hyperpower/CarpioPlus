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
									  Plane_<TYPE> >::type AnalyticExp;

	typedef AnalyticExp*                  pAnalyticExp;
	typedef std::shared_ptr<AnalyticExp> spAnalyticExp;
	typedef const pAnalyticExp      const_pAnalyticExp;
	typedef typename std::conditional<DIM == 2,
				                      Segment_<TYPE, 2>,
									  Segment_<TYPE, 3> >::type Piecewise; //wrong!!!
	typedef Piecewise*                    pPiecewise;
	typedef std::shared_ptr<Piecewise>  spPiecewise;
	typedef const pAnalyticExp      const_pPiecewise;

public:
	VOFTool_() {
	}
	virtual Vt cal_area(const AnalyticExp&, const Vt&, const Vt&) const {
		SHOULD_NOT_REACH;
		return 0.0;
	}
	virtual spAnalyticExp construct_interface(Vt n1, Vt n2, Vt C, Vt c1,
			Vt c2) const {
		SHOULD_NOT_REACH;
		return nullptr;
	}

	virtual spAnalyticExp construct_interface(Vt n1, Vt n2, Vt C) const {
		SHOULD_NOT_REACH;
		return nullptr;
	}

	virtual spPiecewise cal_segment(const AnalyticExp& line, Vt c1,
			Vt c2) const {
		SHOULD_NOT_REACH;
		return nullptr;
	}
	virtual ~VOFTool_() {
}
};


}

#endif
