/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */

#ifndef GEOMETRY_TEST_SEGMENT_HPP_
#define GEOMETRY_TEST_SEGMENT_HPP_
#include "geometry/geometry.hpp"

namespace carpio {


typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef Segment_<double, 2> Seg2;

TEST(segment, segment_initial){
	Point2 x(1, 0);
	Point2 y(0, 1);
	std::cout << "Initial three points and output/n";
	std::cout << "Point x = " << x << std::endl;
	std::cout << "Point y = " << y << std::endl;

	Seg2 seg(x, y);
	std::cout << "The segemnt is " << seg << std::endl;
	ASSERT_EQ(seg.ps(), x);
	ASSERT_EQ(seg.pe(), y);
}


TEST(segment, segment_plot){
	Point2 x(1, 0);
	Point2 y(0, 1);
	std::cout << "Initial three points and output/n";
	std::cout << "Point x = " << x << std::endl;
	std::cout << "Point y = " << y << std::endl;

	Seg2 seg(x, y);
	Gnuplot gnu;
	gnu.add(GnuplotActor::LinePoints(seg));
//	gnu.plot();
	std::cout << "The segemnt is " << seg << std::endl;
	ASSERT_EQ(seg.ps(), x);
	ASSERT_EQ(seg.pe(), y);
}

TEST(segment, segment_point_location){
	Point2 x(0, 0);
	Point2 y(0, 1);
	Seg2 seg(x, y);

	Point2 p(0.0, 1.50);

	Gnuplot gnu;
	gnu.add(GnuplotActor::Points(p));
	gnu.add(GnuplotActor::LinePoints(seg));
//	gnu.plot();
	std::cout << "The segemnt is :" << seg << std::endl;
	std::cout << "The point is   :" << p << std::endl;
	auto side = OnWhichSide7(seg, p);
	std::cout << "Position       :" << ToString(side) << std::endl;
//	ASSERT_EQ(side, _PS_RIGHT_);
}







} //end namespace
#endif
