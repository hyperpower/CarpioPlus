#ifndef STRUCTURE_TEST_VOF_HPP_
#define STRUCTURE_TEST_VOF_HPP_

#include "domain/structure/structure.hpp"
#include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sio_file.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "geometry/geometry.hpp"
#include "geometry/io/ggnuplot_actor.hpp"
#include "gtest/gtest.h"

namespace carpio {

typedef SIndex_<2> Index;
typedef SField_<2> Field;
typedef SIOFile_<2> IOFile;
typedef SVOF_<2>    VOF;
typedef std::shared_ptr<SGrid_<2> > spSGrid;
typedef std::shared_ptr<SGhost_<2> > spSGhost;
typedef std::shared_ptr<SOrderXYZ_<2> > spOrder;

typedef SGnuplotActor_<2> SGA;
typedef GGnuplotActor_<Vt, 2> GGA;

TEST(vof, initial){
	Vt a = 0.7;
	Vt b = 0.5;
	Point_<Vt, 2> pmin(-1, -1, -1);
	Point_<Vt, 2> pmax( 1, 1,   1);
	Line_<Vt>  line(a, b, 0);
	spSGrid spsg(new SGridUniform_<2>(pmin,
			                          {10, 10},
									   0.2, 2));
	spSGhost spgh(new SGhostRegular_<2>(spsg));
	spOrder sporder(new SOrderXYZ_<2>(spsg, spgh));
	Field sc(spsg,spgh, sporder);  //color field


	// set field
	VOF::SetColor(sc, line);

	Gnuplot gnu;
	gnu.set_terminal_png("vof.png");
	gnu.set_xrange(-1.1, 1.1);
	gnu.set_yrange(-1.1, 1.1);
	gnu.set_equal_aspect_ratio();
	gnu.add(SGA::Contour(sc));
	gnu.add(GGA::Lines(line, -0.5, 0.5, 2));
	gnu.plot();
}

}
#endif
