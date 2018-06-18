#ifndef GEOMETRY_TEST_POINT_CHAIN_HPP_
#define GEOMETRY_TEST_POINT_CHAIN_HPP_

#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

namespace carpio {

TEST(point_chain, initial){
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> l;
	l.push_back(Point(0,0));
	l.push_back(Point(1,0));
	l.push_back(Point(1,1));
	l.push_back(Point(0,1));

	PC pc(l);
	Gnuplot gnu;
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GnuplotActor::Arrows(pc, 0));
	gnu.plot();



}



}

#endif
