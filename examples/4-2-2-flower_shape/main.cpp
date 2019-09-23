#include <iostream>
#include <memory> 
#include <string>
#include "equation/equation.hpp"


using namespace carpio;

int main(int argc, char** argv) {
    std::cout << "===== flower shape =====" << std::endl;
    const St DIM = 2;
    typedef SGnuplotActor_<2> GA;
    typedef StructureDomain_<2> Domain;
    typedef SGrid_<2> SGrid;
    typedef typename SGrid::Index SIndex;
    typedef SCellMask_<2> SCellMask;
    typedef SGhostMask_<2> SGhostMask;
    typedef std::shared_ptr<SCellMask_<2> > spSCellMask;
    typedef std::shared_ptr<SGrid_<2> > spSGrid;
    typedef std::shared_ptr<SGhost_<2> > spSGhost;
    typedef std::shared_ptr<SGhostMask_<2> > spSGhostMask;

    Point_<Vt, 2> pmin(-0.5, -0.5, -0.5);
    //  Point_<Vt, 2> pmax(1, 1, 1);
    spSGrid spsg(new SGridUniform_<2>(pmin, 40, 1, 2));

    typedef SCreatGhostByFunction_<Vt, 2> CreatGhost;
    CreatGhost cg;
    CreatGhost::FunXYZT_Value fun = [](Vt x, Vt y, Vt z, Vt time) {
        auto r = std::sqrt(x * x + y * y);
        auto theta = std::asin(y / r);
        return (0.3 + 0.15 * std::cos(6 * theta)) - r;
    };
    St bid = 10;
    auto spgm = cg.ghost_mask(spsg, fun, 0.0, 0.0, bid);
    typename Domain::spOrder sporder(new SOrderXYZ_<2>(spsg, spgm));

    // Create equation
    typedef Poisson_<2, Domain>  Poisson;

    Poisson equ(spsg, spgm, sporder);
    // Set source
    Poisson::FunXYZT_Value fun_source = [](Vt x, Vt y, Vt z, Vt time){
        auto r = std::sqrt(x * x + y * y);
        auto theta = std::asin(y / r);
        return 7 * r * r * std::cos(3 * theta);
    };
    equ.set_source(fun_source);
    // Boundary condition
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef std::shared_ptr<BoundaryCondition> spBC;
    spBI spbi(new BoundaryIndex());
    typename BoundaryConditionFunXYZ::FunXYZ_Value fun_bc = [](Vt x, Vt y, Vt z){
        auto r = std::sqrt(x * x + y * y);
        auto theta = std::asin(y / r);
        return r * r * r * r * std::cos(3 * theta);
    };
    spBC spbc(new BoundaryConditionFunXYZ(BC::_BC1_, fun_bc));
    spbi->insert(bid, spbc);
    equ.set_boundary_index_phi(spbi);


    // Set solver
    equ.set_solver("Jacobi", 100, 1e-4);

    // Add events
    typedef Event_<DIM, Domain> Event;
    typedef std::shared_ptr<Event_<DIM, Domain> > spEvent;
    spEvent spetime(
            new EventOutputTime_<DIM, Domain>(std::cout, -1, -1, 1,
                    Event::START | Event::END));
    equ.add_event("OutputTime", spetime);

    typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
    EventGnuplotField egs("phi", -1, -1, 1, Event::END);
    egs.gnuplot().set_xrange(-0.5, 0.5);
    egs.gnuplot().set_yrange(-0.5, 0.5);
    egs.gnuplot().set_equal_aspect_ratio();
    egs.set_path("./fig/");
    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

    equ.run();
}