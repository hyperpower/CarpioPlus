#include "domain/structure/structure.hpp"
#include "domain/structure/io/splotly_actor.hpp"
#include "domain/structure/io/sio_file.hpp"
//#include "domain/structure/io/gnuplot_actor.hpp"
#include "gtest/gtest.h"

using namespace carpio;

// typedef SIndex_<2> Index;
// typedef SField_<2> Field;
// typedef SIOFile_<2> IOFile;
// typedef std::shared_ptr<SGrid_<2> > spSGrid;
// typedef std::shared_ptr<SGhost_<2> > spSGhost;
// typedef std::shared_ptr<SOrderXYZ_<2> > spOrder;
typedef SGridUniform_<2>           Grid;
typedef SGhostRegular_<2, Grid>    Ghost;
typedef SOrderXYZ_<2, Grid, Ghost> Order;
typedef SField_<2, Grid, Ghost, Order> Field;

TEST(scalar, initial){
    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    std::shared_ptr<Grid> spsg(new Grid(pmin,{5, 5}, 0.3, 2 ));
    std::shared_ptr<Ghost> spgh(new Ghost(spsg));
    std::shared_ptr<Order> sporder(new Order(spsg, spgh));

    Field sc(spsg,spgh, sporder);

	Plotly plt;
	plt.add(PlotlyActor::WireFrame(sc.grid()));
	// plt.add(PlotlyActor::Heatmap(sc.grid(), sc));
	plt.size(800, 800);
	// plt.plot();
}

TEST(scalar, outputfile) {
    Point_<Vt, 2> pmin(0, 0, 0);
    Point_<Vt, 2> pmax(1, 1, 1);
    std::shared_ptr<Grid> spsg(new Grid(pmin,{5, 5}, 0.3, 2 ));
    std::shared_ptr<Ghost> spgh(new Ghost(spsg));
    std::shared_ptr<Order> sporder(new Order(spsg, spgh));
    Field sc(spsg, spgh, sporder);

    SIOFile_<2, Grid, Ghost, Order>::OutputField("s.txt", sc);
}

// TEST(field, DISABLED_plot1){
//     std::cout << "field_define 1d\n";
//     const St Dim = 1;
//     typedef SField_<Dim>                        Field1;
//     typedef std::shared_ptr<SGrid_<Dim> >     spGrid;
//     typedef std::shared_ptr<SGhost_<Dim> >    spGhost;
//     typedef std::shared_ptr<SOrderXYZ_<Dim> > spOrder;
//     Point_<Vt, 1> pmin(0, 0, 0);
//     Point_<Vt, 1> pmax(1, 1, 1);
//     spGrid  spsg(new SGridUniform_<Dim>(pmin, { 5, 5 }, 0.3, 2));

//     spGhost spgh(new SGhostRegular_<Dim>(spsg));

//     spOrder  sporder(new SOrderXYZ_<Dim>(spsg, spgh));

//     Field1 sc(spsg, spgh, sporder);
//     sc.assign([](Vt x, Vt y, Vt z, Vt t){
//         return sin(x);
//     });

//     typedef SGnuplotActor_<1> GA;
//     Gnuplot gnu;
//     gnu.set_terminal_png("1d.png");
//     gnu.set_xrange(-0.1, 1.6);
//     auto awf = GA::WireFrame(*spsg);
//     auto acp = GA::CenterPoints(*spsg);
//     acp->style() = "with points pt 2 ps 2 lc variable";
//     auto af  = GA::LinesPoints(sc, 0);
//     af->style() = "with linespoints pt 7 ps 2 lc variable";
//     gnu.add(awf);
//     gnu.add(acp);
//     gnu.add(af);
//     gnu.plot();

//     SIOFile_<1>::OutputField("1d.txt", sc);
// }

// TEST(field, DISABLED_plot3){
//     std::cout << "field_define 3d\n";
//     const St Dim = 3;
//     typedef SField_<Dim>                        Field;
//     typedef std::shared_ptr<SGrid_<Dim> >     spGrid;
//     typedef std::shared_ptr<SGhost_<Dim> >    spGhost;
//     typedef std::shared_ptr<SOrderXYZ_<Dim> > spOrder;
//     Point_<Vt, 3> pmin(0, 0, 0);
//     Point_<Vt, 3> pmax(1, 1, 1);
//     spGrid  spsg(new SGridUniform_<Dim>(pmin, { 5, 5, 5}, 0.3, 2));

//     spGhost spgh(new SGhostRegular_<Dim>(spsg));

//     spOrder  sporder(new SOrderXYZ_<Dim>(spsg, spgh));

//     Field sc(spsg, spgh, sporder);
//     sc.assign([](Vt x, Vt y, Vt z, Vt t){
//         return sin(x);
//     });

//     typedef SGnuplotActor_<Dim> GA;
//     Gnuplot gnu;
//     gnu.set_terminal_png("3d.png");
//     gnu.set_xrange(-0.1, 1.6);
//     gnu.set_yrange(-0.1, 1.6);
//     gnu.set_zrange(-0.1, 1.6);
//     auto awf = GA::WireFrame(*spsg);
//     gnu.add(awf);
// //	awf->show_data();
//     gnu.splot();

// //	SIOFile_<1>::OutputField("1d.txt", sc);
// }


// TEST(scalar, DISABLED_add){

//     Point_<Vt, 2> pmin(0, 0, 0);
//     Point_<Vt, 2> pmax(1, 1, 1);
//     spSGrid spsg(new SGridUniform_<2>(pmin,
//                                       {3, 3},
//                                        0.3, 2 ));

//     spSGhost spgh(new SGhostRegular_<2>(spsg));

//     spOrder sporder(new SOrderXYZ_<2>(spsg, spgh));

//     Field sc(spsg,spgh, sporder);
//     sc.assign(2.0);

//     Field sc2(spsg,spgh, sporder);
//     sc2.assign(3.0);

//     sc += sc2;
//     std::cout << sc << std::endl;
//     ASSERT_EQ(sc(0,0), 5.0);
//     sc += 1.5;
//     ASSERT_EQ(sc(0,0), 6.5);
//     std::cout << sc << std::endl;

//     sc -= sc2;
//     ASSERT_EQ(sc(0,0), 3.5);
//     std::cout << sc << std::endl;
//     sc -= 1.3;
//     ASSERT_EQ(sc(0,0), 2.2);
//     std::cout << sc << std::endl;

//     sc *= sc2;
//     ASSERT_LE(std::abs(sc(0,0)- 6.60), 1e-4);
//     std::cout << sc << std::endl;
//     sc *= 2.0;
//     ASSERT_LE(std::abs(sc(0,0)- 13.2), 1e-4);
//     std::cout << sc << std::endl;

//     sc /= sc2;
//     std::cout << "sc /= sc2;\n";
//     ASSERT_LE(std::abs(sc(0, 0) - 4.40), 1e-4);
//     std::cout << sc << std::endl;
//     sc /= 2.0;
//     std::cout << "sc /= 2.0;\n";
//     ASSERT_LE(std::abs(sc(0, 0) - 2.20), 1e-4);
//     std::cout << sc << std::endl;

//     auto sc3 = sc + sc2;
//     std::cout << "sc3 = sc + sc2\n";
//     sc3 = sc + 2.6;
//     std::cout << "sc3 = sc + 2.6\n";
//     sc3 = 2.7 + sc + 2.6;
//     std::cout << "sc3 = 2.7 + sc + 2.6\n";

//     sc3 = sc - 2.6;
//     std::cout << "sc3 = sc - 2.6\n";
//     std::cout << "      "<< sc(0,0) << " - 2.6" << std::endl;
//     std::cout << "sc3 = "<< sc3(0,0) << std::endl;
//     sc3 = 2.7 - sc - 2.6;
//     std::cout << "sc3 = 2.7 - sc - 2.6\n";
//     std::cout << "      2.7 - " << sc(0, 0) << " - 2.6" << std::endl;
//     std::cout << "sc3 = " << sc3(0, 0) << std::endl;
//     std::cout << "--------------\n";
//     sc3 = sc * 2.6;
//     std::cout << "sc3 = sc * 2.6\n";
//     std::cout << "      " << sc(0, 0) << " * 2.6" << std::endl;
//     std::cout << "sc3 = " << sc3(0, 0) << std::endl;
//     ASSERT_LE(std::abs(sc3(0, 0) - (sc(0, 0) * 2.6)), 1e-4);
//     std::cout << "--------------\n";
//     sc3 = 2.7 * sc * 2.6;
//     std::cout << "sc3 = 2.7 * sc * 2.6\n";
//     std::cout << "      2.7 * " << sc(0, 0) << " * 2.6" << std::endl;
//     std::cout << "sc3 = " << sc3(0, 0) << std::endl;
//     ASSERT_LE(std::abs(sc3(0, 0) - (2.7 * sc(0, 0) * 2.6)), 1e-4);
//     std::cout << "--------------\n";
//     sc3 = sc / 2.6;
//     std::cout << "sc3 = sc / 2.6\n";
//     std::cout << "      " << sc(0, 0) << " / 2.6" << std::endl;
//     std::cout << "sc3 = " << sc3(0, 0) << std::endl;
//     ASSERT_LE(std::abs(sc3(0, 0) - (sc(0, 0) / 2.6)), 1e-4);
//     sc3 = 2.3 / sc;
//     std::cout << "sc3 = 2.3 / sc\n";
//     std::cout << "      2.3 / " << sc(0, 0) << std::endl;
//     std::cout << "sc3 = " << sc3(0, 0) << std::endl;
//     ASSERT_LE(std::abs(sc3(0, 0) - (2.3 / sc(0, 0))), 1e-4);
// }

// TEST(scalar, DISABLED_corner){
//     std::cout << "corner test -----" << std::endl;
//     const St Dim = 1;
//     typedef SField_<Dim>                        Field;
//     typedef std::shared_ptr<SGrid_<Dim> >     spGrid;
//     typedef std::shared_ptr<SGhost_<Dim> >    spGhost;
//     typedef std::shared_ptr<SOrderXYZ_<Dim> > spOrder;
//     typedef SCorner_<Dim>                       Corner;
//     typedef std::shared_ptr<SCorner_<Dim> >   spCorner;
//     typedef SInterpolate_<Dim>                  Inter;
//     typedef BoundaryIndex                        BI;
//     typedef std::shared_ptr<BoundaryIndex>     spBI;
//     typedef BoundaryCondition                    BC;
//     typedef std::shared_ptr<BoundaryCondition> spBC;
//     // type define
//     Point_<Vt, Dim> pmin(0, 0, 0);
//     Point_<Vt, Dim> pmax(1, 1, 1);
//     spGrid  spsg(new SGridUniform_<Dim>(pmin, { 5, 5, 5}, 0.3, 2));

//     spGhost spgh(new SGhostRegular_<Dim>(spsg));

//     spOrder  sporder(new SOrderXYZ_<Dim>(spsg, spgh));

//     Corner c(spsg, spgh, sporder);

//     Field sc(spsg, spgh, sporder);
//     sc.assign([](Vt x, Vt y, Vt z, Vt t){
//         return sin(x);
//     });

//     // boundary define
//     spBI bi(new BI());
//     spBC spbcm(new BoundaryConditionValue(BC::_BC1_, 0.3));
//     spBC spbcp(new BoundaryConditionValue(BC::_BC1_, 0.5));
//     bi->insert(0, spbcm);
//     bi->insert(1, spbcp);

//     Inter::CenterToCorner(sc, c, bi);

//     std::cout << c(0, 0, 0, 0) << std::endl;
//     typedef SGnuplotActor_<Dim> GA;
//     Gnuplot gnu;
//     gnu.set_terminal_png("vc_1d.png");
//     gnu.set_xrange(-0.1, 1.6);
//     gnu.set_yrange(-0.1, 1.2);
//     auto awf = GA::WireFrame(*spsg);
//     auto asc = GA::LinesPoints(sc);
//     asc->command() = "using 1:2:3 title \"Value on Scalar center\" ";
//     auto asv = GA::LinesPoints(c, 2);
//     asv->command() = "using 1:2:3 title \"Valut on Vertex\" ";
//     gnu.add(awf);
//     gnu.add(asc);
//     gnu.add(asv);
//     gnu.plot();
// }

// TEST(scalar, DISABLED_corner2){
//     std::cout << "corner test -----" << std::endl;
//     const St Dim = 2;
//     typedef SField_<Dim>                        Field;
//     typedef std::shared_ptr<SGrid_<Dim> >     spGrid;
//     typedef std::shared_ptr<SGhost_<Dim> >    spGhost;
//     typedef std::shared_ptr<SOrderXYZ_<Dim> > spOrder;
//     typedef SCorner_<Dim>                       Corner;
//     typedef std::shared_ptr<SCorner_<Dim> >   spCorner;
//     typedef SInterpolate_<Dim>                  Inter;
//     typedef BoundaryIndex                        BI;
//     typedef std::shared_ptr<BoundaryIndex>     spBI;
//     typedef BoundaryCondition                    BC;
//     typedef std::shared_ptr<BoundaryCondition> spBC;
//     // type define
//     Point_<Vt, Dim> pmin(0, 0, 0);
//     Point_<Vt, Dim> pmax(1, 1, 1);
//     spGrid  spsg(new SGridUniform_<Dim>(pmin, { 50, 50, 5}, 0.03, 2));

//     spGhost spgh(new SGhostRegular_<Dim>(spsg));

//     spOrder  sporder(new SOrderXYZ_<Dim>(spsg, spgh));

//     Corner c(spsg, spgh, sporder);

//     Field sc(spsg, spgh, sporder);
//     sc.assign([](Vt x, Vt y, Vt z, Vt t){
//         return sin(x + y);
//     });

//     // boundary define
//     spBI bi(new BI());
//     spBC spbcm(new BoundaryConditionValue(BC::_BC1_, 0.0));
//     spBC spbcp(new BoundaryConditionValue(BC::_BC1_, 1.3));
//     bi->insert(0, spbcm);
//     bi->insert(1, spbcp);

//     Inter::CenterToCorner(sc, c, bi);
//     Index idx(0,0);
//     std::cout <<"0 " <<c(0, idx) << std::endl;
//     std::cout <<"1 " <<c(1, idx) << std::endl;
//     std::cout <<"3 " <<c(3, idx) << std::endl;
//     std::cout <<"2 " <<c(2, idx) << std::endl;
//     idx = {1,1};
//     std::cout <<"0 " <<c(0, idx) << std::endl;
//     std::cout <<"1 " <<c(1, idx) << std::endl;
//     std::cout <<"3 " <<c(3, idx) << std::endl;
//     std::cout <<"2 " <<c(2, idx) << std::endl;
// //
// //	std::cout << c(0, 0, 0, 0) << std::endl;
//     typedef SGnuplotActor_<Dim> GA;
//     Gnuplot gnu;
// //	gnu.set_terminal_png("c2d.png");
//     gnu.set_xrange(-0.1, 1.6);
//     gnu.set_yrange(-0.1, 1.6);
//     auto awf = GA::WireFrame(c);
// //	auto asc = GA::LinesPoints(sc);
// //	asc->command() = "using 1:2:3 title \"Value on Scalar center\" ";
// //	auto asv = GA::LinesPoints(c, 2);
// //	asv->command() = "using 1:2:3 title \"Valut on Vertex\" ";
//     gnu.add(awf);
// //	gnu.add(asc);
// //	gnu.add(asv);
// //	gnu.splot();
// }

// TEST(scalar, DISABLED_corner_assign){
//     std::cout << "corner test -----" << std::endl;
//     const St Dim = 2;
//     typedef SField_<Dim>                        Field;
//     typedef std::shared_ptr<SGrid_<Dim> >     spGrid;
//     typedef std::shared_ptr<SGhost_<Dim> >    spGhost;
//     typedef std::shared_ptr<SOrderXYZ_<Dim> > spOrder;
//     typedef SCorner_<Dim>                       Corner;
//     typedef std::shared_ptr<SCorner_<Dim> >   spCorner;
//     typedef SInterpolate_<Dim>                  Inter;
//     typedef BoundaryIndex                        BI;
//     typedef std::shared_ptr<BoundaryIndex>     spBI;
//     typedef BoundaryCondition                    BC;
//     typedef std::shared_ptr<BoundaryCondition> spBC;
//     // type define
//     Point_<Vt, Dim> pmin(0, 0, 0);
//     Point_<Vt, Dim> pmax(1, 1, 1);
//     spGrid  spsg(new SGridUniform_<Dim>(pmin, { 10, 10, 5}, 0.15, 2));

//     spGhost spgh(new SGhostRegular_<Dim>(spsg));

//     spOrder  sporder(new SOrderXYZ_<Dim>(spsg, spgh));

//     Corner c(spsg, spgh, sporder);

//     typename Corner::FunXYZT_Value fun = [](Vt x, Vt y, Vt z, Vt t){
//         return sin(x + y);
//     };

//     c.assign(fun, 0);

// //	std::cout << c(0, 0, 0, 0) << std::endl;
//     typedef SGnuplotActor_<Dim> GA;
//     Gnuplot gnu;
// //	gnu.set_terminal_png("c2d.png");
//     gnu.set_xrange(-0.1, 1.6);
//     gnu.set_yrange(-0.1, 1.6);
//     auto awf = GA::WireFrame(c);
// //	auto asc = GA::LinesPoints(sc);
// //	asc->command() = "using 1:2:3 title \"Value on Scalar center\" ";
// //	auto asv = GA::LinesPoints(c, 2);
// //	asv->command() = "using 1:2:3 title \"Valut on Vertex\" ";
//     gnu.add(awf);
// //	gnu.add(asc);
// //	gnu.add(asv);
//     gnu.splot();
// }

// TEST(scalar, DISABLED_corner_plotly) {
//     std::cout << "corner test plotly -----" << std::endl;
//     const St Dim = 3;
//     typedef SField_<Dim> Field;
//     typedef std::shared_ptr<SGrid_<Dim> > spGrid;
//     typedef std::shared_ptr<SGhost_<Dim> > spGhost;
//     typedef std::shared_ptr<SOrderXYZ_<Dim> > spOrder;
//     typedef SCorner_<Dim> Corner;
//     typedef std::shared_ptr<SCorner_<Dim> > spCorner;
//     typedef SInterpolate_<Dim> Inter;
//     typedef BoundaryIndex BI;
//     typedef std::shared_ptr<BoundaryIndex> spBI;
//     typedef BoundaryCondition BC;
//     typedef std::shared_ptr<BoundaryCondition> spBC;
//     // type define
//     Point_<Vt, Dim> pmin(0, 0, 0);
//     Point_<Vt, Dim> pmax(1, 1, 1);
//     spGrid spsg(new SGridUniform_<Dim>(pmin, { 10, 10, 5 }, 0.15, 2));

//     spGhost spgh(new SGhostRegular_<Dim>(spsg));

//     spOrder sporder(new SOrderXYZ_<Dim>(spsg, spgh));

//     Corner c(spsg, spgh, sporder);

//     typename Corner::FunXYZT_Value fun = [](Vt x, Vt y, Vt z, Vt t) {
//         return sin(x + y + z);
//     };

//     c.assign(fun, 0);

//     Plotly ply;
//     ply.add(PlotlyActor::WireFrame(*spsg));
//     ply.plot();
// }