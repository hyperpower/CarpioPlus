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

	/**
	 * \brief   known the normal vector (nx, ny)
	 *          and color function C
	 *
	 * \param   Vt nx x component of n
	 * \param   Vt nx x component of n
	 * \param   Vt C the color function
	 * \return  spLine
	 */
	static spLine ConstructInterFaceInUnitBox(Vt nx, Vt ny, Vt C) {

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
