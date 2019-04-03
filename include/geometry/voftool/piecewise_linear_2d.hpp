#ifndef _PIECEWISE_LINEAR_2D_HPP_
#define _PIECEWISE_LINEAR_2D_HPP_

#include "voftool_define.hpp"
#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/line.hpp"
#include "algebra/array/array_list.hpp"

#include <memory>

namespace carpio {

template<class TYPE>
class VOFToolPL_:public VOFTool_<TYPE>{
public:
	static const St Dim = 2;
	typedef TYPE Vt;
	typedef Point_<TYPE, Dim>                  Point;
	typedef Point_<TYPE, Dim>&             ref_Point;
	typedef const Point_<TYPE, Dim>& const_ref_Point;

	typedef Line_<Vt>                          Line;
	typedef Line*                             pLine;
	typedef std::shared_ptr<Line>            spLine;
	typedef const pLine                 const_pLine;

	/*****************************************************
	 * Forward Problem
	 *
	 * Known the Line, calculate Area(color)
	 *****************************************************/
	/**
	 * \brief   Calculate the negative area divided by the line
	 *          in a unit box (Edge length = 1)
	 *
	 * \param   Line l the line
	 * \return  area
	 */
	static Vt CalArea(const Line& l){
		Vt m1     = l.a();
		Vt m2     = l.b();
		Vt alpha  = l.alpha();
		int cases = _WhichCase4(m1, m2);
		switch(cases){
		case 1:
			return _CalArea(m1, m2, alpha);
		case 2:
			return _CalArea(-m1, m2, alpha - m1);
		case 3:
			return _CalArea(-m1, -m2, alpha - m1 - m2);
		case 4:
			return _CalArea(m1, -m2, alpha - m2);
		}
	}
	static Vt CalColorValue(const Line& l){
		return CalArea(l);
	}
	/**
	 * \brief   Calculate the negative area divided by the line
	 *
	 * \param   Line l the line
	 * \param   Vt c1 box edge length on X
	 * \param   Vt c1 box edge length on Y
	 * \return  area
	 */
	static Vt CalArea(const Line& l, const Vt& c1, const Vt& c2){
		Vt m1     = l.a();
		Vt m2     = l.b();
		Vt alpha  = l.alpha();
		int cases = _WhichCase4(m1, m2);
		switch(cases){
		case 1:
			return _CalArea(m1, m2, alpha, c1, c2);
		case 2:
			return _CalArea(-m1, m2, alpha - m1 * c1, c1, c2);
		case 3:
			return _CalArea(-m1, -m2, alpha - m1 * c1 - m2 * c2, c1, c2);
		case 4:
			return _CalArea(m1, -m2, alpha - m2 * c2, c1, c2);
		}
	}

	static Vt CalColorValue(const Line& l, const Vt& c1, const Vt& c2){
		return CalArea(l, c1, c2) / c1 / c2;
	}

	/*****************************************************
	 * Inverse Problem
	 *
	 * Known the color function and normal vector, calculate Line
	 *****************************************************/
	static spLine ConstructInterface(Vt n1, Vt n2, Vt C, Vt c1, Vt c2){
		Vt area   = C * c1 * c2;
		int cases = _WhichCase4(n1,n2);

	}

	/**
	 * \brief   known the normal vector (nx, ny)
	 *          and color function C
	 *
	 * \param   Vt nx x component of n
	 * \param   Vt nx x component of n
	 * \param   Vt C the color function
	 * \return  spLine
	 */
	static spLine ConstructInterface(Vt n1, Vt n2, Vt C) {

	}

//protected:
	/**
	 * \brief   known a,b in ax+by=alpha and C, calculate alpha \n
	 *          no matter what a and b are, they will be change to abs(a) and abs(b)
	 *          return alpha, ax+by=alpha, a>b>0;
	 *          The box must be a unit box
	 *
	 *          Piecewise Linear Interface Calculation
	 *          [Calcul d'Interface Affine par Morceaux]
	 *
	 * \param   Float a a in ax+by=alpha
	 * \param   Float b b in ax+by=alpha
	 * \param   Float C the color function
	 * \return  alpha
	 */
	static Vt _CalAlphaInUnitBox(Vt a, Vt b, Vt C) {
		Vt c1, c2, alpha;
		Vt absa = std::abs(a);
		Vt absb = std::abs(b);
		Vt m, n;
		n = (absa >= absb) ? (absa) : absb;
		m = (absa <= absb) ? (absa) : absb;
		c1 = m / 2 / n;
		c2 = 1 - c1;
		if (C >= 0 && C <= c1) {
			alpha = std::sqrt(2.0 * C * m * n);
		} else if (C > c1 && C < c2) {
			alpha = (2 * C * n + m) / 2.0;
		} else { //(C>=c2 && C<=1)
			alpha = m + n - std::sqrt(2.0 * (1.0 - C) * m * n);
		}
		return alpha;
	}
	static Vt _CalAlpha(Vt mx, Vt my, Vt A, Vt cx, Vt cy){
		Vt an1 = std::abs(mx);
		Vt an2 = std::abs(my);
		Vt m1, m2, c1, c2;
		if(mx * cx < my * cy){
			m1 = mx;
			c1 = cx;
			m2 = my;
			c2 = cy;
		}else{
			m1 = my;
			c1 = cy;
			m2 = mx;
			c2 = cx;
		}


	}
	/**
	 * \brief   m1, m2 must be positive
	 *
	 *          Volume-of-Fluid Interface Tracking with Smoothed
	 *          Surface Stress Methods for Three-Dimensional Flows
	 * \param   Float a a in ax+by=alpha
	 * \param   Float b b in ax+by=alpha
	 * \param   Float C the color function
	 * \return  alpha
	 */
	static Vt _CalArea(
			const Vt& m1, const Vt& m2, const Vt alpha,
			const Vt& c1, const Vt& c2) {
		// m1, m2 and alpha must be positive
		Vt amc1  = alpha - m1 * c1;
		//trivial case
		if (alpha <= 0.0){
			return 0.0;
		}
		if (amc1 - m2 * c2 >= 0.0){
			return c1 * c2;
		}
		Vt amc2  = alpha - m2 * c2;
		Vt term1 = Heaviside(amc1) * amc1 * amc1;
		Vt term2 = Heaviside(amc2) * amc2 * amc2;
		return 0.5 * (alpha * alpha - term1 - term2) / m1 / m2;
	}
	static Vt _CalArea(const Vt& m1, const Vt& m2, const Vt alpha) {
		// m1, m2 and alpha must be positive
		Vt amc1 = alpha - m1;
		//trivial case
		if (alpha <= 0.0) {
			return 0.0;
		}
		if (amc1 - m2  >= 0.0) {
			return 1.0;
		}
		Vt amc2 = alpha - m2;
		Vt term1 = Heaviside(amc1) * amc1 * amc1;
		Vt term2 = Heaviside(amc2) * amc2 * amc2;
		return 0.5 * (alpha * alpha - term1 - term2) / m1 / m2;
	}

	/**
	 * \brief   Calculate the direction of the line normal vector
	 *          <pre>
	 *                          ^y
	 *                          |
	 *                          |
	 *                    2     |     1
	 *                          |
	 *                          |
	 *          --+------+------+------+------+--> x
	 *                          |
	 *                          |
	 *                    3     |     4
	 *                          |
	 *                          |
	 *          </pre>
	 * \param   Float a a in (a,b)
	 * \param   Float b b in (a,b)
	 * \return  the case
	 */
	inline static int _WhichCase4(Vt a, Vt b) {
		const int cVOF_whichcase4[2][2] = { { 1, 4 }, { 2, 3 } };
		return cVOF_whichcase4[a >= 0 ? 0 : 1][b >= 0 ? 0 : 1];
	}

	/**
	 * \brief   Calculate the direction of the line normal vector
	 *          <pre>
	 *                          ^y
	 *                +         |         +
	 *                  +    3  |  2    +
	 *                    +     |     +
	 *                      +   |   +
	 *                  4     + | +      1
	 *          --+------+------+------+------+--> x
	 *                        + | +
	 *                  5   +   |   +    8
	 *                    +     |     +
	 *                  +     6 | 7     +
	 *                +         |         +
	 *          </pre>
	 * \param   Float a a in ax+by=C
	 * \param   Float b b in ax+by=C
	 * \return  the case
	 */
	inline static int _WhichCase8(Vt a, Vt b) {
		static const int cVOF_whichcase8[2][2][2] =
		{ { { 2, 1 }, { 7, 8 } },
		  { { 3, 4 }, { 6, 5 } } };
		return cVOF_whichcase8[a >= 0 ? 0 : 1][b >= 0 ? 0 : 1][
				std::abs(b) >= std::abs(a) ? 0 : 1];
	}



};

}

#endif
