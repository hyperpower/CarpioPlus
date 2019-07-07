#include <iostream>
#include <memory> 
#include <string>
#include "geometry/geometry.hpp"
#include "domain/structure/structure.hpp"
#include "domain/structure/io/sgnuplot_actor.hpp"
#include "equation/equation.hpp"
#include "utility/tinyformat.hpp"

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

int field_1(const std::string& name){
    const St DIM = 1;
    typedef StructureDomain_<DIM> Domain;
    typename Domain::spGrid spgrid(
            new SGridUniform_<DIM>(
                    { 0.0, 0.0 }, // min point
                    { 10,  10 },  // num on each direction
                      0.1,        // cell size
                      2));        // ghost layer
    typename Domain::spGhost spghost(new SGhostRegular_<DIM>(spgrid));
    typename Domain::spOrder sporder(new SOrderXYZ_<DIM>(spgrid, spghost));

    // Define the equation
    Laplace_<DIM, Domain> equ(spgrid, spghost, sporder);

    // Set boundary condition
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef std::shared_ptr<BoundaryCondition> spBC;
    spBI spbi(new BoundaryIndex());
    spBC spbcxm(new BoundaryConditionValue(BC::_BC1_, 1.0));
    spBC spbcym(new BoundaryConditionValue(BC::_BC1_, 0.0));
    spbi->insert(0, spbcxm);
    spbi->insert(1, spbcym);

    equ.set_boundary_index_phi(spbi);

    // Set time term
    Vt dt = 6e-3;
    equ.set_time_term(10, dt);

    // Set solver
    equ.set_time_scheme(name, 0.9);
    equ.set_solver("CG", 200, 1e-4);

    // Add events
    typedef Event_<DIM, Domain> Event;
    typedef std::shared_ptr<Event_<DIM, Domain> >  spEvent;
    spEvent spetime(new EventOutputTime_<DIM, Domain>(std::cout,
                                                  -1, -1, 1, Event::AFTER | Event::END));
    equ.add_event("OutputTime", spetime);

    typedef EventOutputField_<DIM, Domain> EventOutputField;
    EventOutputField eos("phi", -1, -1, 1, Event::AFTER);
    eos.set_path("./data/");
    equ.add_event("OutputPhi", std::make_shared<EventOutputField>(eos));

    typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
    EventGnuplotField::FunPlot fun = [&dt, &name](
                           Gnuplot& gnu,
                     const EventGnuplotField::Field& f,
                     St step, Vt t, int fob,
                     EventGnuplotField::pEqu pd){
        typedef typename Domain::GnuplotActor GA;
        auto actor = GA::LinesPoints(f);
        actor->style() = "with linespoints lw 2 lc variable";
        gnu.add(actor);
        gnu.set_label(1,tfm::format("%s", name), 0.8, 0.9);
        gnu.set_label(2,tfm::format("Time = %.3e", t), 0.8, 0.8);
        gnu.set_label(3,tfm::format("Step = %d", step), 0.8, 0.7);
        gnu.set_label(4,tfm::format("Fo   = dt / dx 2 = %.3f" , dt /0.01), 0.8, 0.6);
        gnu.plot();
        gnu.clear();
        return 1;
    };
    EventGnuplotField egs("phi", fun, -1, -1, 1, Event::AFTER);
    egs.gnuplot().set_xrange(-0.1, 1.1);
    egs.gnuplot().set_yrange(-0.1, 1.1);
    egs.gnuplot().set_ylabel("phi");
    egs.gnuplot().set_xlabel("x");
    egs.set_path("./fig/");
    egs.set_format_string(name + "_%s_%d_%8.4e.png");
    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

    equ.run();
    
    return 0;
}


int main(int argc, char** argv) {
    field_1("explicit");
    field_1("implicit");
    field_1("CN");
}