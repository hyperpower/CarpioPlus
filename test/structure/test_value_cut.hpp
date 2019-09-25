#ifndef STRUCTURE_TEST_VALUE_CUT_HPP_
#define STRUCTURE_TEST_VALUE_CUT_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/operation/soperation.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef SGnuplotActor_<2> GA;
typedef GGnuplotActor_<Vt, 2> GeoGA;
typedef StructureDomain_<2> Domain;
typedef SGrid_<2> SGrid;
typedef typename SGrid::Index Index;
typedef SField_<2>     Field;
typedef SGhostMask_<2> SGhostMask;
typedef SGhostLinearCut_<2> SGhostLinearCut;
typedef std::shared_ptr<SCellMask_<2> > spSCellMask;
typedef std::shared_ptr<SGrid_<2> > spSGrid;
typedef std::shared_ptr<SGhost_<2> > spSGhost;
typedef std::shared_ptr<SGhostMask_<2> > spSGhostMask;
typedef std::shared_ptr<SGhostLinearCut_<2> > spSGhostLinearCut;
typedef SValueCut_<2> Value;

void gp_add_cut_cell(
		Gnuplot& gnu,
		const typename SGhostLinearCut::Cell& cm){
	gnu.add(GeoGA::Points(cm.nc(), 7));
	gnu.add(GeoGA::Points(cm.gc(), 7));
	auto pw = cm.piecewise();
//	std::cout << "pw s =" << pw.front() << std::endl;
//	std::cout << "pw e =" << pw.back()  << std::endl;
	gnu.add(GeoGA::Points(pw.front(), 7));
	gnu.add(GeoGA::Points(pw.back(), 7));
//	std::cout << "front " << cm.front() << std::endl;
	gnu.add(GeoGA::Lines(cm.front(), -0.5, 0.5, 7));
}


TEST(value_cut, initial) {
	Point_<Vt, 2> pmin(-0.5, -0.5, -0.5);
	//	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin, 20, 1, 2));
	typedef SCreatGhostByFunction_<Vt, 2> CreatGhost;
	CreatGhost cg;
	CreatGhost::FunXYZT_Value fun = [](Vt x, Vt y, Vt z, Vt time) {
		return x * x + y * y - 0.20;
	};
	auto spg = cg.ghost_linear_cut(spsg, fun, 0.0, 0.0);
	typename Domain::spOrder sporder(new SOrderXYZ_<2>(spsg, spg));
	std::cout << "Order Info ====" << std::endl;
	std::cout << "num of normal = " << sporder->size() << std::endl;
	// Field
	Field field(spsg, spg, sporder);
	// Boundary condition
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryCondition BC;
	typedef std::shared_ptr<BoundaryCondition> spBC;
	spBI spbi(new BoundaryIndex());
		typename BoundaryConditionFunXYZ::FunXYZ_Value fun_bc = [](Vt x, Vt y, Vt z){
			auto r = std::sqrt(x * x + y * y);
			auto theta = std::asin(y / r);
			return r * r * r * r * std::cos(3 * theta);
	};
	spBC spbc(new BoundaryConditionFunXYZ(BC::_BC1_, fun_bc));
	spbi->insert(10, spbc);

	Value valuetool;
	Index idxc(5,1);
	Index idxg(6,1);
	Orientation ori = _P_;
	Axes axe = _X_;
//	std::cout << "idx c  = " << idxc << std::endl;
	std::cout << "idx g  = " << idxg << std::endl;

	auto spcell  = spg->operator ()(idxg);
	if(spcell != nullptr){
//		spcell->show_aperture_ratio();
		std::cout << "normal side center = " << spcell->nc() << std::endl;
		std::cout << "ghost  side center = " << spcell->gc() << std::endl;
	}else{
		std::cout << "idx = " << idxg << " is nullptr" << std::endl;
	}
	auto exp = valuetool.get_expression(field,*spbi, idxc, idxg, ori, axe);
	std::cout << "exp : " << exp << std::endl;
	Gnuplot gnu;
	gnu.set_terminal_png("./plot/out.png");
	gnu.set_xrange(-0.5, -0.1);
	gnu.set_yrange(-0.5, -0.1);
	gnu.set_equal_ratio();
	gnu.add(GA::WireFrame(*spsg));

	gnu.add(GA::WireFrame(*spg, 3));
	gnu.add(GA::WireFrameCutGhostSide(*spg, 3));
	gnu.add(GA::WireFrameCutNormalSide(*spg, 5));
	gnu.add(GA::WireFrameCutInterface(*spg, 4));
	gnu.add(GA::WireFrameCutInterface(*spg, idxg, 6));
	gp_add_cut_cell(gnu, *spcell);
	//	auto acb = GA::Boundary(*spg);
	//	acb->style() = "with line lw 3 lc variable";
	//	gnu.add(acb);
	gnu.plot();
}
}

#endif
