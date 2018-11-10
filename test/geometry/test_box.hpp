/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */
#ifndef GEOMETRY_TEST_BOX_HPP_
#define GEOMETRY_TEST_BOX_HPP_
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"
namespace carpio {


typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;
typedef Segment_<double, 2> Seg2;
typedef Box_<double, 2> Box2;

TEST(box, point_to_box_position){
	Point2 x(1, 0);
	Point2 y(0, 1);
	std::cout << "Initial three points and output" << std::endl;
	std::cout << "Point x = " << x << std::endl;
	std::cout << "Point y = " << y << std::endl;
	Point2 p(0.5, 0.5);

	Box2 box(x, y);
	std::cout << "The box is " << box << std::endl;

	int code = PointToBoxPositionCode(box.min(), box.max(), p);
	ASSERT_EQ(code, 33);
	std::cout << "code = " << code << std::endl;

	ASSERT_EQ(PointToBoxPositionCode(box.min(), box.max(), Point2(-0.1,  0.1)), 31);
	ASSERT_EQ(PointToBoxPositionCode(box.min(), box.max(), Point2(-0.1, -0.1)), 11);
	ASSERT_EQ(PointToBoxPositionCode(box.min(), box.max(), Point2( 0.0,  0.0)), 22);

	ASSERT_EQ(IsInOn(box, Point2( 0.1, 0.1)), true);
	ASSERT_EQ(IsInOn(box, Point2(-0.1, 0.1)), false);
	ASSERT_EQ(IsInOn(box, Point2( 1.0, 1.0)), true);
}

TEST(box, box_box_in_on){
	std::cout << "Initial 2 boxs" << std::endl;
	Point2 min1(1, 0);
	Point2 max1(0, 1);
	Box2 box1(min1, max1);
	Box2 box2(Point2(0.5, 0.5), Point2(1.5, 1,5));
	std::cout << "The box 1 is " << box1 << std::endl;
	std::cout << "The box 2 is " << box2 << std::endl;

	ASSERT_EQ(IsInOn(box1, box2), true);
	Box2 box3(Point2(1., 1.), Point2(1.5, 1.5));
	std::cout << "The box 3 is " << box3 << std::endl;
	ASSERT_EQ(IsInOn(box1, box3), true);
}




} //end namespace
#endif
