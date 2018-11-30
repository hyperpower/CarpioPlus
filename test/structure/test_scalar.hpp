#ifndef STRUCTURE_TEST_SCALAR_HPP_
#define STRUCTURE_TEST_SCALAR_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sio_file.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef SIndex_<2> Index;
typedef SScalar_<2> Scalar;
typedef SIOFile_<2> IOFile;
typedef std::shared_ptr<SGrid_<2> > spSGrid;
typedef std::shared_ptr<SGhost_<2> > spSGhost;
typedef std::shared_ptr<SOrderXYZ_<2> > spOrder;


TEST(scalar, initial){


	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {5, 5},
									   0.3, 2 ));

	spSGhost spgh(new SGhostRegular_<2>(spsg));

	spOrder sporder(new SOrderXYZ_<2>(spsg, spgh));

	Scalar sc(spsg,spgh, sporder);

	Plotly plt;
	plt.add(PlotlyActor::WireFrame(sc.grid()));
	plt.add(PlotlyActor::Heatmap(sc.grid(), sc));
	plt.size(800, 800);
	plt.plot();
}

TEST(scalar, outputfile) {


	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin, { 5, 5 }, 0.3, 2));

	spSGhost spgh(new SGhostRegular_<2>(spsg));

	spOrder sporder(new SOrderXYZ_<2>(spsg, spgh));

	Scalar sc(spsg, spgh, sporder);

	IOFile::OutputScalar("s.txt", sc);
}


TEST(scalar, add){

	Point_<Vt, 2> pmin(0, 0, 0);
	Point_<Vt, 2> pmax(1, 1, 1);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {3, 3},
									   0.3, 2 ));

	spSGhost spgh(new SGhostRegular_<2>(spsg));

	spOrder sporder(new SOrderXYZ_<2>(spsg, spgh));

	Scalar sc(spsg,spgh, sporder);
	sc.assign(2.0);

	Scalar sc2(spsg,spgh, sporder);
	sc2.assign(3.0);

	sc += sc2;
	std::cout << sc << std::endl;
	ASSERT_EQ(sc(0,0), 5.0);
	sc += 1.5;
	ASSERT_EQ(sc(0,0), 6.5);
	std::cout << sc << std::endl;

	sc -= sc2;
	ASSERT_EQ(sc(0,0), 3.5);
	std::cout << sc << std::endl;
	sc -= 1.3;
	ASSERT_EQ(sc(0,0), 2.2);
	std::cout << sc << std::endl;

	sc *= sc2;
	ASSERT_LE(std::abs(sc(0,0)- 6.60), 1e-4);
	std::cout << sc << std::endl;
	sc *= 2.0;
	ASSERT_LE(std::abs(sc(0,0)- 13.2), 1e-4);
	std::cout << sc << std::endl;

	sc /= sc2;
	std::cout << "sc /= sc2;\n";
	ASSERT_LE(std::abs(sc(0, 0) - 4.40), 1e-4);
	std::cout << sc << std::endl;
	sc /= 2.0;
	std::cout << "sc /= 2.0;\n";
	ASSERT_LE(std::abs(sc(0, 0) - 2.20), 1e-4);
	std::cout << sc << std::endl;

	auto sc3 = sc + sc2;
	std::cout << "sc3 = sc + sc2\n";
	sc3 = sc + 2.6;
	std::cout << "sc3 = sc + 2.6\n";
	sc3 = 2.7 + sc + 2.6;
	std::cout << "sc3 = 2.7 + sc + 2.6\n";

	sc3 = sc - 2.6;
	std::cout << "sc3 = sc - 2.6\n";
	std::cout << "      "<< sc(0,0) << " - 2.6" << std::endl;
	std::cout << "sc3 = "<< sc3(0,0) << std::endl;
	sc3 = 2.7 - sc - 2.6;
	std::cout << "sc3 = 2.7 - sc - 2.6\n";
	std::cout << "      2.7 - " << sc(0, 0) << " - 2.6" << std::endl;
	std::cout << "sc3 = " << sc3(0, 0) << std::endl;
	std::cout << "--------------\n";
	sc3 = sc * 2.6;
	std::cout << "sc3 = sc * 2.6\n";
	std::cout << "      " << sc(0, 0) << " * 2.6" << std::endl;
	std::cout << "sc3 = " << sc3(0, 0) << std::endl;
	ASSERT_LE(std::abs(sc3(0, 0) - (sc(0, 0) * 2.6)), 1e-4);
	std::cout << "--------------\n";
	sc3 = 2.7 * sc * 2.6;
	std::cout << "sc3 = 2.7 * sc * 2.6\n";
	std::cout << "      2.7 * " << sc(0, 0) << " * 2.6" << std::endl;
	std::cout << "sc3 = " << sc3(0, 0) << std::endl;
	ASSERT_LE(std::abs(sc3(0, 0) - (2.7 * sc(0, 0) * 2.6)), 1e-4);
	std::cout << "--------------\n";
	sc3 = sc / 2.6;
	std::cout << "sc3 = sc / 2.6\n";
	std::cout << "      " << sc(0, 0) << " / 2.6" << std::endl;
	std::cout << "sc3 = " << sc3(0, 0) << std::endl;
	ASSERT_LE(std::abs(sc3(0, 0) - (sc(0, 0) / 2.6)), 1e-4);
	sc3 = 2.3 / sc;
	std::cout << "sc3 = 2.3 / sc\n";
	std::cout << "      2.3 / " << sc(0, 0) << std::endl;
	std::cout << "sc3 = " << sc3(0, 0) << std::endl;
	ASSERT_LE(std::abs(sc3(0, 0) - (2.3 / sc(0, 0))), 1e-4);
}




}

#endif
