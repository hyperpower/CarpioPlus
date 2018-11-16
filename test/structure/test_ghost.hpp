#ifndef STRUCTURE_TEST_GHOST_HPP_
#define STRUCTURE_TEST_GHOST_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/splotly_actor.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef SGnuplotActor_<2> GA;

TEST(structure, initial){
	typedef std::shared_ptr<SGrid_<2> > spSGrid;
	typedef std::shared_ptr<SGhost_<2> > spSGhost;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {15, 10},
									   0.3, 2 ));

	spSGhost spgh(new SGhostMask_<2>(spsg));



	Gnuplot gnu;
	gnu.set_xrange(-0.5, 5.5);
	gnu.set_yrange(-0.5, 5.5);
	gnu.set_equal_ratio();
	gnu.add(GA::WireFrame(*spsg));
	gnu.plot();
}

TEST(structure, initial2){
	typedef std::shared_ptr<SGrid_<2> > spSGrid;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          20,
									  3, 2 ));
//	Gnuplot gnu;
//	ply.set_xrange(-0.5, 3.5);
//	ply.set_yrange(-0.5, 3.5);
//	ply.set_equal_ratio();
//	ply.add(GnuplotActor::WireFrame(*spsg));
//	ply.plot();
}




}

#endif
