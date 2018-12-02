#ifndef STRUCTURE_TEST_SCALAR_HPP_
#define STRUCTURE_TEST_SCALAR_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sio_file.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef SIndex_<2>  Index;

typedef SIOFile_<2> IOFile;
typedef std::shared_ptr<SGrid_<2> > spSGrid;
typedef std::shared_ptr<SGhost_<2> > spSGhost;
typedef std::shared_ptr<SOrderXYZ_<2> > spOrder;

typedef SExpression_<2> Exp;
typedef SScalar_<2> Scalar;


TEST(sexpression, initial){

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin, { 5, 5 }, 0.3, 2));

	spSGhost spgh(new SGhostRegular_<2>(spsg));

	spOrder sporder(new SOrderXYZ_<2>(spsg, spgh));

	Exp exp(spsg, spgh);
	Scalar s(spsg, spgh);
	s.assign(1.5);

	Index idx(1,1);
	std::cout << exp(idx) << std::endl;
	// negative
	std::cout << "nagative: " << std::endl;
	auto expn = -(exp);
	std::cout << expn(idx) << std::endl;
	std::cout << "-exp + 1: " << std::endl;
	expn = -(exp) + 1.0;
	std::cout << expn(idx) << std::endl;
	std::cout << "-exp - 2: " << std::endl;
	expn = -(exp) - 2.0;
	std::cout << expn(idx) << std::endl;
	std::cout << "expn + expn: " << std::endl;
	auto expnn = expn + expn;
	std::cout << expnn(idx) << std::endl;
	std::cout << "exp * 5: " << std::endl;
	expnn = expn * 5;
	std::cout << expnn(idx) << std::endl;
	std::cout << "5 * exp * s: " << std::endl;
	expnn = 5 * expn * s;
	std::cout << expnn(idx) << std::endl;


}
}

#endif
