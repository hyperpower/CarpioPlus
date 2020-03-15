#ifndef EQUATION_TEST_CONVECTION_HPP_
#define EQUATION_TEST_CONVECTION_HPP_

#include "equation/equation.hpp"
#include "domain/structure/structure.hpp"
#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "gtest/gtest.h"

namespace carpio {



TEST(convection, DISABLED_initial){
    const St DIM = 1;
	typedef StructureDomain_<DIM> Domain;
	typename Domain::spGrid spgrid(
			new SGridUniform_<DIM>({0.0, 0.0}, // min point
					               {100, 10},  // num on each direction
								    0.02,      // cell size
								    2));       // ghost layer
	typename Domain::spGhost spghost(
			new SGhostRegular_<DIM>(spgrid));

	typename Domain::spOrder sporder(
			new SOrderXYZ_<DIM>(spgrid, spghost));

	// Define the equation
	Convection_<DIM, Domain> equ(spgrid, spghost, sporder);

	equ.set_time_term(10, 0.001);

	// Set boundary condition
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryCondition BC;
	spBI spbi(new BoundaryIndex());
//	spbi->insert(0,std::make_shared<BC>(
//			BoundaryConditionValue(BC::_BC1_, 1.0)));
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

	// Event Normal to exact
	typename Domain::FunXYZT_Value fun_exact = [](Vt x, Vt y, Vt z, Vt t){
	    Vt xc = 0.5 + t;
	    if(x > (xc - 0.25) && x < (xc + 0.25)){
	        return 1.0;
	    }else{
	        return 0.0;
	    }
	};
	typedef EventNormExactFunction_<DIM, Domain> EventNormExactFunction;
	EventNormExactFunction enef("phi", fun_exact, -1, -1, 1, Event::AFTER);
	equ.add_event("NormToExactFunction", std::make_shared<EventNormExactFunction>(enef));

//  typedef EventOutputField_<DIM, Domain> EventOutputField;
//	EventOutputField eos("phi", -1, -1, 1, Event::AFTER);
//	eos.set_path("./plot/");
//	equ.add_event("OutputPhi", std::make_shared<EventOutputField>(eos));

	typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
	EventGnuplotField egs("phi", -1, -1, 1, Event::AFTER);
	egs.gnuplot().set_yrange(-0.3, 1.3);
	egs.set_path("./plot/");
	equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

	// Run
	equ.run();
}

TEST(convection, one_step_2){
#ifdef OPENMP
    std::cout << "DEFINE OPENMP" << std::endl;
#endif
	const St DIM = 2;                            // Dimension
	const St n   = 80;                           // number of cells
	const Vt CFL = 0.4;                          // CFL
	const Vt dt  = CFL * 80.0 / n;               // delta time
	const St steps = 360.0 / dt;

	std::cout << "Cell size  = " << 80.0 / n << std::endl;
	std::cout << "Delta time = " << dt       << std::endl;
	std::cout << "Steps      = " << steps    << std::endl;
	std::cout << "CFL number = " << CFL      << std::endl;
	typedef StructureDomain_<DIM> Domain;
	typename Domain::spGrid spgrid(
			new SGridUniform_<DIM>({0.0, 0.0},  // min point
					               {n,  n},    // num on each direction
								    80.0 / n,     // cell size
								    2));         // ghost layer
	std::shared_ptr<SGhostRegularSubdivision_<DIM> > spghost(new SGhostRegularSubdivision_<DIM>(spgrid));
	typename SGhostRegularSubdivision_<DIM>::FunBoundaryID funbid = [](int bid, Vt x, Vt y, Vt z){
	    if(x < 0.0){
	        return 21; // inlet boundary
	    }else{
	        return 22; // outlet boundary
	    }
	};
	spghost->set_boundary_id_function(2, funbid);

	typename Domain::spOrder sporder(
			new SOrderXYZ_<DIM>(spgrid, spghost, 16));

	// Define the equation
	Convection_<DIM, Domain> equ(spgrid, spghost, sporder);

	equ.set_time_term(steps, dt);
//	equ.set_scheme("QUICK");

	// Set boundary condition
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryCondition BC;
	typedef std::shared_ptr<BoundaryCondition> spBC;
	spBI spbi(new BoundaryIndex());
	BoundaryConditionFunXYZ::FunXYZ_Value fun = [](Vt x, Vt y, Vt z){
	    if(x >= -0.8 && x <= -0.6){
	        return 1.0;
	    }else{
	        return 0.0;
	    }
	};
	spBC spbcxm(new BoundaryConditionFunXYZ(BC::_BC1_, fun));
//	spbi->insert(21, spbcxm);

//	spBC spbc0(new BoundaryConditionValue(BC::_BC1_, 0.0));
//	spbi->insert(0, spbc0);
//	spbi->insert(1, spbc0);
//	spbi->insert(3, spbc0);
	equ.set_boundary_index_phi(spbi);

	spBI spbi_u(new BoundaryIndex());
	BoundaryConditionFunXYZ::FunXYZ_Value fun_u = [](Vt x, Vt y, Vt z){
        return -(y - 40.0)*2*3.1415926/360.0;
	};
	spBC spbc_u(new BoundaryConditionFunXYZ(BC::_BC1_, fun_u));
	spbi_u->insert(0,  spbc_u);
	spbi_u->insert(1,  spbc_u);
	spbi_u->insert(21, spbc_u);
	spbi_u->insert(22, spbc_u);
	spbi_u->insert(3,  spbc_u);
	spbi_u->insert(4,  spbc_u);

	spBI spbi_v(new BoundaryIndex());
	BoundaryConditionFunXYZ::FunXYZ_Value fun_v = [](Vt x, Vt y, Vt z){
        return (x-40)*2*3.1415926/360.0;
	};
	spBC spbc_v(new BoundaryConditionFunXYZ(BC::_BC1_, fun_v));
	spbi_v->insert(0,  spbc_v);
	spbi_v->insert(1,  spbc_v);
	spbi_v->insert(21, spbc_v);
	spbi_v->insert(22, spbc_v);
	spbi_v->insert(3,  spbc_v);
	spbi_v->insert(4,  spbc_v);

	equ.set_boundary_index_velocity(_X_, spbi_u);
	equ.set_boundary_index_velocity(_Y_, spbi_v);

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
	equ.set_initial_velocity(_X_, [fun_u](Vt x, Vt y, Vt z, Vt t){
	    return fun_u(x, y, z);
	});
	equ.set_initial_velocity(_Y_, [fun_v](Vt x, Vt y, Vt z, Vt t){
	    return fun_v(x, y, z);
	});

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
			        1e-1, 1e-1, 1e-1,  // critical value
			        "phi",             // field
			        spgrid, spghost, sporder,
			        1, -1, 10, Event::AFTER));
	equ.add_event("Norm1P1", spen1);

	// Output Field
    typedef EventOutputField_<DIM, Domain> EventOutputField;
    EventOutputField eos("phi", -1, -1, 1, Event::AFTER);
    eos.set_path("./data/");
    eos.set_format_string("FOU_%s_%d_%8.4e.txt");
//    equ.add_event("OutputPhi", std::make_shared<EventOutputField>(eos));


    // Output section
	typedef EventOutputFieldAxisAlignSection_<DIM, Domain> EventOutputFieldAxisAlignSection;
	typedef std::shared_ptr<EventOutputFieldAxisAlignSection> spEventOutputFieldAxisAlignSection;
	spEventOutputFieldAxisAlignSection speaa(
	        new EventOutputFieldAxisAlignSection(
	                "phi", _X_, 0.6,
	                1, -1, 1, Event::END));
	equ.add_event("OutputSection", speaa);

	typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
	typename EventGnuplotField::FunPlot plot_fun = [](
	        Gnuplot& gnu,
	        const EventGnuplotField::Field& f,
	              St step , Vt t, int fob,
	              EventGnuplotField::pEqu pd){
//        gnu.set_xrange(-1.1, 1.1);
//        gnu.set_yrange(-0.1, 1.1);
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

	EventGnuplotField egs("phi", plot_fun, -1, -1, 1, Event::AFTER | Event::END);
    egs.set_path("./plot/");
    egs.set_format_string("Upwind1_%s_%d_%8.4e.png");
	equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

	equ.run();

	OutputNormList("./norm1.txt", "Norm 1", spen1->get_norm1_list());
	OutputNormList("./norm2.txt", "Norm 2", spen1->get_norm2_list());
	OutputNormList("./norminf.txt", "Norm inf", spen1->get_norminf_list());
}



}



#endif
