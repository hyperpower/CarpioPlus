#ifndef STRUCTURE_TEST_OPERATION_HPP_
#define STRUCTURE_TEST_OPERATION_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/operation/soperation.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {

TEST(scalar, DISABLED_initial){
	typedef SIndex_<2> Index;
	typedef SField_<2> Field;
	typedef std::shared_ptr<SGrid_<2> > spSGrid;
	typedef std::shared_ptr<SGhost_<2> > spSGhost;
	typedef std::shared_ptr<SOrderXYZ_<2> > spOrder;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {5, 5},
									   0.3, 2 ));

	spSGhost spgh(new SGhostRegular_<2>(spsg));

	spOrder sporder(new SOrderXYZ_<2>(spsg, spgh));

	Field sc(spsg,spgh, sporder);

	BoundaryIndex bi;

	Index index(6,4);

	std::cout << "index = " << index
			  << " --> ghost   : " << sc.ghost().is_ghost(index) << std::endl;
	index = {4,4};
	std::cout << "index = " << index
		      << " --> ghost   : " << sc.ghost().is_ghost(index) << std::endl;
	std::cout << "index = " << index
		      << " --> boundary on x P : "
			  << sc.ghost().is_boundary(index, _X_, _P_) << std::endl;
	std::cout << "index = " << index
			  << " --> boundary on x M : "
			  << sc.ghost().is_boundary(index, _X_, _M_) << std::endl;
//	Plotly plt;
//	plt.add(PlotlyActor::WireFrame(sc.grid()));
//	plt.add(PlotlyActor::Heatmap(sc.grid(), sc));
//	plt.size(800, 800);
//	plt.plot();
}

TEST(scalar, DISABLED_value){
	typedef SIndex_<2> Index;
	typedef SField_<2> Field;
	typedef SValue_<2> Value;
	typedef std::shared_ptr<SGrid_<2> > spSGrid;
	typedef std::shared_ptr<SGhost_<2> > spSGhost;
	typedef std::shared_ptr<SOrderXYZ_<2> > spOrder;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {5, 5},
									   0.3, 2 ));

	spSGhost spgh(new SGhostRegular_<2>(spsg));

	spOrder sporder(new SOrderXYZ_<2>(spsg, spgh));

	Field sc(spsg,spgh, sporder);
	sc.assign(1);

	BoundaryIndex bi;

	Index index(6,4);

	std::cout << "index = " << index
			  << " --> ghost   : " << sc.ghost().is_ghost(index) << std::endl;
	std::cout << "        " << index
			  << "               "
			  << Value::Get(sc, bi, {4, 4}, index, _X_ , _P_) <<std::endl;
	index = {4,4};
	std::cout << "index = " << index
		      << " --> ghost   : " << sc.ghost().is_ghost(index) << std::endl;
	std::cout << "index = " << index
		      << " --> boundary on x P : "
			  << sc.ghost().is_boundary(index, _X_, _P_) << std::endl;
	std::cout << "index = " << index
			  << " --> boundary on x M : "
			  << sc.ghost().is_boundary(index, _X_, _M_) << std::endl;
//	Plotly plt;
//	plt.add(PlotlyActor::WireFrame(sc.grid()));
//	plt.add(PlotlyActor::Heatmap(sc.grid(), sc));
//	plt.size(800, 800);
//	plt.plot();
}

TEST(scalar, laplacian) {
	typedef StructureDomain_<2> Domain;
	typedef SGnuplotActor_<2> GA;
	typedef SGrid_<2>                          SGrid;
	typedef typename SGrid::Index SIndex;
	typedef SGhostMask_<2> SGhostMask;
	typedef SGhostLinearCut_<2> SGhostLinearCut;
	typedef SField_<2> Field;

	typedef std::shared_ptr<SCellMask_<2> > spSCellMask;
	typedef std::shared_ptr<SGrid_<2> > spSGrid;
	typedef std::shared_ptr<SGhost_<2> > spSGhost;
	typedef std::shared_ptr<SGhostMask_<2> > spSGhostMask;
	typedef std::shared_ptr<SGhostLinearCut_<2> > spSGhostLinearCut;
	typedef std::shared_ptr<SOrderXYZ_<2> > spOrder;

	Point_<Vt, 2> pmin(-0.5, -0.5, -0.5);
	//	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin, 20, 1, 2));
	typedef SCreatGhostByFunction_<Vt, 2> CreatGhost;
	CreatGhost cg;
	CreatGhost::FunXYZT_Value fun = [](Vt x, Vt y, Vt z, Vt time) {
		return 0.3 * sin(30 * x) -y;
	};
	auto spg = cg.ghost_linear_cut(spsg, fun, 0.0, 0.0);
	spOrder sporder(new SOrderXYZ_<2>(spsg, spg));
	std::cout << "order size " << sporder->size()<< std::endl;

	Field sc(spsg, spg, sporder);
	sc.assign(1);

	std::shared_ptr<BoundaryIndex> spbi(new BoundaryIndex());
	// define operation
	SLaplacianCut_<2> Lap(spbi);
	Lap(sc);

	Gnuplot gnu;
	gnu.set_xrange(-0.5, 0.5);
	gnu.set_yrange(-0.5, 0.5);
	gnu.set_equal_ratio();

	// exact
	ArrayListV_<Vt> x(1000);
	x.assign_forward(-0.5, 0.001);
	ArrayListV_<Vt> y(1000);
	y.assign([&x](const St& i) {
		return 0.31 * sin(30 * x[i]);
	});
	auto spa_exact = GnuplotActor::XY(x, y);
	spa_exact->style() = "with line lc 1";
	gnu.add(spa_exact);

	gnu.add(GA::WireFrame(*spsg));

	gnu.add(GA::WireFrame(*spg, 3));
	gnu.add(GA::WireFrameCutGhostSide(*spg, 3));
	gnu.add(GA::WireFrameCutNormalSide(*spg, 5));
	gnu.add(GA::WireFrameCutInterface(*spg, 4));

	//	auto acb = GA::Boundary(*spg);
	//	acb->style() = "with line lw 3 lc variable";
	//	gnu.add(acb);
	gnu.plot();
}




}
#endif
