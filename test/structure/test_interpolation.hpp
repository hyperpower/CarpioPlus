#ifndef STRUCTURE_TEST_INTERPOLATION_HPP_
#define STRUCTURE_TEST_INTERPOLATION_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/operation/soperation.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {
typedef SIndex_<2> Index;
typedef SField_<2> Field;
typedef std::shared_ptr<SGrid_<2> > spSGrid;
typedef std::shared_ptr<SGhost_<2> > spSGhost;
typedef std::shared_ptr<SOrderXYZ_<2> > spOrder;

TEST(scalar, initial){
	typedef SInterpolationLocal_<2> Inter;

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {5, 5},
									   0.3, 2 ));
	spSGhost spgh(new SGhostRegular_<2>(spsg));

	spOrder sporder(new SOrderXYZ_<2>(spsg, spgh));

	Field sc(spsg,spgh, sporder);

	Inter inter;

	inter.on_face_exp(sc, Index(1,1), _X_, _P_);


}

}

#endif
