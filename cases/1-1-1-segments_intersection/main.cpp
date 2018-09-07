#include <iostream>
#include <memory> 
#include "geometry/geometry.hpp"

using namespace carpio;

typedef IntersectionPairSS_<double, 2> Inter;
typedef Point_<double, 2>              Point2;
typedef Segment_<double, 2>            Seg2;

int main(int argc, char** argv) {
    Point2 p1(  -1,    0);
    Point2 p2(   3,  0.5);
    Point2 p3( 0.8,  2.0);
    Point2 p4(-0.3, -1.0);
    Seg2 seg1(p1, p2);
    Seg2 seg2(p3, p4);

    Inter inter(seg1, seg2);
    auto strtype = ToString(inter.cal_intersection_type());
    std::cout << "Intersection Type : "<< strtype << std::endl;
    Point2 np = inter.cal_intersection_point();
    std::cout << "new point         : "<< np << std::endl;

    Gnuplot gnu;
    gnu.set_terminal_png("i1.png");
    gnu.set_xrange(-5, 5);
    gnu.set_yrange(-5, 5);
    gnu.set_label(1, strtype, -4.5, 4);
    gnu.add(GnuplotActor::LinesPoints(seg1, 0));
    gnu.add(GnuplotActor::LinesPoints(seg2, 1));
    gnu.add(GnuplotActor::Points(np, 3));
    gnu.plot();
}