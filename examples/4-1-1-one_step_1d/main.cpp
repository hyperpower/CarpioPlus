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
    // equ.set_scheme("quick");

    // Set boundary condition
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    spBI spbi(new BoundaryIndex());
    // spbi->insert(0,std::make_shared<BC>(
    //         BoundaryConditionValue(BC::_BC1_, 1.0)));
    equ.set_boundary_index_phi(spbi);
    // Set initial condition
    equ.set_initial_phi([](Vt x, Vt y, Vt z, Vt t){
        if(x < 0.5){
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

    typedef EventOutputField_<DIM, Domain> EventOutputField;
    EventOutputField eos("phi", -1, -1, 1, Event::AFTER);
    eos.set_path("./data/");
    eos.set_format_string("FOU_%s_%d_%8.4e.txt");
    equ.add_event("OutputPhi", std::make_shared<EventOutputField>(eos));

    // Run
    equ.run();
}
