#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"

using namespace carpio;

typedef GGnuplotActor_<double, 2> GA;
typedef SField_<1>                Field1;
typedef SField_<2>                Field2;
typedef SField_<3>                Field3;
typedef std::shared_ptr<SGrid_<1> >     spGrid1;
typedef std::shared_ptr<SGhost_<1> >    spGhost1;
typedef std::shared_ptr<SOrderXYZ_<1> > spOrder1;

int main(int argc, char** argv) {
    std::cout << "field_define 1d\n";
    Point_<Vt, 1> pmin(0, 0, 0);
    Point_<Vt, 1> pmax(1, 1, 1);
    spGrid1  spsg(new SGridUniform_<1>(pmin, { 5, 5 }, 0.3, 2));
    spGhost1 spgh(new SGhostRegular_<1>(spsg));
    spOrder1 sporder(new SOrderXYZ_<1>(spsg, spgh));

    Field1 sc(spsg, spgh, sporder);
    sc.assign([](Vt x, Vt y, Vt z, Vt t){
        return sin(x);
    });

    typedef SGnuplotActor_<1> GA;
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/1d.png");
    gnu.set_xrange(-0.1, 1.6);
    auto awf = GA::WireFrame(*spsg);
    auto acp = GA::CenterPoints(*spsg);
    acp->style() = "with points pt 2 ps 2 lc variable";
    auto af  = GA::LinesPoints(sc, 0);
    af->style() = "with linespoints pt 7 ps 2 lc variable";
    gnu.add(awf);
    gnu.add(acp);
    gnu.add(af);
    gnu.plot();


    
    return 0;
}