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
//	gnu.plot();

}

TEST(point_chain, winding_number){
	typedef Point_<double, 2> Point;
	typedef PointChain_<double, 2> PC;

	std::list<Point> l;
	l.push_back(Point(0,   0));
	l.push_back(Point(1,   0));
	l.push_back(Point(0.5 ,0.5));
	l.push_back(Point(1,   1));
	l.push_back(Point(0,   1));

	PC pc(l);

	std::list<Point> ltp;
	ltp.push_back(Point(0.6, 1.8));
	ltp.push_back(Point(0.6, 0.8));
	ltp.push_back(Point(0.8, 0.5));
	ltp.push_back(Point(0.3, 1.0));
	ltp.push_back(Point(0,   1.0));
	ltp.push_back(Point(0,   0.5));
	ltp.push_back(Point(0.8, 0.3));
	ltp.push_back(Point(0.8, 0.0));

	Gnuplot gnu;
	gnu.set_xrange(-1, 2);
	gnu.set_yrange(-1, 2);
	int count = 1;
	for(auto& p : ltp){
		bool wn = IsInOn(p, pc);
		gnu.add(GnuplotActor::Points(p, 3));
		gnu.set_label(count, wn?"T":"F", p.x(), p.y(), "left" );
		count++;
	}
	gnu.add(GnuplotActor::Arrows(pc, 0));
	gnu.plot();

}


//TEST(point_chain, winding_number_2){
//	typedef Point_<double, 2> Point;
//	typedef PointChain_<double, 2> PC;
//
//	std::list<Point> l;
//	l.push_back(Point(0,   0));
//	l.push_back(Point(0,   1));
//	l.push_back(Point(1,   1));
//	l.push_back(Point(0.5 ,0.5));
//	l.push_back(Point(1,   0));
//
//	PC pc(l);
//
//	std::list<Point> ltp;
//	ltp.push_back(Point(0.6, 1.8));
//	ltp.push_back(Point(0.6, 0.8));
//	ltp.push_back(Point(0.8, 0.5));
//	ltp.push_back(Point(0.3, 1.0));
//	ltp.push_back(Point(0,   1.0));
//	ltp.push_back(Point(0,   0.3));
//	ltp.push_back(Point(0.8, 0.3));
//	ltp.push_back(Point(0.8, 0.0));
//
//
//	Gnuplot gnu;
//	gnu.set_xrange(-1, 2);
//	gnu.set_yrange(-1, 2);
//	int count = 1;
//	for(auto& p : ltp){
//		int wn = IsInOn(p, pc);
//		gnu.add(GnuplotActor::Points(p, 3));
//		gnu.set_label(count, "" + ToString(wn), p.x(), p.y(), "left" );
//		count++;
//	}
//	gnu.add(GnuplotActor::Arrows(pc, 0));
//	gnu.plot();
//
//}





}

#endif
