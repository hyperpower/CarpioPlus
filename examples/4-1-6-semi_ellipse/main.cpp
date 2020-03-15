#include "equation/equation.hpp"
#include "domain/structure/structure.hpp"
#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/boundary_index.hpp"

#include <cmath>

using namespace carpio;

const St DIM = 2;
typedef StructureDomain_<DIM>    Domain;
typedef typename Domain::spGrid  spGrid;
typedef typename Domain::spGhost spGhost;
typedef typename Domain::spOrder spOrder;

int run_a_scheme(const std::string& scheme) {
    const St n   = 65;                           // number of cells
    const Vt CFL = 0.4;                          // CFL
    const Vt dt  = CFL / n;                      // delta time

    std::cout << "Scheme     = " << scheme  << std::endl;
    std::cout << "Cell size  = " << 1.0 / n << std::endl;
    std::cout << "n cell     = " << n       << std::endl;
    std::cout << "Delta time = " << dt      << std::endl;
    std::cout << "CFL number = " << CFL     << std::endl;
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
    typedef std::shared_ptr<BoundaryIndex>     spBI;
    typedef BoundaryCondition                    BC;
    typedef std::shared_ptr<BoundaryCondition> spBC;
    spBI spbi(new BoundaryIndex());
    BoundaryConditionFunXYZ::FunXYZ_Value funy = [](Vt x, Vt y, Vt z){
        if(y >= 0.0 && y <= 1.0 / 6.0){
            Vt s = 1.0 - (36.0 * y * y);
            return std::sqrt(s);
        }else{
            return 0.0;
        }
    };
    spBC spbcxm(new BoundaryConditionFunXYZ(BC::_BC1_, funy));
    spbi->insert(0, spbcxm);
    BoundaryConditionFunXYZ::FunXYZ_Value funx = [](Vt x, Vt y, Vt z){
        if(x >= 0.0 && x <= 1.0 / 6.0){
            Vt s = 1.0 - (36.0 * x * x);
            return std::sqrt(s);
        }else{
            return 0.0;
        }
    };
    spBC spbcym(new BoundaryConditionFunXYZ(BC::_BC1_, funx));
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
    equ.set_scheme(scheme);

    // Stop condition
    typedef std::shared_ptr<EventConditionNormPrevious_<DIM, Domain> > spEventConditionNormPrevious;
    spEventConditionNormPrevious spen1(
            new EventConditionNormPrevious_<DIM, Domain>(
                    1e-5, 1e-5, 1e-5,  // critical value
                    "phi",             // field
                    spgrid, spghost, sporder,
                    50, -1, 10, Event::AFTER));
    equ.add_event("Norm", spen1);

    // Output section
    typedef EventOutputFieldAxisAlignSection_<DIM, Domain> EventOutputFieldAxisAlignSection;
    typedef std::shared_ptr<EventOutputFieldAxisAlignSection> spEventOutputFieldAxisAlignSection;
    spEventOutputFieldAxisAlignSection speaa(
            new EventOutputFieldAxisAlignSection(
                    "phi", _X_, 0.7,
                    1, -1, 1, Event::END));
    speaa->set_path("./data/");
    speaa->set_format_string(scheme + "_section_%s_%d_%8.4e.txt");
    equ.add_event("OutputSection", speaa);

    // plot
    if (scheme == "fou"){
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
    egs.set_format_string(scheme + "_%s_%d_%8.4e.png");
    equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));
    }

    equ.run();

    OutputNormList(tfm::format("./data/%s_norm1.txt", scheme), 
                   "Norm 1", spen1->get_norm1_list());
    OutputNormList(tfm::format("./data/%s_norm2.txt", scheme), 
                   "Norm 2", spen1->get_norm2_list());
    OutputNormList(tfm::format("./data/%s_norminf.txt", scheme),
                   "Norm inf", spen1->get_norminf_list());
}

int main(int argc, char** argv) {
    std::vector<std::string> arrscheme = {
        "FOU", "Superbee", "Minmod"
    };
    for(auto& scheme : arrscheme){
        run_a_scheme(scheme);
    }
}