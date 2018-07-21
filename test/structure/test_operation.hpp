#ifndef STRUCTURE_TEST_OPERATION_HPP_
#define STRUCTURE_TEST_OPERATION_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/plotly_actor.hpp"
#include "domain/structure/operation/operation.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {

TEST(scalar, initial){
	typedef SIndex_<2> Index;
	typedef SScalar_<2> Scalar;
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

	Scalar sc(spsg,spgh, sporder);

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

TEST(scalar, value){
	typedef SIndex_<2> Index;
	typedef SScalar_<2> Scalar;
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

	Scalar sc(spsg,spgh, sporder);
	sc.assign(1);

	BoundaryIndex bi;

	Index index(6,4);

	std::cout << "index = " << index
			  << " --> ghost   : " << sc.ghost().is_ghost(index) << std::endl;
	std::cout << "        " << index
			  << "               "
			  << Value(sc, bi, {4, 4}, index, _X_ , _P_) <<std::endl;
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






}
#endif
