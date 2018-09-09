#ifndef EQUATION_TEST_CONVECTION_HPP_
#define EQUATION_TEST_CONVECTION_HPP_

#include "equation/equation.hpp"
#include "domain/structure/structure.hpp"
#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "gtest/gtest.h"

namespace carpio {

TEST(convection, initial){
	const St DIM = 1;
	typedef StructureDomain_<DIM> Domain;
	typename Domain::spGrid spgrid(
			new SGridUniform_<DIM>({0.0, 0.0}, // min point
					               {100, 10},  // num on each direction
								    0.25,       // cell size
								    2));       // ghost layer
	typename Domain::spGhost spghost(
			new SGhostRegular_<DIM>(spgrid));

	typename Domain::spOrder sporder(
			new SOrderXYZ_<DIM>(spgrid, spghost));

	// Define the equation
	Convection_<DIM, Domain> equ(spgrid, spghost, sporder);

	equ.set_time_term(4, 0.01);

	// Set boundary condition
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryCondition BC;
	spBI spbi(new BoundaryIndex());
	spbi->insert(0,std::make_shared<BC>(
			BoundaryConditionValue(BC::_BC1_, 1.0)));
	equ.set_boundary_index_phi(spbi);
	// Set initial condition
	equ.set_initial_velocity(_X_, [](Vt, Vt, Vt, Vt){return 1.0;});

	// Add events
	typedef Event_<DIM, Domain> Event;
	typedef std::shared_ptr<Event_<DIM, Domain> >  spEvent;
	spEvent spetime(new EventOutputTime_<DIM, Domain>(std::cout,
			                                          -1, -1, 1, Event::AFTER));
	equ.add_event("OutputTime", spetime);

	typedef EventOutputScalar_<DIM, Domain> EventOutputScalar;
	EventOutputScalar eos("phi", -1, -1, 1, Event::AFTER);
	eos.set_path("./plot/");
	equ.add_event("OutputPhi", std::make_shared<EventOutputScalar>(eos));

	typedef EventGnuplotScalar_<DIM, Domain> EventGnuplotScalar;
	EventGnuplotScalar egs("phi", -1, -1, 1, Event::AFTER);
	egs.gnuplot().set_yrange(-0.3, 1.3);
	egs.set_path("./plot/");
	equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotScalar>(egs));

	// Run
	equ.run();
}



}



#endif
