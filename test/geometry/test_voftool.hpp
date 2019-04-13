#ifndef GEOMETRY_TEST_VOFTOOL_HPP_
#define GEOMETRY_TEST_VOFTOOL_HPP_
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef GGnuplotActor_<double, 2> GA;

typedef VOFToolPL_<Vt> Tool;
typedef Line_<Vt> Line;
typedef Point_<Vt, 2> Point;

TEST(GEOVOF, DISABLED_case1){
	Vt m1 =  0.8;
	Vt m2 =  1.0;
	Vt alpha = 2.1;
	Vt c1 = 1.5;
	Vt c2 = 1.0;
	std::vector<Point> box{
			Point(0,0),
			Point(c1,0),
			Point(c1,c2),
			Point(0,c2),
			Point(0,0)
		};
	Line l1(m1, m2, alpha);
	std::cout<< "l1    : " << l1 << std::endl;
	std::cout<< "alpha =  " << l1.alpha() << std::endl;
	std::cout<< "m1    =  " << l1.a() << std::endl;
	std::cout<< "m2    =  " << l1.b() << std::endl;
	std::cout<< "c1    =  " << c1 << std::endl;
	std::cout<< "c2    =  " << c2 << std::endl;
	std::cout<< "c2 / c1 =  " << c2 / c1 << std::endl;
	std::cout<< "m1 / m2 =  " << m1 / m2 << std::endl;

	auto area = Tool::CalArea(l1, c1, c2);
	std::cout << "Area  = " << area << std::endl;
	Vt C = area / c1 / c2;
	std::cout << "Color = " << C << std::endl;

	auto spl  = Tool::ConstructInterface(m1, m2, C, c1, c2);
	if(spl != nullptr){
		std::cout << "spl : " << (*spl) << std::endl;
	}

	auto sps = Tool::CalSegment((*spl), c1, c2);
	Gnuplot gnu;
//	gnu.set_terminal_png("cline.png");
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines((*spl), -1, 5, 2));
	gnu.add(GA::LinesPoints((*sps), 3));
	gnu.plot();
}

TEST(GEOVOF, case2){
	Vt m1 = -0.5;
	Vt m2 =  0.9;
	Vt alpha = 1.0;
	Vt c1 = 1.0;
	Vt c2 = 1.5;
	std::vector<Point> box{
			Point(0,0),
			Point(c1,0),
			Point(c1,c2),
			Point(0,c2),
			Point(0,0)
		};
	Line l1(m1, m2, alpha);
	std::cout<< "l1 : " << l1 << std::endl;
	std::cout<< "a =  " << l1.a() << std::endl;

	Line l2(-m1, m2, alpha - m1 * c1);
	std::cout<< "l2 : " << l2 << std::endl;

	auto area = Tool::CalArea(l1, c1, c2);
	std::cout << "Area = " << area << std::endl;
	Vt C = area / c1 / c2;
	std::cout << "Color = " << C << std::endl;
	auto spl  = Tool::ConstructInterface(m1, m2, C, c1, c2);
	if(spl != nullptr){
		std::cout << "spl : " << (*spl) << std::endl;
	}
	auto sps = Tool::CalSegment((*spl), c1, c2);
	Gnuplot gnu;
//	gnu.set_terminal_png("cline.png");
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines((*spl), -1, 5, 1));
	ASSERT_LE(l1.alpha() - spl->alpha(), 1e-8);
	gnu.add(GA::Arrows((*sps), 3));
//	gnu.add(GA::Lines(l2, -1, 5, 2));
	gnu.plot();
}

TEST(GEOVOF, case3){
	Vt m1 = -0.5;
	Vt m2 = -0.9;
	Vt alpha = -1.4;
	Vt c1 = 1.5;
	Vt c2 = 1.0;
	std::vector<Point> box{
		Point(0,0),
		Point(c1,0),
		Point(c1,c2),
		Point(0,c2),
		Point(0,0)
	};
	Line l1(m1, m2, alpha);
	std::cout<< "l1 : " << l1 << std::endl;
	std::cout<< "a =  " << l1.a() << std::endl;

	Line l2(-m2, -m1, alpha -m1*c1 - m2*c2 ); // important
	std::cout<< "l2 : " << l2 << std::endl;

	auto area = Tool::CalArea(l1, c1, c2);
	std::cout << "Area = " << area << std::endl;
	Vt C = area / c1 / c2;
	std::cout << "Color = " << C << std::endl;
	auto spl  = Tool::ConstructInterface(m1, m2, C, c1, c2);
	if(spl != nullptr){
		std::cout << "spl : " << (*spl) << std::endl;
	}

	Gnuplot gnu;
	gnu.set_terminal_png("cline.png");
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines((*spl), -1, 5, 2));
	ASSERT_LE(l1.alpha() - spl->alpha(), 1e-8);
//	gnu.plot();
}

TEST(GEOVOF, DISABLED_case4){
	Vt m1     =  0.5;
	Vt m2     = -0.9;
	Vt alpha = -0.2;
	Vt c1    = 1.5;
	Vt c2    = 1.0;
	std::vector<Point> box{
		Point(0,0),
		Point(c1,0),
		Point(c1,c2),
		Point(0,c2),
		Point(0,0)
	};
	Line l1(m1, m2, alpha);
	std::cout<< "l1 : " << l1 << std::endl;
	std::cout<< "a =  " << l1.a() << std::endl;

	Line l2(m1, -m2, alpha - m2*c2 ); // important
	std::cout<< "l2 : " << l2 << std::endl;


	auto area = Tool::CalArea(l1, c1, c2);
	std::cout << "Area = " << area << std::endl;
	Vt C = area / c1 / c2;
	std::cout << "Color = " << C << std::endl;
	auto spl  = Tool::ConstructInterface(m1, m2, C, c1, c2);
	if(spl != nullptr){
		std::cout << "spl : " << (*spl) << std::endl;
	}

	Gnuplot gnu;
	gnu.set_terminal_png("cline.png");
	gnu.set_equal_aspect_ratio();
	gnu.set_grid();
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(box.begin(), box.end()));
	gnu.add(GA::Lines(l1, -1, 5));
	gnu.add(GA::Lines((*spl), -1, 5, 2));
	ASSERT_LE(l1.alpha() - spl->alpha(), 1e-8);
//	gnu.plot();
}

}

#endif
