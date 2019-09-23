#ifndef EQUATION_TEST_POISSON_HPP_
#define EQUATION_TEST_POISSON_HPP_

#include "equation/equation.hpp"
#include "domain/structure/structure.hpp"
#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "gtest/gtest.h"

namespace carpio {
TEST(equation, DISABLED_initial){
	const St DIM = 2;
	typedef StructureDomain_<DIM> Domain;
	typedef typename Domain::GnuplotActor GA;
	typename Domain::spGrid spgrid(
			new SGridUniform_<DIM>(
					{ 0.0, 0.0 }, // min point
			        { 20,  20 },  // num on each direction
			          0.05,       // cell size
			          2));        // ghost layer
	typename Domain::spGhost spghost(new SGhostRegular_<DIM>(spgrid));
	typename Domain::spOrder sporder(new SOrderXYZ_<DIM>(spgrid, spghost));

	// Define the equation
	Laplace_<DIM, Domain> equ(spgrid, spghost, sporder);

	// Set boundary condition
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryCondition BC;
	typedef std::shared_ptr<BoundaryCondition> spBC;
	spBI spbi(new BoundaryIndex());
	spBC spbcxm(new BoundaryConditionValue(BC::_BC1_, 1.0));
	spBC spbcym(new BoundaryConditionValue(BC::_BC1_, 0.0));
	spbi->insert(0, spbcym);
	spbi->insert(1, spbcym);
	spbi->insert(2, spbcym);
	spbi->insert(3, spbcxm);
	equ.set_boundary_index_phi(spbi);

	// Set solver
	equ.set_solver("Jacobi", 100, 1e-4);

	// Add events
	typedef Event_<DIM, Domain> Event;
	typedef std::shared_ptr<Event_<DIM, Domain> >  spEvent;
	spEvent spetime(new EventOutputTime_<DIM, Domain>(std::cout,
		                                          -1, -1, 1, Event::START | Event::END));
	equ.add_event("OutputTime", spetime);

	typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
	EventGnuplotField egs("phi", -1, -1, 1, Event::END);
	egs.gnuplot().set_xrange(-0.1, 1.1);
	egs.gnuplot().set_yrange(-0.1, 1.1);
	egs.gnuplot().set_equal_aspect_ratio();
	egs.set_path("./plot/");
	equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

	equ.run();

}

TEST(equation, DISABLED_explicit_run){
	const St DIM = 2;
	typedef StructureDomain_<DIM> Domain;
	typename Domain::spGrid spgrid(
			new SGridUniform_<DIM>(
					{ 0.0, 0.0 }, // min point
			        { 20,  20 },  // num on each direction
			          0.05,       // cell size
			          2));        // ghost layer
	typename Domain::spGhost spghost(new SGhostRegular_<DIM>(spgrid));
	typename Domain::spOrder sporder(new SOrderXYZ_<DIM>(spgrid, spghost));

	// Define the equation
	Laplace_<DIM, Domain> equ(spgrid, spghost, sporder);

	// Set boundary condition
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryCondition BC;
	typedef std::shared_ptr<BoundaryCondition> spBC;
	spBI spbi(new BoundaryIndex());
	spBC spbcxm(new BoundaryConditionValue(BC::_BC1_, 1.0));
	spBC spbcym(new BoundaryConditionValue(BC::_BC1_, 0.0));
	spbi->insert(0, spbcym);
	spbi->insert(1, spbcym);
	spbi->insert(2, spbcym);
	spbi->insert(3, spbcxm);
	equ.set_boundary_index_phi(spbi);

	// Set time term
	equ.set_time_term(10, 1e-3);

	// Set solver
	equ.set_time_scheme("CNgeneral", 0.9);
	equ.set_solver("CG", 200, 1e-4);

	// Add events
	typedef Event_<DIM, Domain>                     Event;
	typedef std::shared_ptr<Event_<DIM, Domain> > spEvent;
	spEvent spetime(
			new EventOutputTime_<DIM, Domain>(
					std::cout, -1, -1, 1, Event::AFTER | Event::END));
	equ.add_event("OutputTime", spetime);

	typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
	EventGnuplotField::FunPlot fun = [](
			               Gnuplot& gnu,
			         const EventGnuplotField::Field& f,
					 St step, Vt t, int fob,
					 EventGnuplotField::pEqu pd){
		typedef typename Domain::GnuplotActor GA;
		gnu.add(GA::Contour(f));
		gnu.plot();
		gnu.clear();
		return 1;
	};
	EventGnuplotField egs("phi", fun, -1, -1, 1, Event::AFTER);
	egs.gnuplot().set_xrange(-0.1, 1.1);
	egs.gnuplot().set_yrange(-0.1, 1.1);
	egs.gnuplot().set_equal_aspect_ratio();
	egs.gnuplot().set_cbrange(0.0, 1.0);
	egs.set_path("./plot/");
	equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

	equ.run();

}

TEST(poisson, flower_mask) {
	const St DIM = 2;
	typedef SGnuplotActor_<2> GA;
	typedef StructureDomain_<2> Domain;
	typedef SGrid_<2> SGrid;
	typedef typename SGrid::Index SIndex;
	typedef SCellMask_<2> SCellMask;
	typedef SGhostMask_<2> SGhostMask;
	typedef std::shared_ptr<SCellMask_<2> > spSCellMask;
	typedef std::shared_ptr<SGrid_<2> > spSGrid;
	typedef std::shared_ptr<SGhost_<2> > spSGhost;
	typedef std::shared_ptr<SGhostMask_<2> > spSGhostMask;

	Point_<Vt, 2> pmin(-0.5, -0.5, -0.5);
	//	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin, 40, 1, 2));

	typedef SCreatGhostByFunction_<Vt, 2> CreatGhost;
	CreatGhost cg;
	CreatGhost::FunXYZT_Value fun = [](Vt x, Vt y, Vt z, Vt time) {
		auto r = std::sqrt(x * x + y * y);
		auto theta = std::asin(y / r);
		return (0.3 + 0.15 * std::cos(6 * theta)) - r;
	};
	St bid = 10;
	auto spgm = cg.ghost_mask(spsg, fun, 0.0, 0.0, bid);
	typename Domain::spOrder sporder(new SOrderXYZ_<2>(spsg, spgm));

	// Create equation
	typedef Poisson_<2, Domain>  Poisson;

	Poisson equ(spsg, spgm, sporder);
	// Set source
	Poisson::FunXYZT_Value fun_source = [](Vt x, Vt y, Vt z, Vt time){
		auto r = std::sqrt(x * x + y * y);
		auto theta = std::asin(y / r);
		return 7 * r * r * std::cos(3 * theta);
	};
	equ.set_source(fun_source);
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
	spbi->insert(bid, spbc);
	equ.set_boundary_index_phi(spbi);


	// Set solver
	equ.set_solver("Jacobi", 100, 1e-4);

	// Add events
	typedef Event_<DIM, Domain> Event;
	typedef std::shared_ptr<Event_<DIM, Domain> > spEvent;
	spEvent spetime(
			new EventOutputTime_<DIM, Domain>(std::cout, -1, -1, 1,
					Event::START | Event::END));
	equ.add_event("OutputTime", spetime);

	typedef EventGnuplotField_<DIM, Domain> EventGnuplotField;
	EventGnuplotField egs("phi", -1, -1, 1, Event::END);
	egs.gnuplot().set_xrange(-0.5, 0.5);
	egs.gnuplot().set_yrange(-0.5, 0.5);
	egs.gnuplot().set_equal_aspect_ratio();
	egs.set_path("./plot/");
	equ.add_event("GnuplotPhi", std::make_shared<EventGnuplotField>(egs));

	equ.run();




}


}

#endif
