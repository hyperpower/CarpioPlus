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
    const St n     = 80;                           // number of cells
    const Vt CFL   = 0.4;                          // CFL
    const Vt dt    = CFL * 80.0 / n;               // delta time
    const St steps = 360.0 / dt;

    std::cout << "Scheme     = " << scheme  << std::endl;
    std::cout << "Cell size  = " << 80.0 / n << std::endl;
    std::cout << "n cell     = " << n       << std::endl;
    std::cout << "Delta time = " << dt      << std::endl;
    std::cout << "CFL number = " << CFL     << std::endl;
    spGrid spgrid(
            new SGridUniform_<DIM>({0.0, 0.0},   // min point
                                   {n,  n},     // num on each direction
                                    80.0 / n,      // cell size
                                    2));          // ghost layer
    std::shared_ptr<SGhostRegular_<DIM> > spghost(new SGhostRegular_<DIM>(spgrid));

    spOrder sporder(
            new SOrderXYZ_<DIM>(spgrid, spghost, 16));

    // Define the equation
    Convection_<DIM, Domain> equ(spgrid, spghost, sporder);

    equ.set_time_term(steps, dt);
    equ.set_scheme(scheme);

    // Set boundary condition
    typedef std::shared_ptr<BoundaryIndex> spBI;
    spBI spbi(new BoundaryIndex());
    equ.set_boundary_index_phi(spbi);

    // Set initial condition
    equ.set_initial_phi([](Vt x, Vt y, Vt z, Vt t){
        Vt xc = 20;
        Vt yc = 40;
        Vt l  = 8;
        if(   IsInRange(xc-l, x, xc+l, _cc_) 
           && IsInRange(yc-l, y, yc+l, _cc_)){
            return 1.0;
        }else{
            return 0.0;
        }
    });
    equ.set_initial_velocity(_X_, [](Vt x, Vt y, Vt z, Vt t){
        return -(y - 40.0)*2*3.1415926/360.0;
    });
    equ.set_initial_velocity(_Y_, [](Vt x, Vt y, Vt z, Vt t){
        return (x-40)*2*3.1415926/360.0;
    });

    // Add events
    typedef Event_<DIM, Domain> Event;
    typedef std::shared_ptr<Event_<DIM, Domain> >  spEvent;
    spEvent spetime(new EventOutputTime_<DIM, Domain>(std::cout,
                                                  -1, -1, 1, Event::AFTER));
    equ.add_event("OutputTime", spetime);

    // Output section
    typedef EventOutputFieldAxisAlignSection_<DIM, Domain> EventOutputFieldAxisAlignSection;
    typedef std::shared_ptr<EventOutputFieldAxisAlignSection> spEventOutputFieldAxisAlignSection;
    spEventOutputFieldAxisAlignSection speaay(
            new EventOutputFieldAxisAlignSection(
                    "phi", _Y_, 40,
                    1, -1, 1, Event::END));
    speaay->set_path("./data/");
    speaay->set_format_string(scheme + "_y_%s_%d_%8.4e.txt");
    equ.add_event("OutputSectionY", speaay);

    spEventOutputFieldAxisAlignSection speaax(
            new EventOutputFieldAxisAlignSection(
                    "phi", _X_, 20,
                    1, -1, 1, Event::END));
    speaax->set_path("./data/");
    speaax->set_format_string(scheme + "_x_%s_%d_%8.4e.txt");
    equ.add_event("OutputSectionX", speaax);

    // plot
    if (scheme == "FOU"){
        typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
        typename EventGnuplotField::FunPlot plot_fun = [](
                Gnuplot& gnu,
                const EventGnuplotField::Field& f,
                      St step , Vt t, int fob,
                      EventGnuplotField::pEqu pd){
            gnu.set_xrange(0.0, 80.0);
            gnu.set_yrange(0.0, 80.0);
            gnu.set_zrange(0.0, 1.0);
            gnu.set_palette_blue_red();
            gnu.set_xlabel("X");
            gnu.set_ylabel("Y");
            gnu.set_cblabel("phi");
            gnu.set_cbrange(0.0, 1.0);
            gnu.set_equal_aspect_ratio();
            gnu.set_ticslevel();
            gnu.set_view(20,25,1.1,1.0);
    //        gnu.set_label(1,tfm::format("Step = %6d", step), 0.0, 1.02);
    //        gnu.set_label(2,tfm::format("Time = %f", t), 0.5, 1.02);
            gnu.add(Domain::GnuplotActor::ContourWire(f));
            gnu.splot();
            gnu.clear();
            return 1;
        };

        EventGnuplotField egs("phi", plot_fun, -1, -1, 10, Event::AFTER);
        egs.set_path("./fig/");
        egs.set_format_string(scheme + "_%s_%d_%8.4e.png");
        equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));
    }

    equ.run();

    // OutputNormList(tfm::format("./data/%s_norm1.txt", scheme), 
    //                "Norm 1", spen1->get_norm1_list());
    // OutputNormList(tfm::format("./data/%s_norm2.txt", scheme), 
    //                "Norm 2", spen1->get_norm2_list());
    // OutputNormList(tfm::format("./data/%s_norminf.txt", scheme),
    //                "Norm inf", spen1->get_norminf_list());
}

int main(int argc, char** argv) {
    std::vector<std::string> arrscheme = {
        "FOU", "Superbee", "Minmod"
    };
    for(auto& scheme : arrscheme){
        run_a_scheme(scheme);
    }
    #ifdef OPENMP
    std::cout << "Define OpenMP" << std::endl;
    #endif
}