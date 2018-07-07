#ifndef STRUCTURE_TEST_GRID_HPP_
#define STRUCTURE_TEST_GRID_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {


TEST(structure, initial){
	typedef std::shared_ptr<SGrid_<2> > spSGrid;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridNonUniform_<2>(pmin, pmax, {10, 10}, 1));

	Gnuplot ply;
	ply.set_xrange(-0.5, 1.5);
	ply.set_yrange(-0.5, 1.5);
	ply.set_equal_ratio();
	ply.add(GnuplotActor::WireFrame(*spsg));
	ply.plot();

}


}

#endif
