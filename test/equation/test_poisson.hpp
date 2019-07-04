#ifndef EQUATION_TEST_POISSON_HPP_
#define EQUATION_TEST_POISSON_HPP_

#include "equation/equation.hpp"
#include "domain/structure/structure.hpp"
#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "gtest/gtest.h"

namespace carpio {
TEST(equation, DISABLED_initial){
	const St DIM = 2;
	typedef StructureDomain_<DIM> Domain;
	typename Domain::spGrid spgrid(
			new SGridUniform_<DIM>(
					{ 0.0, 0.0 }, // min point
			        { 20,  20 },  // num on each direction
			          0.05,       // cell size
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
	spbi->insert(0, spbcym);
	spbi->insert(1, spbcym);
	spbi->insert(2, spbcym);
	spbi->insert(3, spbcxm);
	equ.set_boundary_index_phi(spbi);

	// Set solver
	equ.set_solver("Jacobi", 100, 1e-4);

	// Add events
	typedef Event_<DIM, Domain> Event;
	typedef std::shared_ptr<Event_<DIM, Domain> >  spEvent;
	spEvent spetime(new EventOutputTime_<DIM, Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::END));
	equ.add_event("OutputTime", spetime);

	typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
	EventGnuplotField egs("phi", -1, -1, 1, Event::END);
	egs.gnuplot().set_xrange(-0.1, 1.1);
	egs.gnuplot().set_yrange(-0.1, 1.1);
	egs.gnuplot().set_equal_aspect_ratio();
	egs.set_path("./plot/");
	equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

	equ.run();

}

TEST(equation, explicit_run){
	const St DIM = 2;
	typedef StructureDomain_<DIM> Domain;
	typename Domain::spGrid spgrid(
			new SGridUniform_<DIM>(
					{ 0.0, 0.0 }, // min point
			        { 20,  20 },  // num on each direction
			          0.05,       // cell size
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
	spbi->insert(0, spbcym);
	spbi->insert(1, spbcym);
	spbi->insert(2, spbcym);
	spbi->insert(3, spbcxm);
	equ.set_boundary_index_phi(spbi);

	// Set time term
	equ.set_time_term(100, 1e-3);

	// Set solver
	equ.set_solver("CG", 200, 1e-4);

	// Add events
	typedef Event_<DIM, Domain> Event;
	typedef std::shared_ptr<Event_<DIM, Domain> >  spEvent;
	spEvent spetime(new EventOutputTime_<DIM, Domain>(std::cout,
		                                          -1, -1, 1, Event::AFTER | Event::END));
	equ.add_event("OutputTime", spetime);

	typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
	EventGnuplotField egs("phi", -1, -1, 1, Event::AFTER);
	egs.gnuplot().set_xrange(-0.1, 1.1);
	egs.gnuplot().set_yrange(-0.1, 1.1);
	egs.gnuplot().set_equal_aspect_ratio();
	egs.gnuplot().set_cbrange(0.0, 1.0);
	egs.set_path("./plot/");
	equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

	equ.run();

}

}

#endif
