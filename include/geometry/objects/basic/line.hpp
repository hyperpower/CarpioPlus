#ifndef _LINE_HPP_
#define _LINE_HPP_

#include "geometry/geometry_define.hpp"
#include "point.hpp"
#include <array>

namespace carpio {

template<typename TYPE>
class Line_: public std::array<TYPE, 3> {
	//The Line function defined as ax+by=alpha
public:
	typedef St size_type;
	typedef TYPE Vt;
	typedef TYPE& reference;
	typedef const TYPE& const_reference;

	typedef Point_<Vt, 2> Point;
public:
	Line_() :
			std::array<Vt, 3>() {
	}
	Line_(const Vt& a, const Vt& b, const Vt& c) {
		this->at(0) = a;
		this->at(1) = b;
		this->at(2) = c;
	}
	Line_(Vt ax, Vt ay, Vt bx, Vt by) {
		//assert(!isEqual(ax, bx) || !isEqual(ay,by));
		Point p1(ax, ay), p2(bx, by);
		if (p1.x() == p2.x()) {
			this->a() = 1;
			this->b() = 0;
			this->alpha() = p1.x();
		} else if (p1.y() == p2.y()) {
			this->a() = 0;
			this->b() = 1;
			this->alpha() = p1.y();
		} else {
			this->a() = 1.0 / (p1.x() - p2.x());
			this->b() = -1.0 / (p1.y() - p2.y());
			this->alpha() = p2.x() / (p1.x() - p2.x())
					- p2.y() / (p1.y() - p2.y());
		}
	}
	Line_(const Point &p1, const Point &p2) {
		if (p1.x() == p2.x()) {
			this->a() = 1;
			this->b() = 0;
			this->alpha() = p1.x();
		} else if (p1.y() == p2.y()) {
			this->a() = 0;
			this->b() = 1;
			this->alpha() = p1.y();
		} else {
			this->a() = 1.0 / (p1.x() - p2.x());
			this->b() = -1.0 / (p1.y() - p2.y());
			this->alpha() = p2.x() / (p1.x() - p2.x())
					- p2.y() / (p1.y() - p2.y());
		}
	}
	void reconstruct(Vt a, Vt b, Vt c) {
		if (a == 0.0 && b == 0.0) {
			a = SMALL;
			b = SMALL;
		}
		this->a() = a;
		this->b() = b;
		this->alpha() = c;
	}
	inline reference a() {
		return this->at(0);
	}
	inline reference b() {
		return this->at(1);
	}
	inline reference alpha() {
		return this->at(2);
	}
	inline const_reference a() const {
		return this->at(0);
	}
	inline const_reference b() const {
		return this->at(1);
	}
	inline const_reference alpha() const {
		return this->at(2);
	}
	Vt cal_x(Vt y) const {
		return (this->alpha() - this->b() * y)
				/ ((this->a() == 0.0) ? SMALL : this->a());
	}
	Vt cal_y(Vt x) const {
		return (this->alpha() - this->a() * x)
				/ ((this->b() == 0.0) ? SMALL : this->b());
	}
	// _X_=v ---> value of _Y_
	// _Y_=v ---> value of _X_
	Vt cal(Axes a, Vt v) const{
		if (a == _X_) {
			return cal_y(v);
		} else {
			return cal_x(v);
		}
	}
	Vt slope() const {
		return -this->a() / (this->b() + SMALL);
	}
	Vt intersept_x() const {
		return this->alpha() / (this->a() + SMALL);
	}
	Vt intersept_y() const {
		return this->alpha() / (this->b() + SMALL);
	}
	Vt intersept(Axes aix) const {
		ASSERT(aix!=_Z_);
		if(aix == _X_){
			return intersept_x();
		}else{
			return intersept_y();
		}
	}
	Vt norm_x() const {
		return this->a();
	}
	Vt norm_y() const {
		return this->b();
	}
	Vt shear_x() const {
		return this->b();
	}
	Vt shear_y() const {
		return -this->a();
	}
	bool empty() const {
		if (this->a() != 0.0 || this->b() != 0.0) {
			return true;
		} else {
			return false;
		}
	}
	// line can be written to Parametric Form
	// Ref: Geometric tools for computer graphics P174
	Point parametric_p() const{
		const Vt& a = this->at(0);
		const Vt& b = this->at(1);
		const Vt& c = this->at(2);
		Vt s = a * a + b * b;
		s = (s == 0) ? SMALL : s;
		return Point(-a * c / s, -b *c /s);
	}

	Point parametric_d() const{
		return Point(-(this->at(1)), this->at(0));
	}
};

template<typename TYPE>
std::ostream& operator<<(std::ostream& stream, const Line_<TYPE>& line) {
	stream << line.a() << " X + "
		   << line.b() << " Y = "
		   << line.alpha();
	return stream;
}

}

#endif
