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
typedef Curve_<double, 2> Curve;
typedef BezierCurve_<double, 2> BC;
typedef BezierCurveQuadratic_<double, 2> BC2;
typedef BezierCurveCubic_<double, 2> BC3;
typedef BezierHighOrder_<double, 2>  BCH;
typedef GGnuplotActor_<double, 2> GA;
typedef PointChain_<double, 2> PC;

TEST(bezier, first){
	Point2 s(-0.3,   -0.1);
    Point2 c(1,        0);
	Point2 e(0.5,    0.5);

	std::shared_ptr<BC> spc(new BC2(s, c, e, 50));

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 1.5);
	gnu.set_yrange(-0.5, 1.5);
//	int count = 1;
//	for (auto& p : l) {
//		auto actor = GA::Points(p, 3);
//		actor->style() = "with points ps 2 pt 7 lc variable";
//		gnu.add(actor);
//		gnu.set_label(count, ToString(count-1), p.x() + 0.05, p.y(), "left" );
//		count++;
//	}
//	auto a_arrows = GA::Arrows(l, 2);
//	a_arrows->style() = "with vectors lw 2 lc variable";
//	gnu.add(a_arrows);
//	for (auto& p : curve){
//		std::cout << " P = " << p << std::endl;
//	}

	auto a_p = GA::Lines(spc->begin(), spc->end(), 4);
	gnu.add(a_p);
	gnu.add( GA::LinesPoints(spc->control_points().begin(),
			                 spc->control_points().end(), 0));
	gnu.add( GA::Points(spc->start_point(), 1));
	gnu.add( GA::Points(spc->end_point(), 1));
//	gnu.plot();

}

TEST(bezier, cubic){
	Point2 s(-0.3,   -0.1);
    Point2 c1(0.8,        0);
    Point2 c2(0.5,        1.0);
	Point2 e(0.5,    0.5);

	std::shared_ptr<BC> spc(new BC3(s, c1, c2, e, 50));

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 1.5);
	gnu.set_yrange(-0.5, 1.5);

	auto a_p = GA::Lines(spc->begin(), spc->end(), 4);
	gnu.add(a_p);
	gnu.add( GA::LinesPoints(spc->control_points().begin(),
			                 spc->control_points().end(), 0));
	gnu.add( GA::Points(spc->start_point(), 1));
	gnu.add( GA::Points(spc->end_point(), 1));
	gnu.plot();

}


TEST(bezier, high){
	std::list<Point2> l{
	Point2(-0.3,   -0.1),
    Point2(0.8,      0),
    Point2(0.5,    1.0),
	Point2(0.5,    0.5)};

	std::shared_ptr<BC> spc(new BCH(l, 50, "bernstein"));

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 1.5);
	gnu.set_yrange(-0.5, 1.5);

	auto a_p = GA::Lines(spc->begin(), spc->end(), 4);
	gnu.add(a_p);
	gnu.add( GA::LinesPoints(spc->control_points().begin(),
			                 spc->control_points().end(), 0));
	gnu.add( GA::Points(spc->start_point(), 1));
	gnu.add( GA::Points(spc->end_point(), 1));
	gnu.plot();

}


}
#endif
