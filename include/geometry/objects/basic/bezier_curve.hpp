#ifndef _BEZIERCURVE_HPP_
#define _BEZIERCURVE_HPP_

#include "geometry/geometry_define.hpp"
#include "algebra/algebra.hpp"
#include "point.hpp"
#include "segment.hpp"
#include <array>
#include <vector>

namespace carpio {

// The method is
// de Casteljau's Algorithm
// https://en.wikipedia.org/wiki/De_Casteljau's_algorithm
//

template<class TYPE, St DIM>
class BezierCurve_{
public:
	static const St Dim = DIM;
	typedef TYPE Cvt;

	typedef Point_<TYPE, 2> Point;
	typedef Point* pPoint;
	typedef Point& ref_Point;
	typedef const Point& const_ref_Point;

	typedef std::vector<Point> VectorPoints;

	typedef typename VectorPoints::iterator iterator;
	typedef typename VectorPoints::const_iterator const_iterator;
protected:
	VectorPoints _control_points;
	VectorPoints _curve_points;

public:
	template<class Container_Point>
	BezierCurve_(
			const Container_Point& ver,
			St precision = 10,
			std::string method = "decasteljau") :
	    _curve_points(precision + 2){
		std::copy(ver.begin(), ver.end(), std::back_inserter(_control_points));
		_calculate(method);
	}

	BezierCurve_(St numPoints, Cvt *points[Dim], int precision = 2) :
			_control_points(numPoints) {
	}

	const Point& control_point(St i) const{
		return _control_points[i];
	}

	const Point& operator[](St i) const{
		return _curve_points[i];
	}

	iterator begin(){
		return _curve_points.begin();
	}

	const_iterator begin() const{
		return _curve_points.begin();
	}

	iterator end() {
		return _curve_points.end();
	}

	const_iterator end() const {
		return _curve_points.end();
	}

	St size() const{
		return _curve_points.size();
	}

    ~BezierCurve_(){};

protected:
    void _calculate(const std::string& method){
    	if(method == "decasteljau"){
    		_calculate_decasteljau();
    	}else if(method == "bernstein"){
    		_calculate_bernstein();
    	}else{
    		SHOULD_NOT_REACH;
    	}
    }

    void _calculate_decasteljau(){
    	St precision = _curve_points.size() - 2;
    	_curve_points.front() = _control_points.front();
    	_curve_points.back()  = _control_points.back();
    	for(St i = 1; i <= precision; i++){
    		St seg = precision + 1;
    		Cvt t  = 1.0 / seg * i;
    		_curve_points[i] = _decasteljau(_control_points, t);
    	}
    }

    void _calculate_bernstein() {
    	St precision = _curve_points.size() - 2;
    	_curve_points.front() = _control_points.front();
    	_curve_points.back()  = _control_points.back();
    	St seg = precision + 1;
		for (int i = 1; i <= precision; i++) {
    	    Cvt t  = i / (precision + 2.0);
			St ncp = _control_points.size();
			Point pres(0.0, 0.0, 0.0);
			for (int m = 0; m < ncp; m++) {
				auto bm = _bernstein(m, ncp, t);
				for(St d = 0; d < Dim; d++){
					pres[d] += bm * _control_points[m][d];
				}
			}
			_curve_points[i] = pres;
		}
	}
    // Bernstein basis polynomial
    //            n            n-i    i
    //b   (t)=  (   ) * (1 - t)    * t
    // i,n        i
    //   n
    //  ( ) is a binomial coefficient.
    //   t
    Cvt _bernstein(St i, St n, Cvt t) {
    	// Binomial coefficient
		Cvt r = (Cvt) Factorial(n) / (Cvt) (Factorial(i) * Factorial(n - i));
		r *= std::pow(t, double(i));
		r *= std::pow(1.0 - t, double(n - i));
		return r;
	}

	Point _decasteljau(const VectorPoints& cp, Cvt t) {
		// cp : control points
		// Number of control points must be >= 3
		VectorPoints res = cp;
		St degree = cp.size();
		for (St r = 0; r < degree - 1; ++r) {
			for (St i = 0; i < degree - 1 - r; ++i) {
				res[i] = _inter_t(res[i], res[i + 1], t);
			}
		}
		return res[0];

	}

	Point _inter_t(const Point& p1, const Point& p2, Cvt t){
		Point res;
		for(St i = 0; i < Dim; ++i){
			res[i] = (1.0 - t) * p1[i] + t * p2[i];
		}
		return res;
	}

};


}

#endif
