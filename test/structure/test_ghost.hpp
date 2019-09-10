#ifndef STRUCTURE_TEST_GHOST_HPP_
#define STRUCTURE_TEST_GHOST_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/splotly_actor.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef SGnuplotActor_<2> GA;

TEST(structure, DISABLED_initial){
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

TEST(structure, DISABLED_initial2){
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


TEST(structure, DISABLED_initial3){
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
			                          40,
									  1, 2 ));

	typedef SCreatGhostByFunction_<Vt, 2> CreatGhost;
	CreatGhost cg;
	CreatGhost::FunXYZT_Value fun = [](Vt x, Vt y, Vt z, Vt time){
		auto r     = std::sqrt(x * x + y * y);
		auto theta = std::asin(y / r);
		return 0.3 + 0.15 * std::cos(6 * theta) - r;
	};

	auto spgm = cg.ghost_mask(spsg, fun, 0.0, 0.0);

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 0.5);
	gnu.set_yrange(-0.5, 0.5);
	gnu.set_equal_ratio();
	gnu.add(GA::WireFrame(*spsg));
	gnu.add(GA::WireFrame(*spgm, 3));
	auto acb     = GA::Boundary(*spgm);
	acb->style() = "with line lw 3 lc variable";
	gnu.add(acb);
//	gnu.plot();
}

TEST(structure, initial_cut){
	typedef SGrid_<2>                          SGrid;
	typedef typename SGrid::Index              SIndex;
	typedef SGhostMask_<2>                   SGhostMask;
	typedef SGhostLinearCut_<2>              SGhostLinearCut;

	typedef std::shared_ptr<SCellMask_<2> >  spSCellMask;
	typedef std::shared_ptr<SGrid_<2> >      spSGrid;
	typedef std::shared_ptr<SGhost_<2> >     spSGhost;
	typedef std::shared_ptr<SGhostMask_<2> > spSGhostMask;
	typedef std::shared_ptr<SGhostLinearCut_<2> > spSGhostLinearCut;

	Point_<Vt, 2> pmin(-0.5, -0.5, -0.5);
//	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          20,
									  1, 2 ));
	typedef SCreatGhostByFunction_<Vt, 2> CreatGhost;
	CreatGhost cg;
	CreatGhost::FunXYZT_Value fun = [](Vt x, Vt y, Vt z, Vt time){
		return x * x + y * y - 0.20;
	};
	auto spg = cg.ghost_linear_cut(spsg, fun, 0.0, 0.0);

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 0.5);
	gnu.set_yrange(-0.5, 0.5);
	gnu.set_equal_ratio();
	gnu.add(GA::WireFrame(*spsg));

	gnu.add(GA::WireFrame(*spg, 3));
//	auto acb = GA::Boundary(*spg);
//	acb->style() = "with line lw 3 lc variable";
//	gnu.add(acb);
//	gnu.plot();

}

TEST(structure, initial_cut_trivial){
	typedef SGrid_<2>                          SGrid;
	typedef typename SGrid::Index              SIndex;
	typedef SGhostMask_<2>                   SGhostMask;
	typedef SGhostLinearCut_<2>              SGhostLinearCut;

	typedef std::shared_ptr<SCellMask_<2> >  spSCellMask;
	typedef std::shared_ptr<SGrid_<2> >      spSGrid;
	typedef std::shared_ptr<SGhost_<2> >     spSGhost;
	typedef std::shared_ptr<SGhostMask_<2> > spSGhostMask;
	typedef std::shared_ptr<SGhostLinearCut_<2> > spSGhostLinearCut;

	Point_<Vt, 2> pmin(-0.5, -0.5, -0.5);
//	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          20,
									  1, 2 ));
	typedef SCreatGhostByFunction_<Vt, 2> CreatGhost;
	CreatGhost cg;
	CreatGhost::FunXYZT_Value fun = [](Vt x, Vt y, Vt z, Vt time){
		return  -x + y + 0.2;
	};
	auto spg = cg.ghost_linear_cut(spsg, fun, 0.0, 0.0);

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 0.5);
	gnu.set_yrange(-0.5, 0.5);
	gnu.set_equal_ratio();
	gnu.add(GA::WireFrame(*spsg));

	gnu.add(GA::WireFrame(*spg, 3));
//	auto acb = GA::Boundary(*spg);
//	acb->style() = "with line lw 3 lc variable";
//	gnu.add(acb);
	gnu.plot();

}



}

#endif
