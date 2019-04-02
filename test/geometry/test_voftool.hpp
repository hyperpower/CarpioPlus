#ifndef GEOMETRY_TEST_VOFTOOL_HPP_
#define GEOMETRY_TEST_VOFTOOL_HPP_
#include "geometry/geometry.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef GGnuplotActor_<double, 2> GA;

typedef VOFToolPL_<Vt> Tool;
typedef Line_<Vt> Line;

TEST(GEOVOF, initial){
	Line l(0.5,0.5,1);


	Gnuplot gnu;
	gnu.set_xrange(-5, 5);
	gnu.set_yrange(-5, 5);
	gnu.add(GA::Lines(l, 0, 5));
	gnu.plot();
}

}

#endif
