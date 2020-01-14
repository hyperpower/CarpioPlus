#ifndef STRUCTURE_TEST_ORDER_HPP_
#define STRUCTURE_TEST_ORDER_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/splotly_actor.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {

TEST(structure, DISABLED_initial){
	typedef SIndex_<2> Index;
	typedef std::shared_ptr<SGrid_<2> > spSGrid;
	typedef std::shared_ptr<SGhost_<2> > spSGhost;
	typedef std::shared_ptr<SOrderXYZ_<2> > spOrder;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {5, 3},
									   0.3, 2 ));

	spSGhost spgh(new SGhostRegular_<2>(spsg));

	spOrder spo(new SOrderXYZ_<2>(spsg, spgh));

	St count = 0;
	for(auto& index : *spo){
//		std::cout << index << " order = "<< count << std::endl;
		count ++;
	}

	Index idx(2,2);
	std::cout << "index = " << idx << "  Order = " << spo->get_order(idx) << std::endl;
}

TEST(structure, plot_order){
	typedef SIndex_<2> Index;
	typedef std::shared_ptr<SGrid_<2> > spSGrid;
	typedef std::shared_ptr<SGhost_<2> > spSGhost;
	typedef std::shared_ptr<SOrder_<2> > spOrder;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {5, 5},
									   0.3, 2 ));

	spSGhost spgh(new SGhostRegular_<2>(spsg));

	spOrder spo(new SOrderXYZ_<2>(spsg, spgh, 5));

	typedef SGnuplotActor_<2> GA;
	Gnuplot gnu;
	gnu.set_equal_aspect_ratio();
	gnu.set_xrange(-0.2, 1.7);
	gnu.set_yrange(-0.2, 1.7);
	GA::OrderLabel(gnu, *spo);
	gnu.add(GA::WireFrame(*spsg));
	gnu.plot();
}

TEST(scalar, parallel_order){
	typedef SIndex_<2> Index;
	typedef std::shared_ptr<SGrid_<2> > spSGrid;
	typedef std::shared_ptr<SGhost_<2> > spSGhost;
	typedef std::shared_ptr<SOrder_<2> > spOrder;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {5, 3},
									   0.3, 2 ));

	spSGhost spgh(new SGhostRegular_<2>(spsg));

	spOrder spo(new SOrderXYZ_<2>(spsg, spgh, 4));

	std::cout << "Size order =   " << spo->size() << std::endl;
	std::cout << "Size threads = " << spo->num_threads() << std::endl;

	for(St i = 0; i < spo->num_threads(); i++){
		auto i1 = spo->begin(i);
		auto e1 = spo->end(i);
		std::cout << " Thread Num = " << i;
		std::cout << " idx bgn    = " << (*i1);
		std::cout << " idx end    = " << (*e1);
		std::cout << " Order bgn  = " << spo->get_order((*i1)) << std::endl;
//		std::cout << " Order end  = " << spo->get_order((*e1)) << std::endl;
	}

}


}

#endif
