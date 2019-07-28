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
	typedef SGhostMask_<2>                   SGhostMask;
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

	typename SGhostMask::FunSetByXYZ fun2 = [spcm](const Vt& x, const Vt& y, const Vt& z){
		if(x * x + y * y < 0.4){
			return spcm;
		}else{
			return spSCellMask(nullptr);
		}
	};
	spgm->set_mask(fun2);
	Gnuplot gnu;
	gnu.set_xrange(-0.5, 3.5);
	gnu.set_yrange(-0.5, 3.5);
	gnu.set_equal_ratio();
	gnu.add(GA::WireFrame(*spsg));
	gnu.add(GA::WireFrame(*spgm, 3));
	auto acb     = GA::Boundary(*spgm);
	acb->style() = "with line lw 3 lc variable";
	gnu.add(acb);
//	gnu.plot();
}


TEST(structure, initial3){
	typedef SGrid_<2>                          SGrid;
	typedef typename SGrid::Index              SIndex;
	typedef SCellMask_<2>                    SCellMask;
	typedef SGhostMask_<2>                   SGhostMask;
	typedef std::shared_ptr<SCellMask_<2> >  spSCellMask;
	typedef std::shared_ptr<SGrid_<2> >      spSGrid;
	typedef std::shared_ptr<SGhost_<2> >     spSGhost;
	typedef std::shared_ptr<SGhostMask_<2> > spSGhostMask;

	Point_<Vt, 2> pmin(-0.5, -0.5, -0.5);
//	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          80,
									  1, 2 ));
	spSGhostMask spgm(new SGhostMask_<2>(spsg));
	spSCellMask  spcm(new SCellMask(6));

	typename SGhostMask::FunSetByXYZ fun2 = [spcm](const Vt& x, const Vt& y, const Vt& z){
		auto r     = std::sqrt(x * x + y * y);
		auto theta = std::asin(y / r);
		if(0.3 + 0.15 * std::cos(6 * theta) < r){
			return spcm;
		}else{
			return spSCellMask(nullptr);
		}
	};
	spgm->set_mask(fun2);
	Gnuplot gnu;
	gnu.set_xrange(-0.5, 0.5);
	gnu.set_yrange(-0.5, 0.5);
	gnu.set_equal_ratio();
	gnu.add(GA::WireFrame(*spsg));
	gnu.add(GA::WireFrame(*spgm, 3));
	auto acb     = GA::Boundary(*spgm);
	acb->style() = "with line lw 3 lc variable";
	gnu.add(acb);
	gnu.plot();
}




}

#endif
