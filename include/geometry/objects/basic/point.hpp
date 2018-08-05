#ifndef _POINT_HPP_
#define _POINT_HPP_

#include "geometry/geometry_define.hpp"
#include <array>
#include <sstream>
#include <iostream>

namespace carpio {

struct TagPoint: public TagGeometry {
	TagPoint() {
	}
};

//Point T ====================================
template<typename TYPE, St DIM>
class Point_: public std::array<TYPE, DIM> {
public:
	static const St Dim = DIM;
	typedef TagPoint Tag;
	typedef Point_<TYPE, DIM> Point;
	typedef St size_type;
	typedef TYPE Vt;
	typedef TYPE& reference;
	typedef TYPE* pointer;
	typedef const TYPE* const_pointer;
	typedef const TYPE& const_reference;

	typedef const Point& const_ref_Self;

	//constructor
	Point_() :
			std::array<TYPE, Dim>() {
		reconstruct(0, 0, 0);
	}

	Point_(const Vt& a, const Vt& b = 0, const Vt& c = 0) :
			std::array<Vt, Dim>() {
		this->at(0) = a;
		if (Dim >= 2) {
			this->at(1) = b;
		}
		if (Dim == 3) {
			this->at(2) = c;
		}
	}

	const_reference operator()(St idx) const {
		ASSERT(idx < Dim);
		return this->at(idx);
	}
	reference operator()(St idx) {
		ASSERT(idx < Dim);
		return this->at(idx);
	}

	Vt value(St idx) const {
		Vt res;
		if (idx < Dim) {
			return this->operator ()(idx);
		} else {
			return 0;
		}
	}

	const_reference x() const {
		return this->at(0);
	}

	reference x() {
		return this->at(0);
	}

	const_reference y() const {
		ASSERT(Dim >= 2);
		return this->at(1);
	}

	reference y() {
		ASSERT(Dim >= 2);
		return this->at(1);
	}

	const_reference z() const {
		ASSERT(Dim == 3);
		return this->at(2);
	}

	reference z() {
		ASSERT(Dim == 3);
		return this->at(2);
	}

	void reconstruct(const Vt& a, const Vt& b = 0, const Vt& c = 0) {
		this->at(0) = a;
		if (Dim >= 2) {
			this->at(1) = b;
		}
		if (Dim >= 3) {
			this->at(2) = c;
		}
	}

	bool operator<(const Point_<Vt, Dim>& a) const {
		for (St i = 0; i < Dim; i++) {
			if (this->at(i) < a[i]) {
				return true;
			}
		}
		return false;
	}

	bool operator==(const Point_<Vt, Dim> &a) const {
		if (Dim == 2) {
			return (this->at(0) == a[0] && this->at(1) == a[1]) ? true : false;
		} else {
			return (this->at(0) == a[0] && this->at(1) == a[1]
					&& this->at(2) == a[2]) ? true : false;
		}
	}
	bool operator!=(const Point_<Vt, Dim> &a) const {
		if (Dim == 2) {
			return !((this->at(0) == a[0] && this->at(1) == a[1]) ? true : false);
		} else {
			return !(
			(this->at(0) == a[0] && this->at(1) == a[1]
					&& this->at(2) == a[2]) ? true : false);
		}
	}
	void show() const {
		std::cout << std::scientific << "( " << this->at(0);
		if (Dim >= 2) {
			std::cout << " , " << this->at(1);
		}
		if (Dim >= 3) {
			std::cout << " , " << this->at(2);
		}
		std::cout << " )\n";
	}

	std::string to_string() const {
		std::stringstream sstr;
		sstr.precision(4);
		sstr << std::scientific << "( " << this->at(0);
		if (Dim >= 2) {
			sstr << " , " << this->at(1);
		} else if (Dim == 3) {
			sstr << " , " << this->at(2) << " )\n";
		} else {
			sstr << " )\n";
		}
		return sstr.str();
	}
	template<typename T>
	void transfer(const T&dx, const T& dy = 0, const T& dz = 0) {
		this->at(0) = this->at(0) + Vt(dx);
		this->at(1) = this->at(1) + Vt(dy);
		if (Dim == 3) {
			this->at(2) = this->at(2) + Vt(dz);
		}
	}
	template<typename T>
	void scale(const T&dx, const T&dy = 1, const T&dz = 1) {
		this->at(0) = this->at(0) * Vt(dx);
		this->at(1) = this->at(1) * Vt(dy);
		if (Dim == 3) {
			this->at(2) = this->at(2) * Vt(dz);
		}
	}
	/** Distance to other point */
	Vt dist(const Point& p) const {
		double dx = x() - p.x();
		double dy = y() - p.y();
		return sqrt(dx * dx + dy * dy);
	}

	void normalize() {
		double n = 0;
		for (St i = 0; i < Dim; i++) {
			n += double(this->at(i) * this->at(i));
		}
		n = sqrt(n);
		for (St i = 0; i < Dim; i++) {
			if (n != 0) {
				this->at(i) /= n;
			} else {
				this->at(i) = 0;
			}
		}
	}
	inline size_type size() const {
		return size_type(Dim);
	}
	inline pointer ptr() {
		return this->data();
	}
	inline const_pointer ptr() const {
		return this->data();
	}

	// operator

	Point& operator+=(const Point& p) {
		for (St i = 0; i < Dim; i++) {
			this->at(i) += p[i];
		}
		return *this;
	}
	Point& operator+=(const Vt& val) {
		for (St i = 0; i < Dim; i++) {
			this->at(i) += val;
		}
		return *this;
	}
	Point& operator-=(const Point& p) {
		for (St i = 0; i < Dim; i++) {
			this->at(i) -= p[i];
		}
		return *this;
	}
	Point& operator-=(const Vt& val) {
		for (St i = 0; i < Dim; i++) {
			this->at(i) -= val;
		}
		return *this;
	}
	Point& operator*=(const Point& p) {
		for (St i = 0; i < Dim; i++) {
			this->at(i) *= p[i];
		}
		return *this;
	}
	Point& operator*=(const Vt& val) {
		for (St i = 0; i < Dim; i++) {
			this->at(i) *= val;
		}
		return *this;
	}
	Point& operator/=(const Point& p) {
		for (St i = 0; i < Dim; i++) {
			this->at(i) /= p[i];
		}
		return *this;
	}
	Point& operator/=(const Vt& val) {
		for (St i = 0; i < Dim; i++) {
			this->at(i) /= val;
		}
		return *this;
	}

};
template<typename TYPE, St DIM>
inline Point_<TYPE, DIM> operator+(
		Point_<TYPE, DIM> lhs,
		const Point_<TYPE, DIM>& rhs) {
	lhs += rhs;
	return lhs;
}

template<typename TYPE, St DIM>
inline Point_<TYPE, DIM> operator-(
		Point_<TYPE, DIM> lhs,
		const Point_<TYPE, DIM>& rhs) {
	lhs -= rhs;
	return lhs;
}

template<typename TYPE, St DIM>
std::ostream& operator<<(std::ostream& stream, const Point_<TYPE, DIM>& point) {
	stream << "(";
	for (St d = 0; d < DIM; ++d) {
		stream << point[d];
		if (d != DIM - 1) {
			stream << ", ";
		}
	}
	stream << ")";
	return stream;
}

template<typename TYPE, St DIM>
double Distance2(const Point_<TYPE, DIM>& p1,
		         const Point_<TYPE, DIM>& p2) {
	if (DIM == 1) {
		double dis = p1[0] - p2[0];
		return std::abs(dis);
	}
	if (DIM == 2) {
		return double(
				(p1.x() - p2.x()) * (p1.x() - p2.x())
						+ (p1.y() - p2.y()) * (p1.y() - p2.y()));
	}
	if (DIM == 3) {
		return double(
				(p1.x() - p2.x()) * (p1.x() - p2.x())
						+ (p1.y() - p2.y()) * (p1.y() - p2.y())
						+ (p1.z() - p2.z()) * (p1.z() - p2.z()));
	}
	SHOULD_NOT_REACH;
	return 0.0;
}
template<typename TYPE, St DIM>
double Distance(const Point_<TYPE, DIM>& p1,
		         const Point_<TYPE, DIM>& p2) {
	return std::sqrt(Distance2(p1, p2));
}

/**
 * Get minimun loaction of
 *
 * for example:
 * a   = ( 1, 2, 3)
 *            ^
 * b   = ( 0, 4, 2)
 *         ^     ^
 * res = Min(a, b);
 * res = ( 0, 2, 2)
 */
template<typename TYPE, St DIM>
Point_<TYPE, DIM> Min(
		const Point_<TYPE, DIM>& a,
		const Point_<TYPE, DIM>& b) {
	Point_<TYPE, DIM> res;
	for (St i = 0; i < DIM; i++) {
		res[i] = std::min(a[i], b[i]);
	}
	return res;
}
/**
 * Get max location of
 */
template<typename TYPE, St DIM>
Point_<TYPE, DIM> Max(
		const Point_<TYPE, DIM>& a,
		const Point_<TYPE, DIM>& b) {
	Point_<TYPE, DIM> res;
	for (St i = 0; i < DIM; i++) {
		res[i] = std::max(a[i], b[i]);
	}
	return res;
}

//===============================================
// Cross product (v1 - v4) . ((v2-v4) x (v3-v4))
// for Point
// @param    p1 Point1
// @param    p2 Point1
// @return      the resualt of cross multiply
//              The scalar triple product
//              (also called the mixed product, box product)
//-----------------------------------------------
template<typename TYPE, St DIM>
double Cross(
		const Point_<TYPE, DIM>&v1,
		const Point_<TYPE, DIM>&v2,
		const Point_<TYPE, DIM>&v3,
		const Point_<TYPE, DIM>&v4 = Point_<TYPE, DIM>()) {
	if (DIM == 2) {
		/// d1    = v1 - v3
		/// d2    = v2 - v3
		/// cross = d1x * d2y - d1y * d2x
		return ((v1.x() - v3.x()) * (v2.y() - v3.y())
				- (v2.x() - v3.x()) * (v1.y() - v3.y()));
	}

	if (DIM == 3) {
		double a[3][3];
		for (short i = 0; i != 3; ++i) {
			a[0][i] = v1[i] - v4[i];
			a[1][i] = v2[i] - v4[i];
			a[2][i] = v3[i] - v4[i];
		}

		return a[0][0] * a[1][1] * a[2][2] + a[0][1] * a[1][2] * a[2][0]
				+ a[0][2] * a[1][0] * a[2][1] - a[0][2] * a[1][1] * a[2][0]
				- a[0][1] * a[1][0] * a[2][2] - a[0][0] * a[1][2] * a[2][1];
	}
	SHOULD_NOT_REACH;
	return 0.0;
}



template<typename TYPE>
Trinary OnWhichSide3(
		const Point_<TYPE, 2>& p0,
		const Point_<TYPE, 2>& p1,
		const Point_<TYPE, 2>& p2) {
	double tmp = Cross(p0, p1, p2);
	if (tmp > 0) {
		return _POSITIVE_;
	} else if (tmp < 0) {
		return _NEGATIVE_;
	} else {
		return _ZERO_;
	}
}

template<typename TYPE>
Trinary OnWhichSide3(
		const Point_<TYPE, 3>& p0,
		const Point_<TYPE, 3>& p1,
		const Point_<TYPE, 3>& p2,
		const Point_<TYPE, 3>& p3) {
	double tmp = Cross(p0, p1, p2, p3);
	if (tmp > 0) {
		return _POSITIVE_;
	} else if (tmp < 0) {
		return _NEGATIVE_;
	} else {
		return _ZERO_;
	}
}



} //end namespace

#endif /* POINT_H_ */
