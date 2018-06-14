/*
 * test_point.hpp
 *
 *  Created on: Jun 15, 2018
 *      Author: zhou
 */

#ifndef GEOMETRY_TEST_POINT_HPP_
#define GEOMETRY_TEST_POINT_HPP_

#include "geometry/geometry.hpp"

namespace carpio {

const St dim = 3;

typedef Point_<double, dim> Point;
typedef Point_<double, 3> Point3;
typedef Point_<double, 2> Point2;


TEST(point, point_initial){
	Point3 x(1, 0, 0);
	Point3 y(0, 1, 0);
	Point3 z(0, 0, 1);
	std::cout << "Initial three points and output/n";
	std::cout << "Point x = " << x << std::endl;
	std::cout << "Point y = " << y << std::endl;
	std::cout << "Point z = " << z << std::endl;
	ASSERT_EQ(x(0), 1);
	ASSERT_EQ(y(1), 1);

}

TEST(point, distance){
	Point2 x(0, 0);
	Point2 y(1, 1);
	Point2 z(0, 1);
	std::cout << "Point x = " << x << std::endl;
	std::cout << "Point y = " << y << std::endl;
	std::cout << "Square Distance from x to y = " << Distance2(x, y) << std::endl;
	std::cout << "       Distance from x to y = " << Distance(x, y) << std::endl;
}

TEST(point, max_and_min){
	Point3 x(1, 0, 0);
	Point3 y(0, 1, 0);
	Point3 z(0, 0, 1);
	std::cout << "Initial three points and output/n";
	std::cout << "Point x = " << x << std::endl;
	std::cout << "Point y = " << y << std::endl;
	std::cout << "Point z = " << z << std::endl;

	std::cout << "Max of x and y " << Max(x, y) << std::endl;
	std::cout << "Min of x and z " << Min(x, z) << std::endl;

	ASSERT_EQ(Min(x,z), Point3(0,0,0));
	ASSERT_EQ(Max(x,y), Point3(1,1,0));

}



}



#endif /* GEOMETRY_TEST_POINT_HPP_ */
