#ifndef GEOMETRY_TEST_VOFTOOL_HPP_
#define GEOMETRY_TEST_VOFTOOL_HPP_
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef GGnuplotActor_<double, 2> GA;

typedef VOFToolPL_<Vt> Tool;
typedef Line_<Vt> Line;
typedef Point_<Vt, 2> Point;

TEST(GEOVOF, case1){
	Vt a =  0.5;
	Vt b =  0.9;
	Vt alpha = 1.5;
	Vt c1 = 1.0;
	Vt c2 = 1.5;
	std::vector<Point> box{
			Point(0,0),
			Point(c1,0),
			Point(c1,c2),
			Point(0,c2),
			Point(0,0)
		};
	Line l1(a, b, alpha);
	std::cout<< "l1 : " << l1 << std::endl;
	std::cout<< "a =  " << l1.a() << std::endl;

	auto area = Tool::CalArea(l1, c1, c2);
	std::cout << "Area  = " << area << std::endl;
	Vt C = area / c1 / c2;
	std::cout << "Color = " << C << std::endl;

	auto spl  = Tool::ConstructInterface(a, b, C, c1, c2);
	if(spl != nullptr){
		std::cout << "spl : " << (*spl) << std::endl;
	}
	Gnuplot gnu;
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines((*spl), -1, 5, 2));
	gnu.plot();
}

TEST(GEOVOF, case2){
	Vt a = -0.5;
	Vt b =  0.9;
	Vt alpha = 1.0;
	Vt c1 = 1.0;
	Vt c2 = 2.0;
	std::vector<Point> box{
			Point(0,0),
			Point(c1,0),
			Point(c1,c2),
			Point(0,c2),
			Point(0,0)
		};
	Line l1(a, b, alpha);
	std::cout<< "l1 : " << l1 << std::endl;
	std::cout<< "a =  " << l1.a() << std::endl;

	Line l2(-a, b, alpha - a * c1);
	std::cout<< "l2 : " << l2 << std::endl;


	auto area = Tool::CalArea(l1, c1, c2);
	std::cout << "Area = " << area << std::endl;

	Gnuplot gnu;
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines(l2, -1, 5, 2));
//	gnu.plot();
}

TEST(GEOVOF, case3){
	Vt a = -0.5;
	Vt b = -0.9;
	Vt alpha = -1.0;
	Vt c1 = 1.5;
	Vt c2 = 1.0;
	std::vector<Point> box{
		Point(0,0),
		Point(c1,0),
		Point(c1,c2),
		Point(0,c2),
		Point(0,0)
	};
	Line l1(a, b, alpha);
	std::cout<< "l1 : " << l1 << std::endl;
	std::cout<< "a =  " << l1.a() << std::endl;

	Line l2(-a, -b, alpha -a*c1 - b*c2 ); // important
	std::cout<< "l2 : " << l2 << std::endl;


	Gnuplot gnu;
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines(l2, -1, 5, 2));
//	gnu.plot();
}

TEST(GEOVOF, case4){
	Vt a     =  0.5;
	Vt b     = -0.9;
	Vt alpha = -0.5;
	Vt c1    = 1.5;
	Vt c2    = 1.0;
	std::vector<Point> box{
		Point(0,0),
		Point(c1,0),
		Point(c1,c2),
		Point(0,c2),
		Point(0,0)
	};
	Line l1(a, b, alpha);
	std::cout<< "l1 : " << l1 << std::endl;
	std::cout<< "a =  " << l1.a() << std::endl;

	Line l2(a, -b, alpha - b*c2 ); // important
	std::cout<< "l2 : " << l2 << std::endl;

	Gnuplot gnu;
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines(l2, -1, 5, 2));
//	gnu.plot();
}

}

#endif
