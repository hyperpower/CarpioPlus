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
//	gnu.plot();
}

TEST(structure, initial2){
	typedef SGrid_<2>                          SGrid;
	typedef typename SGrid::Index              SIndex;
	typedef SCellMask_<2>                    SCellMask;
	typedef std::shared_ptr<SCellMask_<2> >  spSCellMask;
	typedef std::shared_ptr<SGrid_<2> >      spSGrid;
	typedef std::shared_ptr<SGhost_<2> >     spSGhost;
	typedef std::shared_ptr<SGhostMask_<2> > spSGhostMask;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          80,
									  3, 2 ));
	spSGhostMask spgm(new SGhostMask_<2>(spsg));
	spSCellMask  spcm(new SCellMask(6));
	typename SGrid::FunIndex fun = [spgm, spsg, spcm](const SIndex& index){
		auto& gm = *spgm;
		auto& g  = *spsg;
		auto cp  = g.c(index);
		auto x = cp.x();
		auto y = cp.y();
		if(x * x + y* y < 1){
			gm(index) = spcm;
		}
	};
	spsg->for_each(fun);
	Gnuplot gnu;
	gnu.set_xrange(-0.5, 3.5);
	gnu.set_yrange(-0.5, 3.5);
	gnu.set_equal_ratio();
	gnu.add(GA::WireFrame(*spsg));
	gnu.add(GA::WireFrame(*spgm, 3));
	auto acb = GA::Boundary(*spgm);
	acb->style() = "with line lw 3 lc variable";
	gnu.add(acb);
	gnu.plot();
}




}

#endif
