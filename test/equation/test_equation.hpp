#ifndef EQUATION_TEST_EQUATION_HPP_
#define EQUATION_TEST_EQUATION_HPP_

#include "equation/equation.hpp"
#include "domain/structure/structure.hpp"
#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "gtest/gtest.h"

namespace carpio {
TEST(event_manager, initial){
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
	typedef Equation_<DIM, Domain> Equation;
	Equation equ(spgrid, spghost, sporder);
	EventManager_<DIM, Domain> em;
	Equation::spEvent spe(new Equation::Event(-1, -1, -1, Equation::Event::BEFORE));
	Equation::spEvent spe2(new Equation::Event(-1, -1, -1, Equation::Event::START));
	em.add("aaa", spe);
	em.add("bbb", spe2);
	std::cout << "Has event" << em.has("aaa") << std::endl;
	em.show();
}


TEST(equation, initial){
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
	Equation_<DIM, Domain> equ(spgrid, spghost, sporder);

	equ.set_time_term(10, 0.01);

	equ.run();
}
}

#endif
