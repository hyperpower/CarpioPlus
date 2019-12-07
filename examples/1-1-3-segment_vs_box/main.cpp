#include <iostream>
#include <memory>
#include <string>
#include "geometry/geometry.hpp"

using namespace carpio;

typedef Point_<double, 2>              Point2;
typedef Segment_<double, 2>            Seg2;
typedef Box_<double, 2>                Box2;
typedef Line_<double>                  Line;
typedef GGnuplotActor_<double, 2>      GA;


// Forware Declare
int line_box_normal_case();

int main(){
    int r = line_box_normal_case();
}

int line_box_normal_case(){
    std::cout << "Initial box" << std::endl;
    Point2 min1(0, 0);
    Point2 max1(1, 1);
    Box2 box1(min1, max1);
    std::cout << "The box 1 is " << box1 << std::endl;

    Line line(1.5, 1.0, 1.3);
    std::cout << "Line is " << line << std::endl;

    auto lspp = IntersectLineBox(max1, line);
    std::cout << "Result points : " << lspp.size() << std::endl;
    for(auto& sp : lspp){
        std::cout << "   " << *sp << std::endl;
    }

    Gnuplot gnu;
    gnu.set_xrange(-0.5, 1.5);
    gnu.set_yrange(-0.5, 1.5);
    gnu.set_equal_aspect_ratio();
    gnu.set_terminal_png("./fig/line_box_normal.png");
    auto spbox1 = GA::Lines(box1);
    spbox1->style() = "with lines lw 2 lc 8";
    gnu.add(spbox1);
    auto spline = GA::Lines(line, -0.5, 1.5);
    spline->style() = "with lines lw 3 lc 6";
    gnu.add(spline);
    // add intersection points
    int numlabel = 30;
    for(auto& sp : lspp){
        auto sppoint = GA::Points(*sp);
        sppoint->style() = "with points pt 7 ps 2 lc 7";
        gnu.set_label(numlabel, tfm::format("(%.2f, %.2f)", sp->x(), sp->y()),
                     sp->x(), sp->y()+ 0.05, "left font \",16\"");
        gnu.add(sppoint);
        numlabel++;
    }
    gnu.set_label(1,"(1.0, 1.0)", 1.0, 1.05, "center font \",16\"");
    gnu.set_label(2,"(0.0, 0.0)", 0.0, -0.05, "center font \",16\"");

    gnu.set_label(4,tfm::format("Line : %.1f X + %.1f Y = %.1f", line.a(), line.b(), line.alpha()),
                    0.1, 1.3, "left font \",16\"");
    gnu.plot();
}