#ifndef _PIECEWISE_LINEAR_2D_HPP_
#define _PIECEWISE_LINEAR_2D_HPP_

#include "voftool_define.hpp"
#include "geometry/geometry_define.hpp"
#include "geometry/objects/basic/point.hpp"
#include "geometry/objects/basic/line.hpp"
#include "algebra/algebra.hpp"
#include "algebra/array/array_list.hpp"

#include <memory>

namespace carpio {

template<class TYPE>
class VOFToolPL_:public VOFTool_<TYPE, 2>{
public:
	static const St Dim = 2;
	typedef TYPE Vt;
	typedef VOFTool_<TYPE, 2>                  Base;
	typedef Point_<TYPE, Dim>                  Point;
	typedef Point_<TYPE, Dim>&             ref_Point;
	typedef const Point_<TYPE, Dim>& const_ref_Point;

	typedef Line_<Vt>                           Line;
	typedef Line*                              pLine;
	typedef std::shared_ptr<Line>             spLine;
	typedef const pLine                  const_pLine;

	typedef Segment_<Vt, Dim>                Segment;
	typedef Segment*                        pSegment;
	typedef std::shared_ptr<Segment>       spSegment;
	typedef const pSegment            const_pSegment;

public:
	VOFToolPL_():Base(){}

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
	Vt cal_area(const Line& l) const{
		Vt m1     = l.a();
		Vt m2     = l.b();
		Vt alpha  = l.alpha();
		int cases = this->_which_case_4(m1, m2);
		switch(cases){
		case 1:
			return this->_cal_area(m1, m2, alpha);
		case 2:
			return this->_cal_area(-m1, m2, alpha - m1);
		case 3:
			return this->_cal_area(-m1, -m2, alpha - m1 - m2);
		case 4:
			return this->_cal_area(m1, -m2, alpha - m2);
		}
		SHOULD_NOT_REACH;
		return 0.0;
	}
	Vt cal_color(const Line& l) const{
		return this->cal_area(l);
	}
	/**
	 * \brief   Calculate the negative area divided by the line
	 *
	 * \param   Line l the line
	 * \param   Vt c1 box edge length on X
	 * \param   Vt c1 box edge length on Y
	 * \return  area
	 */
	Vt cal_area(const Line& l, const Vt& c1, const Vt& c2) const{
		Vt m1     = l.a();
		Vt m2     = l.b();
		Vt alpha  = l.alpha();
		int cases = this->_which_case_4(m1, m2);
		switch(cases){
		case 1:
			return this->_cal_area(m1, m2, alpha, c1, c2);
		case 2:
			return this->_cal_area(-m1, m2, alpha - m1 * c1, c1, c2);
		case 3:
			return this->_cal_area(-m1, -m2, alpha - m1 * c1 - m2 * c2, c1, c2);
		case 4:
			return this->_cal_area(m1, -m2, alpha - m2 * c2, c1, c2);
		}
		SHOULD_NOT_REACH;
		return 0.0;
	}

	Vt cal_color(const Line& l, const Vt& c1, const Vt& c2) const{
		return this->cal_area(l, c1, c2) / c1 / c2;
	}

	/*****************************************************
	 * Inverse Problem
	 *
	 * Known the color function and normal vector, calculate Line
	 *****************************************************/
	spLine construct_interface(Vt n1, Vt n2, Vt C, Vt c1, Vt c2) const{
		Vt area   = C * c1 * c2;
		Vt alpha  = this->_cal_alpha(std::abs(n1), std::abs(n2), area, c1, c2);
		int cases = this->_which_case_4(n1,n2);
		switch (cases) {
		case 1:
			return spLine(new Line(n1, n2, alpha));
		case 2:
			return spLine(new Line(n1, n2, alpha + n1 * c1));
		case 3:
			return spLine(new Line(n1, n2, alpha + n1 * c1 + n2 * c2));
		case 4:
			return spLine(new Line(n1, n2, alpha + n2 * c2));
		}
		return nullptr;
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
	spLine construct_interface(Vt n1, Vt n2, Vt C) const{
		Vt alpha  = this->_cal_alpha_in_unit_box(std::abs(n1), std::abs(n2), C);
		int cases = this->_which_case_4(n1,n2);
		switch (cases) {
		case 1:
			return spLine(new Line(n1, n2, alpha));
		case 2:
			return spLine(new Line(n1, n2, alpha + n1));
		case 3:
			return spLine(new Line(n1, n2, alpha + n1 + n2));
		case 4:
			return spLine(new Line(n1, n2, alpha + n2));
		}
		return nullptr;
	}
	/*****************************************************
	 * Middle Problem
	 *
	 * Known the Line, calculate Segment
	 *****************************************************/
	spSegment cal_segment(const Line& line, Vt c1, Vt c2) const{
		std::array<Axes, 4> ao = {_Y_, _Y_, _X_, _X_};
		std::array<Vt, 4>   av = {0.0 , c2, 0.0, c1};
		std::array<Vt, 4>   rv = {c1  , c1, c2,  c2};
		int f = 0;
		spSegment sps(new Segment());
		for(int i = 0; i < 4; i++){
			Vt cv = line.cal(ao[i], av[i]);
			if(IsInRange(0.0, cv, rv[i], _cc_)){
				sps->p(f)[_X_] = (ao[i] == _Y_) ? cv : av[i];
				sps->p(f)[_Y_] = (ao[i] == _X_) ? cv : av[i];
				f++;
			}
			if(f > 1){
				break;
			}
		}
		// orientation
		return sps;
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
	Vt _cal_alpha_in_unit_box(Vt a, Vt b, Vt C) const{
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
			alpha = (2.0 * C * n + m) / 2.0;
		} else { //(C>=c2 && C<=1)
			alpha = m + n - std::sqrt(2.0 * (1.0 - C) * m * n);
		}
		return alpha;
	}
	Vt _cal_alpha(Vt m1, Vt m2, Vt A, Vt c1, Vt c2) const{
		Vt n1, n2, d1, d2, alpha;
		if(m1 * c1 < m2 * c2){
			n2 = m2; d2 = c2;
			n1 = m1; d1 = c1;
		} else {
			n2 = m1; d2 = c1;
			n1 = m2; d1 = c2;
		}
		Vt Ac1 = 0.5 * d1 * d1 * n1 / n2;
		Vt Ac2 = d1 * d2 - Ac1;
		if (A >= 0 && A <= Ac1){
			return std::sqrt(2.0 * A * n1 * n2);
		} else if (A > Ac1 && A < Ac2){
			return (2.0 * A * n2 + n1 * d1 * d1) / 2.0 / d1;
		} else {
			return n1 * d1 + n2 * d2 - std::sqrt(2.0 * (d1 * d2 - A) * n1 * n2);
		}
		SHOULD_NOT_REACH;
		return 0;
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
	Vt _cal_area(
			const Vt& m1, const Vt& m2, const Vt alpha,
			const Vt& c1, const Vt& c2) const{
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
	Vt _cal_area(const Vt& m1, const Vt& m2, const Vt alpha) const{
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
	inline int _which_case_4(Vt a, Vt b) const{
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
