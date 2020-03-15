#include "equation/equation.hpp"
#include "domain/structure/structure.hpp"
#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/boundary_index.hpp"

using namespace carpio;

const St DIM = 2;
typedef StructureDomain_<DIM>    Domain;
typedef typename Domain::spGrid  spGrid;
typedef typename Domain::spGhost spGhost;
typedef typename Domain::spOrder spOrder;

int main(int argc, char** argv) {
    const St n   = 50;                           // number of cells
    const Vt CFL = 0.4;                          // CFL
    const Vt dt  = CFL / n;                      // delta time

    std::cout << "Cell size  = " << 1.0 / n << std::endl;
    std::cout << "Delta time = " << dt << std::endl;
    std::cout << "CFL number = " << CFL << std::endl;
    spGrid spgrid(
            new SGridUniform_<DIM>({0.0, 0.0},   // min point
                                   {n,  n},      // num on each direction
                                    1.0 / n,     // cell size
                                    2));         // ghost layer
    spGhost spghost(
            new SGhostRegular_<DIM>(spgrid));
    spOrder sporder(
            new SOrderXYZ_<DIM>(spgrid, spghost));

    // Define the equation
    Convection_<DIM, Domain> equ(spgrid, spghost, sporder);

    equ.set_time_term(500, dt);

    // Set boundary condition
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef std::shared_ptr<BoundaryCondition> spBC;
    spBI spbi(new BoundaryIndex());
    spBC spbcxm(new BoundaryConditionValue(BC::_BC1_, 1.0));
    spbi->insert(0, spbcxm);
    spBC spbcym(new BoundaryConditionValue(BC::_BC1_, 0.0));
    spbi->insert(2, spbcym);
    equ.set_boundary_index_phi(spbi);

    // Set initial condition
    equ.set_initial_phi([](Vt x, Vt y, Vt z, Vt t){
        return 0.0;
    });
    equ.set_initial_velocity(_X_, [](Vt, Vt, Vt, Vt){return 1.0;});
    equ.set_initial_velocity(_Y_, [](Vt, Vt, Vt, Vt){return 1.0;});

    // Add events
    typedef Event_<DIM, Domain> Event;
    typedef std::shared_ptr<Event_<DIM, Domain> >  spEvent;
    spEvent spetime(new EventOutputTime_<DIM, Domain>(std::cout,
                                                  -1, -1, 1, Event::AFTER));
    equ.add_event("OutputTime", spetime);

    // Stop condition
    typedef std::shared_ptr<EventConditionNormPrevious_<DIM, Domain> > spEventConditionNormPrevious;
    spEventConditionNormPrevious spen1(
            new EventConditionNormPrevious_<DIM, Domain>(
                    1e-5, 1e-5, 1e-5,  // critical value
                    "phi",             // field
                    spgrid, spghost, sporder,
                    50, -1, 10, Event::AFTER));
    equ.add_event("Norm", spen1);

    // plot
    typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
    typename EventGnuplotField::FunPlot plot_fun = [](
            Gnuplot& gnu,
            const EventGnuplotField::Field& f,
                  St step , Vt t, int fob,
                  EventGnuplotField::pEqu pd){
        gnu.set_xrange(-0.1, 1.1);
        gnu.set_yrange(-0.1, 1.1);
        gnu.set_palette_red_grey();
        gnu.set_xlabel("X");
        gnu.set_ylabel("Y");
        gnu.set_cblabel("phi");
        gnu.set_cbrange(0.0, 1.0);
        gnu.set_equal_aspect_ratio();
        gnu.set_label(1,tfm::format("Step = %6d", step), 0.0, 1.02);
        gnu.set_label(2,tfm::format("Time = %f", t), 0.5, 1.02);
        gnu.add(Domain::GnuplotActor::Contour(f));
        gnu.plot();
        gnu.clear();
        return 1;
    };

    EventGnuplotField egs("phi", plot_fun, -1, -1, 1, Event::AFTER);
    egs.set_path("./fig/");
    egs.set_format_string("FOU_%s_%d_%8.4e.png");
    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

    equ.run();

    OutputNormList("./data/norm1.txt", "Norm 1", spen1->get_norm1_list());
    OutputNormList("./data/norm2.txt", "Norm 2", spen1->get_norm2_list());
    OutputNormList("./data/norminf.txt", "Norm inf", spen1->get_norminf_list());
}
