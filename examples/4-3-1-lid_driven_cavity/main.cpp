#include "equation/equation.hpp"
#include "domain/structure/structure.hpp"
#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/boundary_index.hpp"

using namespace carpio;

const St DIM = 1;
typedef StructureDomain_<DIM> Domain;
typedef typename Domain::spGrid  spGrid;
typedef typename Domain::spGhost spGhost;
typedef typename Domain::spOrder spOrder;

int main(int argc, char** argv) {
    spGrid spgrid(
            new SGridUniform_<DIM>({0.0, 0.0}, // min point
                                   {100, 10},  // num on each direction
                                    0.02,      // cell size
                                    2));       // ghost layer
    spGhost spghost(
            new SGhostRegular_<DIM>(spgrid));

    spOrder sporder(
            new SOrderXYZ_<DIM>(spgrid, spghost));

    // Define the equation
    Convection_<DIM, Domain> equ(spgrid, spghost, sporder);

    equ.set_time_term(100, 0.01);

    // Set boundary condition
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    spBI spbi(new BoundaryIndex());
    // spbi->insert(0,std::make_shared<BC>(
    //         BoundaryConditionValue(BC::_BC1_, 1.0)));
    equ.set_boundary_index_phi(spbi);
    // Set initial condition
    equ.set_initial_phi([](Vt x, Vt y, Vt z, Vt t){
        if((x > 0.25) && (x < 0.75)){
            return 1.0;
        }else{
            return 0.0;
        }
    });
    equ.set_initial_velocity(_X_, [](Vt, Vt, Vt, Vt){return 1.0;});

    // Add events
    typedef Event_<DIM, Domain> Event;
    typedef std::shared_ptr<Event_<DIM, Domain> >  spEvent;
    spEvent spetime(new EventOutputTime_<DIM, Domain>(std::cout,
                                                      -1, -1, 1, Event::AFTER));
    equ.add_event("OutputTime", spetime);

    typedef EventOutputScalar_<DIM, Domain> EventOutputScalar;
    EventOutputScalar eos("phi", -1, -1, 1, Event::AFTER);
    eos.set_path("./data/");
    equ.add_event("OutputPhi", std::make_shared<EventOutputScalar>(eos));

    typedef EventGnuplotScalar_<DIM, Domain> EventGnuplotScalar;
    EventGnuplotScalar egs("phi", -1, -1, 1, Event::AFTER);
    egs.gnuplot().set_yrange(-0.3, 1.3);
    egs.set_path("./fig/");
    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotScalar>(egs));

    // Run
    equ.run();
}
