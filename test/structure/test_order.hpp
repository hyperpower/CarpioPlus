#ifndef STRUCTURE_TEST_ORDER_HPP_
#define STRUCTURE_TEST_ORDER_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/plotly_actor.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {

TEST(structure, initial){
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
		std::cout << index << " order = "<< count << std::endl;
		count ++;
	}

	Index idx(2,2);
	std::cout << "index = " << idx << "  Order = " << spo->get_order(idx) << std::endl;

}


}

#endif
