#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"

using namespace carpio;

typedef Point_<double, 2> Point2;
typedef Point_<double, 3> Point3;
typedef Segment_<double, 2> Seg2;
typedef Curve_<double, 2> Curve;
typedef BezierCurve_<double, 2> BC;
typedef BezierCurveLinear_<double, 2> BC1;
typedef BezierCurveQuadratic_<double, 2> BC2;
typedef BezierCurveCubic_<double, 2> BC3;
typedef BezierHighOrder_<double, 2>  BCH;
typedef GGnuplotActor_<double, 2> GA;


void plot(std::string filename, const BC& curve){
    Gnuplot gnu;
    gnu.set_terminal_png(
            filename, //const std::string& filename,
            800,
            600,
            "Helvetica",
            14);
    gnu.set_xrange(-0.2, 1.2);
    gnu.set_yrange(-0.2, 1.2);
    gnu.set("key left top");
    auto a_line = GA::Lines(curve.begin(), curve.end(), 4);
    a_line->command() = "using 1:2:3 title \"Bezier Curve\" ";
    a_line->style()   = "with lines lw 2 lc 0";
    gnu.add(a_line);

    auto a_cp = GA::Points(curve.begin(),curve.end(), 0);
    a_cp->command() = "using 1:2:3 title \"Curve Points\" ";
    a_cp->style()   = "with point ps 1 lc variable";
    gnu.add(a_cp);

    auto a_conp = GA::Points(curve.control_points().begin(),
                             curve.control_points().end(), 6);
    a_conp->command() = "using 1:2:3 title \"Control Points\" ";
    a_conp->style()   = "with linespoint ps 2 lc variable";
    gnu.add(a_conp);

    auto a_ps = GA::Points(curve.start_point(), 1);
    a_ps->command() = "using 1:2:3 title \"Start Point\" ";
    a_ps->style()   = "with points ps 2 pt 7 lc variable";
    gnu.add(a_ps);
    auto a_pe = GA::Points(curve.end_point(), 2);
    a_pe->command() = "using 1:2:3 title \"End Point\" ";
    a_pe->style() =  "with points ps 2 pt 7 lc variable";

    gnu.add(a_pe);
    gnu.plot();
}

void bezier_linear(){
    Point2 s(0.0,   0.0);
    Point2 e(1.0,   1.0);

    std::shared_ptr<BC> spc(new BC1(s, e, 5));

    plot("./fig/b1.png", *spc);
}

void bezier_quadratic(){
    Point2 s(0.0,   0.0);
    Point2 c(0.3,   0.7);
    Point2 e(1.0,   1.0);

    std::shared_ptr<BC> spc(new BC2(s, c, e, 5));

    plot("./fig/b2.png", *spc);
}

void bezier_cubic(){
    Point2 s(0.0,   0.0);
    Point2 c1(0.7,   -0.1);
    Point2 c2(0.8,   1.1);
    Point2 e(1.0,   1.0);

    std::shared_ptr<BC> spc(new BC3(s, c1, c2, e, 20));

    plot("./fig/b3.png", *spc);
}

void bezier_high(){
    std::list<Point2> l{
        Point2(0.0,    0.0),
        Point2(0.2,    0.1),
        Point2(0.3,    1.0),
        Point2(0.6,    0.2),
        Point2(0.6,    0.6)
    };

    std::shared_ptr<BC> spc(new BCH(l, 20));

    plot("./fig/bh.png", *spc);
}

int main(int argc, char** argv) {
    bezier_linear();
    bezier_quadratic();
    bezier_cubic();
    bezier_high();
}