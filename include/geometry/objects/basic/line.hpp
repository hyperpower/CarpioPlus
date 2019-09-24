#ifndef _LINE_HPP_
#define _LINE_HPP_

#include "geometry/geometry_define.hpp"
#include "point.hpp"
#include <array>
#include <memory>

namespace carpio {


template<typename TYPE>
class Line_: public std::array<TYPE, 3> {
	//The Line function defined as ax+by=alpha
public:
	typedef St size_type;
	typedef TYPE Vt;
	typedef TYPE& reference;
	typedef const TYPE& const_reference;
	typedef Line_<TYPE> Line;

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
		auto arr = Line::Construct(ax, ay, bx, by);
		this->at(0) = arr[0];
		this->at(1) = arr[1];
		this->at(2) = arr[2];
	}
	Line_(const Point &p1, const Point &p2) {
		auto arr = Line::Construct(p1.x(), p1.y(), p2.x(), p2.y());
		this->at(0) = arr[0];
		this->at(1) = arr[1];
		this->at(2) = arr[2];
	}
	Line_(const Line& l){
		this->at(0) = l[0];
		this->at(1) = l[1];
		this->at(2) = l[2];
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

	static std::array<TYPE, 3> Construct(
			Vt ax, Vt ay, Vt bx, Vt by) {
		//assert(!isEqual(ax, bx) || !isEqual(ay,by));
		std::array<TYPE, 3> res;
		if (ax == bx) {
			res[0] = 1;
			res[1] = 0;
			res[2] = ax;
		} else if (ay == by) {
			res[0] = 0;
			res[1] = 1;
			res[2] = ay;
		} else {
			res[0] = 1.0 / (ax - bx);
			res[1] = -1.0 / (ay - by);
			res[2] = bx / (ax - bx) - by / (ay - by);
		}
		return res;
	}

	static std::shared_ptr<Point> Intersect(Vt a1, Vt b1, Vt c1,
			               Vt a2, Vt b2, Vt c2){
		double det = a1 * b2 - a2 * b1;
		if(std::abs(det) < 1e-14){
			return std::shared_ptr<Point>(nullptr);
		}
		double x   = (b2 * c1 - b1 * c2) / det;
		double y   = (a1 * c2 - a2 * c1) / det;
		return std::make_shared<Point>(Point(x, y));
	}

	static std::shared_ptr<Point> Intersect(const Line& l1, const Line& l2){
		double det = l1.a() * l2.b() - l2.a() * l1.b();
		if(det == 0){
			return std::shared_ptr<Point>(nullptr);
		}
		double x   = (l2.b() * l1.alpha() - l1.b() * l2.alpha()) / det;
		double y   = (l1.a() * l2.alpha() - l2.a() * l1.alpha()) / det;
		return std::make_shared<Point>(Point(x, y));
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
