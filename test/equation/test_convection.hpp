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
	const St DIM = 2;
	typedef StructureDomain_<DIM> Domain;
	typename Domain::spGrid spgrid(
			new SGridUniform_<DIM>({0.0, 0.0}, // min point
					               {20,  20},  // num on each direction
								    0.05,      // cell size
								    2));       // ghost layer
	typename Domain::spGhost spghost(
			new SGhostRegular_<DIM>(spgrid));
	typename Domain::spOrder sporder(
			new SOrderXYZ_<DIM>(spgrid, spghost));

	// Define the equation
	Convection_<DIM, Domain> equ(spgrid, spghost, sporder);

	equ.set_time_term(500, 0.01);

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

	typedef std::shared_ptr<EventStopNorm1Previous_<DIM, Domain> > spEventStopNorm1Previous;
	spEventStopNorm1Previous spen1(
			new EventStopNorm1Previous_<DIM, Domain>( 1e-1,
			"phi", spgrid, spghost, sporder, -1, -1, 1, Event::AFTER));
	equ.add_event("Norm1P1", spen1);

	typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
	EventGnuplotField egs("phi", -1, -1, 1, Event::AFTER);
	egs.gnuplot().set_xrange(-0.1, 1.1);
	egs.gnuplot().set_yrange(-0.1, 1.1);
	egs.gnuplot().set_palette_red_grey();
	egs.gnuplot().set_cbrange(0.0, 1.0);
	egs.gnuplot().set_equal_aspect_ratio();
	egs.set_path("./plot/");
	equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

	equ.run();

	typedef SGnuplotActor_<DIM> GA;
	Gnuplot gnu;
	gnu.set_terminal_png("./plot/norm1.png");
	gnu.set_xlabel("Step");
	gnu.set_ylabel("Norm1");
//	gnu.set_xrange(-0.1, 1.6);
//	gnu.set_yrange(-0.1, 1.2);
	auto a = GnuplotActor::XY(spen1->get_list(), 1, 2);
//	asc->command() = "using 1:2:3 title \"Value on Scalar center\" ";
//	auto asv = GA::LinesPoints(c, 2);
//	asv->command() = "using 1:2:3 title \"Valut on Vertex\" ";
	gnu.add(a);
	gnu.plot();

}



}



#endif
