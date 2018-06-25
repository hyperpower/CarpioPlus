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

	pb.show_table();
	Gnuplot gnu;
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
	gnu.add(pb.actor_clip(gnu));
	gnu.add(pb.actor_object(gnu));
	gnu.add(pb.actor_label(gnu));
//	gnu.plot();
}

TEST(polygon_boolean, test2) {
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
	lo.push_back(Point(2  , 0.8));
	lo.push_back(Point(0.8, 2.5));
	PC pobject(lo);

	PolygonBoolean_<double> pb(pclip, pobject);

	pb.show_table();
	Gnuplot gnu;
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
	gnu.add(pb.actor_clip(gnu));
	gnu.add(pb.actor_object(gnu));
	gnu.add(pb.actor_label(gnu));
//	gnu.plot();
}

TEST(polygon_boolean, test3) {
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
	lo.push_back(Point(2.0, 2.0));
	lo.push_back(Point(0.8, 2.5));
	PC pobject(lo);

	PolygonBoolean_<double> pb(pclip, pobject);

	pb.show_table();
	Gnuplot gnu;
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
	gnu.add(pb.actor_clip(gnu));
	gnu.add(pb.actor_object(gnu));
	gnu.add(pb.actor_label(gnu));
//	gnu.plot();
}

TEST(polygon_boolean, test4) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point( 0.5, 0.5));
	lo.push_back(Point( 2.0, 2.0));
	lo.push_back(Point(-0.8, 2.0));

	PC pobject(lo);

	PolygonBoolean_<double> pb(pclip, pobject);

	pb.show_table();
	Gnuplot gnu;
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
//	gnu.add(pb.actor_clip(gnu));
	gnu.add(pb.actor_object(gnu));
//	gnu.add(pb.actor_label(gnu));
	gnu.plot();
}

TEST(polygon_boolean, test5) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point( 0.5, 0.5));
	lo.push_back(Point( 2.0, 2.0));
	lo.push_back(Point( 1.5, 2.5));
	lo.push_back(Point( 0.8, 1.5));
	lo.push_back(Point( 0.2, 2.5));

	PC pobject(lo);

	PolygonBoolean_<double> pb(pclip, pobject);

	pb.show_table();
	Gnuplot gnu;
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
	gnu.add(pb.actor_clip(gnu));
	gnu.add(pb.actor_object(gnu));
//	gnu.add(pb.actor_label(gnu));
//	gnu.plot();
}

TEST(polygon_boolean, test6) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point( 1.0, 1.0));
	lo.push_back(Point( 3.0, 1.0));
	lo.push_back(Point( 1.0, 3.0));
	lo.push_back(Point( 0.5, 1.5));

	PC pobject(lo);

	PolygonBoolean_<double> pb(pclip, pobject);

	pb.show_table();
	Gnuplot gnu;
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
	gnu.add(pb.actor_clip(gnu));
	gnu.add(pb.actor_object(gnu));
//	gnu.add(pb.actor_label(gnu));
//	gnu.plot();
}

TEST(polygon_boolean, test7) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point( 0.0, -1.0));
	lo.push_back(Point( 2.0, -1.0));
	lo.push_back(Point( 2.0,  3.0));
	lo.push_back(Point( 0.0,  3.0));

	PC pobject(lo);

	PolygonBoolean_<double> pb(pclip, pobject);

	pb.show_table();
	Gnuplot gnu;
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
	gnu.add(pb.actor_clip(gnu));
	gnu.add(pb.actor_object(gnu));
//	gnu.add(pb.actor_label(gnu));
//	gnu.plot();
}
TEST(polygon_boolean, test8) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point( 0.0, 0.5));
	lo.push_back(Point( 2.0, 0.5));
	lo.push_back(Point( 2.0, 1.5));
	lo.push_back(Point( 0.0, 1.5));

	PC pobject(lo);

	PolygonBoolean_<double> pb(pclip, pobject);

	pb.show_table();
	Gnuplot gnu;
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
	gnu.add(pb.actor_clip(gnu));
//	gnu.add(pb.actor_object(gnu));
//	gnu.add(pb.actor_label(gnu));
//	gnu.plot();
}

TEST(polygon_boolean, test9) {
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> lc;
	lc.push_back(Point(0, 0));
	lc.push_back(Point(2, 0));
	lc.push_back(Point(2, 2));
	lc.push_back(Point(0, 2));
	PC pclip(lc);

	std::list<Point> lo;
	lo.push_back(Point(0, 0));
	lo.push_back(Point(2, 0));
	lo.push_back(Point(2, 2));
	lo.push_back(Point(0, 2));

	PC pobject(lo);
	std::cout<< "here\n";

	PolygonBoolean_<double> pb(pclip, pobject);

	pb.show_table();
	Gnuplot gnu;
	gnu.set_xrange(-2, 4);
	gnu.set_yrange(-2, 4);
	gnu.add(pb.actor_clip(gnu));
	gnu.add(pb.actor_object(gnu));
//	gnu.add(pb.actor_label(gnu));
//	gnu.plot();
}




} //end namespace
#endif
