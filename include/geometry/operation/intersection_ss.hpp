#ifndef _INTERSECTION_SS_HPP_
#define _INTERSECTION_SS_HPP_

#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/segment.hpp"
#include "algebra/array/array_list.hpp"

#include <memory>

namespace carpio {

enum IntersectionTypeSS {
	_SS_NO_       = 0,  //
	_SS_CONNECT_  = 1,  //
	_SS_TOUCH_    = 2,  //
	_SS_OVERLAP_  = 3,  //
	_SS_SAME_     = 4,  //
	_SS_INTERSECT_= 5
};

std::string ToString(IntersectionTypeSS ss){
	switch(ss){
		case _SS_NO_       :  return "NO";       break;
		case _SS_CONNECT_  :  return "CONNECT";  break;
		case _SS_TOUCH_    :  return "TOUCH";    break;
		case _SS_OVERLAP_  :  return "OVERLAP";  break;
		case _SS_SAME_     :  return "SAME";     break;
		case _SS_INTERSECT_:  return "INTERSECT";break;
	}
	SHOULD_NOT_REACH;
	return "ERROR IntersectionType";
}

IntersectionTypeSS ToIntersectionTypeSS(int a){
	ASSERT(a >=0 && a <=5);
	switch(a){
		case 0 : return _SS_NO_       ; break;
		case 1 : return _SS_CONNECT_  ; break;
		case 2 : return _SS_TOUCH_    ; break;
		case 3 : return _SS_OVERLAP_  ; break;
		case 4 : return _SS_SAME_     ; break;
		case 5 : return _SS_INTERSECT_; break;
	}
	SHOULD_NOT_REACH;
	return _SS_NO_;
}

static const short _SS_ERROR_   = -2;
static const short _SS_FURTHER_ = -1;

static const short _SSTYPE[7][7] = {
	// -1 further predicate
	// -2 error
	{_SS_OVERLAP_, _SS_TOUCH_,   _SS_TOUCH_,   _SS_OVERLAP_, _SS_OVERLAP_, _SS_OVERLAP_, _SS_OVERLAP_ },
	{_SS_TOUCH_,   _SS_NO_,      _SS_FURTHER_, _SS_CONNECT_, _SS_CONNECT_, _SS_NO_,      _SS_NO_      },
	{_SS_TOUCH_,   _SS_FURTHER_, _SS_NO_,      _SS_CONNECT_, _SS_CONNECT_, _SS_NO_,      _SS_NO_      },
	{_SS_OVERLAP_, _SS_CONNECT_, _SS_CONNECT_, _SS_ERROR_,   _SS_SAME_,    _SS_CONNECT_, _SS_OVERLAP_ },
	{_SS_OVERLAP_, _SS_CONNECT_, _SS_CONNECT_, _SS_SAME_,    _SS_ERROR_,   _SS_OVERLAP_, _SS_CONNECT_ },
	{_SS_OVERLAP_, _SS_NO_,      _SS_NO_,      _SS_CONNECT_, _SS_OVERLAP_, _SS_NO_,      _SS_OVERLAP_ },
	{_SS_OVERLAP_, _SS_NO_,      _SS_NO_,      _SS_OVERLAP_, _SS_CONNECT_, _SS_OVERLAP_, _SS_NO_      }
};

template<typename TYPE, St DIM>
class IntersectionPairSS_ {
public:
	static const St Dim = DIM;
	typedef Point_<TYPE, DIM> Point;
	typedef Box_<TYPE, DIM> Box;
	typedef Segment_<TYPE, DIM> Segment;
	typedef TYPE Vt;

	typedef const Point& const_ref_Point;
	typedef const Point* const_pPoint;

protected:
	ArrayListT<const_pPoint> _arrp;
	ArrayListV<int> _pointloc;

	std::shared_ptr<Point> _sppoint;

public:
	IntersectionPairSS_(
			const Point& p1,
			const Point& p2,
			const Point& p3,
			const Point& p4) : _arrp(4), _pointloc(4), _sppoint(nullptr){
		_arrp[0] = &p1;
		_arrp[1] = &p2;
		_arrp[2] = &p3;
		_arrp[3] = &p4;
		if (_is_box_in_on()) {
			_get_relation();
		} else {
			_pointloc.assign(-1);
		}
	}
	IntersectionPairSS_(
			const Segment& seg1,
			const Segment& seg2
			) : _arrp(4), _pointloc(4), _sppoint(nullptr){
		_arrp[0] = &(seg1.ps());
		_arrp[1] = &(seg1.pe());
		_arrp[2] = &(seg2.ps());
		_arrp[3] = &(seg2.pe());

		if (_is_box_in_on()) {
			_get_relation();
		} else {
			_pointloc.assign(-1);
		}
	}

	bool _is_box_in_on(){
		Box b1(*(_arrp[0]), *(_arrp[1]));
		Box b2(*(_arrp[2]), *(_arrp[3]));
		return IsInOn(b1, b2);
	}

	void _get_relation(){
		_pointloc[0] = OnWhichSide7(*(_arrp[2]), *(_arrp[3]), *(_arrp[0]));
		_pointloc[1] = OnWhichSide7(*(_arrp[2]), *(_arrp[3]), *(_arrp[1]));
		if(_get_ss_type1() == _SS_FURTHER_){
			_pointloc[2] = OnWhichSide7(*(_arrp[0]), *(_arrp[1]), *(_arrp[2]));
			_pointloc[3] = OnWhichSide7(*(_arrp[0]), *(_arrp[1]), *(_arrp[3]));
		}// else _relation[2] and [3] are -1
	}

	short _get_ss_type1(){ // seg1 to seg2
		short t = _SSTYPE[_pointloc[0]][_pointloc[1]];
		ASSERT(t != _SS_ERROR_);
		return t;
	}

	short _get_ss_type2() { // seg2 to seg1
		short t = _SSTYPE[_pointloc[2]][_pointloc[3]];
		ASSERT(t != _SS_ERROR_);
		return t;
	}

	IntersectionTypeSS cal_intersection_type(){
		if(_pointloc[1] > -1){
			short t1 = _get_ss_type1();
			if(t1 == _SS_FURTHER_) { // type is further
				short t2 = _get_ss_type2();
				if(t2 == _SS_FURTHER_){
					return _SS_INTERSECT_;
				}else{
					return ToIntersectionTypeSS(t2);
				}
			}else{
				return ToIntersectionTypeSS(t1);
			}
		}else{
			return _SS_NO_;
		}
	}

	// normal intersection
	Point cal_intersection_point(){
		// The intersection type must be _SS_INTERSECT_
		ASSERT(DIM == 2);
		Vt x0 = (*(_arrp[0])).x();
		Vt x1 = (*(_arrp[1])).x();
		Vt x2 = (*(_arrp[2])).x();
		Vt x3 = (*(_arrp[3])).x();


		Vt y0 = (*(_arrp[0])).y();
		Vt y1 = (*(_arrp[1])).y();
		Vt y2 = (*(_arrp[2])).y();
		Vt y3 = (*(_arrp[3])).y();

		double denom = (x0-x1) * (y2- y3) - (y0 - y1) * (x2- x3) + 1e-20;

		double x = ((x0 * y1 - y0 * x1) * (x2 - x3)
				- (x0 - x1) * (x2 * y3 - y2 * x3)) / denom ;
		double y = ((x0 * y1 - y0 * x1) * (y2 - y3)
				- (y0 - y1) * (x2 * y3 - y2 * x3)) / denom;

		return Point(x, y);
	}




};




}

#endif
