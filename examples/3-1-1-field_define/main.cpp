#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"
#include "domain/structure/io/sgnuplotactor_maker.hpp"

using namespace carpio;

typedef GGnuplotActor_<double, 2> GA;

typedef SGridUniform_<1>             Grid1;
typedef SGhostRegular_<1, Grid1>     Ghost1;
typedef SOrderXYZ_<1, Grid1, Ghost1> Order1;


int field_1d(){
    std::cout << "field_define 1d\n";
    Point_<Vt, 1> pmin(0, 0, 0);
    Point_<Vt, 1> pmax(1, 1, 1);
    std::shared_ptr<Grid1> spsg(new Grid1(pmin, { 5, 5 }, 0.5, 2));
    std::shared_ptr<Ghost1> spgh(new Ghost1(spsg));
    std::shared_ptr<Order1> sporder(new Order1(spsg, spgh));

    typedef SField_<1, Grid1, Ghost1, Order1> Field1;
    Field1 sc(spsg, spgh, sporder);
    sc.assign([](Vt x, Vt y, Vt z, Vt t){
        return sin(x);
    });

    typedef SGnuplotActorMaker_<1, Grid1, Ghost1, Order1> GAMaker;
    Gnuplot gnu;
    GAMaker gamaker;
    gnu.set_terminal_png("./fig/1d.png");
    gnu.set_xrange(-0.1, 2.6);
    auto awf = gamaker.wire_frame(*spsg);
    auto acp = gamaker.center_points(*spsg);
    acp->style() = "with points pt 2 ps 2 lc variable";
    auto af  = gamaker.lines_points(sc, 0);
    af->style()  = "with linespoints pt 7 ps 2 lc variable";
    gnu.add(awf);
    gnu.add(acp);
    gnu.add(af);
    gnu.plot();
    
    return 0;
}

// int field_2d(){
//     std::cout << "field_define 2d\n";
//     Point_<Vt, 2> pmin(0, 0, 0);
//     Point_<Vt, 2> pmax(1, 1, 1);
//     spGrid2  spsg(   new SGridUniform_<2>(pmin, { 5, 5 }, 0.5, 2));
//     spGhost2 spgh(   new SGhostRegular_<2>(spsg));
//     spOrder2 sporder(new SOrderXYZ_<2>(spsg, spgh));

//     Field2 sc(spsg, spgh, sporder);
//     sc.assign([](Vt x, Vt y, Vt z, Vt t){
//         return sin(x+y);
//     });

//     typedef SGnuplotActor_<2> GA;
//     Gnuplot gnu;
//     gnu.set_terminal_png("./fig/2d.png");
//     gnu.set_xrange(-0.1, 2.6);
//     gnu.set_yrange(-0.1, 2.6);
//     gnu.set_ylabel("y");
//     gnu.set_xlabel("x");
//     gnu.set_equal_aspect_ratio();
//     auto awf = GA::WireFrame(*spsg);
//     auto ac  = GA::Contour(sc);
    
//     gnu.add(ac);
//     gnu.add(awf);
//     gnu.plot();
    
//     return 0;
// }

// int field_3d(){
//     std::cout << "field_define 3d\n";
//     Point_<Vt, 3> pmin(0, 0, 0);
//     Point_<Vt, 3> pmax(1, 1, 1);
//     spGrid3  spsg(   new SGridUniform_<3>(pmin, { 5, 5, 5}, 0.5, 2));
//     spGhost3 spgh(   new SGhostRegular_<3>(spsg));
//     spOrder3 sporder(new SOrderXYZ_<3>(spsg, spgh));

//     Field3 sc(spsg, spgh, sporder);
//     sc.assign([](Vt x, Vt y, Vt z, Vt t){
//         return sin(x+y+z);
//     });

//     typedef SGnuplotActor_<3> GA;
//     Gnuplot gnu;
//     gnu.set_terminal_png("./fig/3d.png");
//     gnu.set_xrange(-0.1, 2.6);
//     gnu.set_yrange(-0.1, 2.6);
//     gnu.set_zrange(-0.1, 2.6);
//     gnu.set_ylabel("y");
//     gnu.set_xlabel("x");
//     gnu.set_equal_aspect_ratio();
//     auto awf = GA::WireFrame(*spsg);
//     // auto ac  = GA::Contour(sc);
    
//     // gnu.add(ac);
//     gnu.add(awf);
//     gnu.splot();
    
//     return 0;
// }

int main(int argc, char** argv) {
    field_1d();
    // field_2d();
    // field_3d();
}