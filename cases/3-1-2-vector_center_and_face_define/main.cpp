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

typedef std::shared_ptr<SGrid_<2> >     spGrid2;
typedef std::shared_ptr<SGhost_<2> >    spGhost2;
typedef std::shared_ptr<SOrderXYZ_<2> > spOrder2;

typedef std::shared_ptr<SGrid_<3> >     spGrid3;
typedef std::shared_ptr<SGhost_<3> >    spGhost3;
typedef std::shared_ptr<SOrderXYZ_<3> > spOrder3;

int vector_center_1d(){
    std::cout << "vector_center_define 1d\n";
    // typedef -----------------------------------------
    const   St Dim = 1;
    typedef SField_<Dim>                            Field;
    typedef std::shared_ptr<Field>                spField;
    typedef std::shared_ptr<SGrid_<Dim> >         spGrid;
    typedef std::shared_ptr<SGhost_<Dim> >        spGhost;
    typedef std::shared_ptr<SOrderXYZ_<Dim> >     spOrder;
    typedef SVectorCenter_<Dim>                     VC;
    typedef std::shared_ptr<SVectorCenter_<Dim> > spVC;
    typedef Point_<Vt, Dim>                           Point;

    // Program -----------------------------------------
    Point pmin(0, 0, 0);
    Point pmax(1, 1, 1);
    spGrid  spsg(new SGridUniform_<Dim>(pmin, { 5, 5 }, 0.3, 2));
    spGhost spgh(new SGhostRegular_<Dim>(spsg));
    spOrder sporder(new SOrderXYZ_<Dim>(spsg, spgh));

    spField spf(new Field(spsg, spgh, sporder));
    spf->assign([](Vt x, Vt y, Vt z, Vt t) {
        return sin(x+0.5);
    });

    spVC spvc(new VC(spf));

    typedef SGnuplotActor_<Dim> GA;
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/vc_1d.png");
    gnu.set_xrange(-0.1, 1.6);
    auto awf = GA::WireFrame(*spsg);
    auto acp = GA::CenterPoints(*spsg);
    acp->style() = "with points pt 2 ps 2 lc variable";
    auto af = GA::LinesPoints(*spvc, 0);
    af->style() = "with linespoints pt 7 ps 2 lc variable";
    auto aa = GA::Arrows(*spvc);
    aa->style() = "with vector lc palette";
    gnu.add(awf);
    gnu.add(acp);
    gnu.add(af);
    gnu.add(aa);
    gnu.plot();
    
    return 0;
}

int vector_center_2d(){
    std::cout << "vector_center_define 2d\n";
    // typedef -----------------------------------------
    const   St Dim = 2;
    typedef SField_<Dim>                            Field;
    typedef std::shared_ptr<Field>                spField;
    typedef std::shared_ptr<SGrid_<Dim> >         spGrid;
    typedef std::shared_ptr<SGhost_<Dim> >        spGhost;
    typedef std::shared_ptr<SOrderXYZ_<Dim> >     spOrder;
    typedef SVectorCenter_<Dim>                     VC;
    typedef std::shared_ptr<SVectorCenter_<Dim> > spVC;
    typedef Point_<Vt, Dim>                           Point;

    // Program -----------------------------------------
    Point pmin(0, 0, 0);
    Point pmax(1, 1, 1);
    spGrid  spsg(new SGridUniform_<Dim>(pmin, { 5, 5, 5 }, 0.3, 2));

    spGhost spgh(new SGhostRegular_<Dim>(spsg));

    spOrder sporder(new SOrderXYZ_<Dim>(spsg, spgh));

    spField spfx(new Field(spsg, spgh, sporder));
    spfx->assign([](Vt x, Vt y, Vt z, Vt t) {
        return sin(x);
    });

    spField spfy(new Field(spsg, spgh, sporder));
    spfy->assign([](Vt x, Vt y, Vt z, Vt t) {
        return sin(y);
    });

    spVC spvc(new VC(spfx, spfy));

    typedef SGnuplotActor_<Dim> GA;
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/vc_2d.png");
    gnu.set_xrange(-0.1, 1.6);
    gnu.set_yrange(-0.1, 1.6);
    gnu.set_equal_ratio();
    auto awf = GA::WireFrame(*spsg);
    auto acp = GA::CenterPoints(*spsg);
    acp->style() = "with points pt 2 ps 2 lc variable";
    auto aa = GA::Arrows(*spvc);
    aa->style() = "with vector lc palette";
    gnu.add(awf);
    gnu.add(acp);
    gnu.add(aa);
    gnu.plot();

    return 0;
}

int vector_center_3d(){
    std::cout << "vector_center_define 3d\n";
    // typedef -----------------------------------------
    const   St Dim = 3;
    typedef SField_<Dim>                            Field;
    typedef std::shared_ptr<Field>                spField;
    typedef std::shared_ptr<SGrid_<Dim> >         spGrid;
    typedef std::shared_ptr<SGhost_<Dim> >        spGhost;
    typedef std::shared_ptr<SOrderXYZ_<Dim> >     spOrder;
    typedef SVectorCenter_<Dim>                     VC;
    typedef std::shared_ptr<SVectorCenter_<Dim> > spVC;
    typedef Point_<Vt, Dim>                           Point;

    // Program -----------------------------------------
    Point pmin(0, 0, 0);
    Point pmax(1, 1, 1);
    spGrid  spsg(new SGridUniform_<Dim>(pmin, { 5, 5, 5 }, 0.3, 2));

    spGhost spgh(new SGhostRegular_<Dim>(spsg));

    spOrder sporder(new SOrderXYZ_<Dim>(spsg, spgh));

    spField spfx(new Field(spsg, spgh, sporder));
    spfx->assign([](Vt x, Vt y, Vt z, Vt t) {
        return sin(x);
    });

    spField spfy(new Field(spsg, spgh, sporder));
    spfy->assign([](Vt x, Vt y, Vt z, Vt t) {
        return sin(y);
    });

    spField spfz(new Field(spsg, spgh, sporder));
    spfz->assign([](Vt x, Vt y, Vt z, Vt t) {
        return sin(z);
    });

    spVC spvc(new VC(spfx, spfy, spfz));

    typedef SGnuplotActor_<Dim> GA;
    Gnuplot gnu;
    gnu.set_terminal_png("./fig/vc_3d.png");
    gnu.set_xrange(-0.1, 1.6);
    gnu.set_yrange(-0.1, 1.6);
    gnu.set_equal_ratio();
    auto awf = GA::WireFrame(*spsg);
    auto aa = GA::Arrows(*spvc);
    aa->style() = "with vector lc palette";
    gnu.add(aa);
    gnu.splot();
}


int main(int argc, char** argv) {
    vector_center_1d();
    vector_center_2d();
    vector_center_3d();
}