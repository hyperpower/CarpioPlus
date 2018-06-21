#ifndef GEOMETRY_TEST_POLYGON_BOOLEAN_HPP_
#define GEOMETRY_TEST_POLYGON_BOOLEAN_HPP_

#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

namespace carpio {

TEST(polygon_boolean, initial) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0.5, 0.5));
	lo.push_back(Point(2.6, 0.5));
	lo.push_back(Point(0.8, 2.5));
	PC pobject(lo);

	PolygonBoolean_<double> pb(pclip, pobject);

	Gnuplot gnu;
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
	gnu.add(pb.actor_clip(gnu));
	gnu.add(pb.actor_object(gnu));
	gnu.plot();


}



} //end namespace
#endif
