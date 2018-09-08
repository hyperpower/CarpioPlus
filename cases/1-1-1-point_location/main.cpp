#include <iostream>
#include <memory> 
#include "geometry/geometry.hpp"

using namespace carpio;

typedef Point_<double, 2>              Point2;
typedef Segment_<double, 2>            Seg2;

int main(int argc, char** argv) {
    Point2 p1( 0.0, 0.0);
    Point2 p2( 1.0, 1.0);
    Point2 p3( 0.8,  2.0);
    
    Seg2 seg1(p1, p2);
    
    
}