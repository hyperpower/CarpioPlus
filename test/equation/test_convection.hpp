#ifndef EQUATION_TEST_CONVECTION_HPP_
#define EQUATION_TEST_CONVECTION_HPP_

#include "equation/advection/convection.hpp"
#include "domain/structure/structure.hpp"
#include "gtest/gtest.h"

namespace carpio {

TEST(convection, initial){
	const St DIM = 1;
	typedef StructureDomain_<DIM> Domain;
	typename Domain::spGrid spgrid(
			new SGridUniform_<DIM>({0.0, 0.0}, // min point
					               {10,  10},  // num on each direction
								    0.5,       // cell size
								    2));       // ghost layer
	typename Domain::spGhost spghost(
			new SGhostRegular_<DIM>(spgrid));

	typename Domain::spOrder sporder(
			new SOrderXYZ_<DIM>(spgrid, spghost));

	// Define the equation
	Convection_<DIM, Domain> equ(spgrid, spghost, sporder);

	equ.set_time_term(1, 0.1);

	// Set boundary condition


	// Set initial condition

	// Run
	equ.run();
}



}



#endif
