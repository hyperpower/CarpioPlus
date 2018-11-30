/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */
#ifndef GEOMETRY_TEST_BEZIER_HPP_
#define GEOMETRY_TEST_BEZIER_HPP_
#include "geometry/geometry.hpp"

#include "gtest/gtest.h"
#include <list>

namespace carpio {


typedef Point_<double, 2> Point2;
typedef Point_<double, 3> Point3;
typedef Segment_<double, 2> Seg2;
typedef Box_<double, 2> Box2;
typedef BezierCurve_<double, 2> BC2;
typedef GGnuplotActor_<double, 2> GA;
typedef PointChain_<double, 2> PC;

TEST(bezier, first){
	std::list<Point2> l =
		{ Point2(-0.3,   -0.1),
		  Point2(1,   0),
		  Point2(0.5, 0.5),
		  Point2(0.9, 1.2),
		  Point2(0.3, 1.5)};

	BC2 curve(l,3);

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 1.5);
	gnu.set_yrange(-0.5, 1.5);
	int count = 1;
	for (auto& p : l) {
		auto actor = GA::Points(p, 3);
		actor->style() = "with points ps 2 pt 7 lc variable";
		gnu.add(actor);
		gnu.set_label(count, ToString(count-1), p.x() + 0.05, p.y(), "left" );
		count++;
	}
	auto a_arrows = GA::Arrows(l, 2);
	a_arrows->style() = "with vectors lw 2 lc variable";
	gnu.add(a_arrows);
	for (auto& p : curve){
		std::cout << " P = " << p << std::endl;
	}
	auto a_p = GA::LinesPoints(curve.begin(), curve.end(), 4);
	gnu.add(a_p);
	gnu.plot();

}
}
#endif
