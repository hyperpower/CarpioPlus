#ifndef EQUATION_TEST_CONVECTION_HPP_
#define EQUATION_TEST_CONVECTION_HPP_

#include "equation/advection/convection.hpp"
#include "domain/structure/structure.hpp"
#include "gtest/gtest.h"

namespace carpio {

TEST(convection, initial){
	typedef StructureDomain_<2> Domain;
	typename Domain::spGrid spgrid(
			new SGridUniform_<2>({0.0,0.0},  // min point
					             {10,  10},  // num on each direction
								  0.5,       // cell size
								  2));       // ghost layer
	typename Domain::spGhost spghost(
			new SGhostRegular_<2>(spgrid));

	// Define the equation
	Convection_<2, Domain> equ(spgrid, spghost);
}



}



#endif
