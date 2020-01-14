#ifndef _S_GNUPLOT_ACTOR_HPP
#define _S_GNUPLOT_ACTOR_HPP

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/operation/soperation.hpp"
#include "io/gnuplot.hpp"

namespace carpio{

using namespace GnuplotActor;

template<St DIM>
class SGnuplotActor_ {
public:
    static const St Dim = DIM;
    typedef ArrayListT_<Vt> Arr;
    typedef Point_<Vt, Dim> Poi;
    typedef SIndex_<Dim> Index;
    typedef St size_type;

    typedef SGrid_<DIM>   Grid;
    typedef SGrid_<1>     Grid1;
    typedef SGrid_<2>     Grid2;
    typedef SGrid_<3>     Grid3;
    typedef SGhost_<DIM>  Ghost;
    typedef SGhost_<1>    Ghost1;
    typedef SGhost_<2>    Ghost2;
    typedef SGhost_<3>    Ghost3;

    typedef SGhostLinearCut_<DIM> GhostLinearCut;
    typedef SGhostLinearCut_<1>   GhostLinearCut1;
    typedef SGhostLinearCut_<2>   GhostLinearCut2;
    typedef SGhostLinearCut_<3>   GhostLinearCut3;

    typedef SCellLinearCut_<DIM> CellLinearCut;
    typedef SCellLinearCut_<1> CellLinearCut1;
    typedef SCellLinearCut_<2> CellLinearCut2;
    typedef SCellLinearCut_<3> CellLinearCut3;

    typedef SOrder_<DIM>  Order;
    typedef SOrder_<1>    Order1;
    typedef SOrder_<2>    Order2;
    typedef SOrder_<3>    Order3;
    typedef SField_<DIM>  Field;
    typedef SField_<1>    Field1;
    typedef SField_<2>    Field2;
    typedef SField_<3>    Field3;
    typedef SVectorCenter_<DIM> VC;
    typedef SVectorCenter_<1>   VC1;
    typedef SVectorCenter_<2>   VC2;
    typedef SVectorCenter_<3>   VC3;

    typedef SVectorFace_<DIM> VF;
    typedef SVectorFace_<1>   VF1;
    typedef SVectorFace_<2>   VF2;
    typedef SVectorFace_<3>   VF3;

    typedef SCorner_<DIM>     Corner;
    typedef SCorner_<1>       Corner1;
    typedef SCorner_<2>       Corner2;
    typedef SCorner_<3>       Corner3;

    typedef SIndex_<1>    Index1;
    typedef SIndex_<2>    Index2;
    typedef SIndex_<3>    Index3;

    typedef SLoop_<DIM>   Loop;

    static spActor WireFrame(
            const Grid2& grid, int color_idx = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with line lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
        for (St j = 0; j < grid.n(_Y_); j++) {
            for (St i = 0; i < grid.n(_X_); i++) {
                typename Grid2::Index index(i, j);
                for (short o = 0; o < grid.num_vertex(); ++o) {
                    auto p = grid.v(order[o], index);
                    actor->data().push_back(
                                ToString(p.value(_X_),
                                         p.value(_Y_),
                                         c,
                                         " "));
                }
                auto p = grid.v(0, index);
                actor->data().push_back(
                            ToString(p.value(_X_), p.value(_Y_), c,
                                    " "));
                actor->data().push_back("");
            }
        }
        return actor;
    }



    static spActor CenterPoints(const Grid1& grid, int color_idx = -1){
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with points lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        for (St i = 0; i < grid.n(_X_); i++) {
            typename Grid1::Index index(i);
            auto p = grid.c(index);
            actor->data().push_back(
                    ToString(p.value(_X_), 0.0, c, " "));
            actor->data().push_back("");
        }
        return actor;
    }

    static spActor CenterPoints(const Grid2& grid, int color_idx = -1){
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with points lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;
        for (St j = 0; j < grid.n(_Y_); j++) {
            for (St i = 0; i < grid.n(_X_); i++) {
            typename Grid2::Index index(i, j);
            auto p = grid.c(index);
            actor->data().push_back(
                    ToString(p.value(_X_), p.value(_Y_), c, " "));
            actor->data().push_back("");
            }
        }
        return actor;
    }
    //plot by splot
    static spActor FacePoints(const VF2& vf, int color_idx = 1){
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3:4 title \"\" ";
        actor->style()   = "with points lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;
        for (auto& index : vf.order()) {
            for(St d = 0; d < 2; d++){
            if(vf.ghost().is_boundary(index, d, _M_)){
                auto fm = vf.grid().f(d, _M_, index);
                auto v  = vf(d, _M_, index);
                if(color_idx < 0){
                    c = v;
                }
            actor->data().push_back(ToString(fm.x(), fm.y(), v, c, " "));
            }
            auto fp = vf.grid().f(d, _P_, index);
            auto v  = vf(d, _P_, index);
            if (color_idx < 0) {
                c = v;
            }
            actor->data().push_back(ToString(fp.x(), fp.y(), v, c, " "));
            }
        }
        return actor;
    }

    static spActor Contour(const Field2& f){
        spActor actor = spActor(new Gnuplot_actor());
        auto    ghost = f.spghost();
        actor->command() = "using 1:2:3:4:5:6:7 title \"\" ";
        actor->style()   = "with boxxy fs solid palette";
        for (St i = 0; i < f.grid().n(_X_); i++) {
            for (St j = 0; j < f.grid().n(_Y_); j++) {
                Index2 index(i, j);
				if (ghost->is_normal(index) == true) {
					auto pc   = f.grid().c(index);
					auto pmin = f.grid().v(0, index);
					auto pmax = f.grid().v(3, index);
					actor->data().push_back(
							ToString(pc(_X_), pc(_Y_), pmin(_X_), pmax(_X_),
									pmin(_Y_), pmax(_Y_), f(index), " "));
				}
            }
        }
        return actor;
    }


    static spActor Contour(const Field1& f){
        SHOULD_NOT_REACH;
    }


    static spActor WireFrame(const Ghost2& g,  int color_idx = 1) {
		typedef CuboidToolPL2_<Vt> Tool;
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with line lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
        auto& grid = g.grid();
        auto gl    = grid.ghost_layer();
        for (int j = -gl; j < grid.n(_Y_) + gl; j++) {
            for (int i = -gl; i < grid.n(_X_) + gl; i++) {
                typename Grid2::Index index(i, j);
                if (g.is_ghost(index)) {
                    for (short o = 0; o < grid.num_vertex(); ++o) {
                        auto p = grid.v(order[o], index);
                        actor->data().push_back(
                                ToString(p.value(_X_), p.value(_Y_), c, " "));
                    }
                    auto p = grid.v(0, index);
                    actor->data().push_back(
                            ToString(p.value(_X_), p.value(_Y_), c, " "));
                    actor->data().push_back("");
                }
            }
        }
        return actor;
    }

    static spActor WireFrameCut(
    		const GhostLinearCut2& g,
			int   color_idx = 1) {
		typedef CuboidToolPL2_<Vt> Tool;
		Tool tool;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style() = "with line lc variable";
		int c = (color_idx == -1) ? 0 : color_idx;

		short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
		auto& grid = g.grid();
		auto gl = grid.ghost_layer();
		for (int j = -gl; j < grid.n(_Y_) + gl; j++) {
			for (int i = -gl; i < grid.n(_X_) + gl; i++) {
				typename Grid2::Index index(i, j);
				if (g.is_cut(index)) {
					auto po     = grid.v(0, index);
					auto sx     = grid.s_(_X_, index);
					auto sy     = grid.s_(_Y_, index);
					for (short o = 0; o < grid.num_vertex(); ++o) {
						auto p = grid.v(order[o], index);
						actor->data().push_back(
								ToString(p.value(_X_), p.value(_Y_), c, " "));
					}
					auto p = grid.v(0, index);
					actor->data().push_back(
							ToString(p.value(_X_), p.value(_Y_), c, " "));
					actor->data().push_back("");
				}
			}
		}
		return actor;
	}


	static spActor WireFrameCutInterface(
			const GhostLinearCut2& g,
			int   color_idx = 1) {
		typedef CuboidToolPL2_<Vt> Tool;
		Tool tool;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style() = "with line lc variable";
		int c = (color_idx == -1) ? 0 : color_idx;

		short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
		auto& grid = g.grid();
		auto gl = grid.ghost_layer();
		for (int j = -gl; j < grid.n(_Y_) + gl; j++) {
			for (int i = -gl; i < grid.n(_X_) + gl; i++) {
				typename Grid2::Index index(i, j);
				if (g.is_cut(index)) {
					auto spcell = g(index);
					auto arr = spcell->get_aperture_ratio();
					auto po = grid.v(0, index);
					auto sx = grid.s_(_X_, index);
					auto sy = grid.s_(_Y_, index);
					auto sp = tool.start_point(po.x(), po.y(), sx, sy, arr);
					auto ep = tool.end_point(po.x(), po.y(), sx, sy, arr);
					actor->data().push_back("");
					actor->data().push_back(
							ToString(sp.value(_X_), sp.value(_Y_), c, " "));
					actor->data().push_back(
							ToString(ep.value(_X_), ep.value(_Y_), c, " "));
					actor->data().push_back("");
				}
			}
		}
		return actor;
	}

	static spActor WireFrameCutInterface(
				const GhostLinearCut2& g,
				const Index2&          index,
				int   color_idx = 1) {
			typedef CuboidToolPL2_<Vt> Tool;
		Tool tool;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style() = "with line lc variable";
		int c = (color_idx == -1) ? 0 : color_idx;

		short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
		auto& grid = g.grid();
		if (g.is_cut(index)) {
			auto spcell = g(index);
			auto arr = spcell->get_aperture_ratio();
			auto po = grid.v(0, index);
			auto sx = grid.s_(_X_, index);
			auto sy = grid.s_(_Y_, index);
			auto sp = tool.start_point(po.x(), po.y(), sx, sy, arr);
			auto ep = tool.end_point(po.x(), po.y(), sx, sy, arr);
			actor->data().push_back("");
			actor->data().push_back(
					ToString(sp.value(_X_), sp.value(_Y_), c, " "));
			actor->data().push_back(
					ToString(ep.value(_X_), ep.value(_Y_), c, " "));
			actor->data().push_back("");
		}
		return actor;
	}


	static spActor WireFrameCutGhostSide(
			const GhostLinearCut2& g, int color_idx = 1) {
		typedef CuboidToolPL2_<Vt> Tool;
		Tool tool;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style() = "with line lc variable";
		int c = (color_idx == -1) ? 0 : color_idx;

		short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
		auto& grid = g.grid();
		auto gl = grid.ghost_layer();
		for (int j = -gl; j < grid.n(_Y_) + gl; j++) {
			for (int i = -gl; i < grid.n(_X_) + gl; i++) {
				typename Grid2::Index index(i, j);
				if (g.is_cut(index)) {
                    auto spcell = g(index);
                    auto arr = spcell->get_aperture_ratio();
					auto po = grid.v(0, index);
					auto sx = grid.s_(_X_, index);
					auto sy = grid.s_(_Y_, index);
                    auto pc = tool.cut_cell_point_chain_ghost_side(po.x(), po.y(), sx, sy, arr);
					for (auto p : pc) {
						actor->data().push_back(
								ToString(p.value(_X_), p.value(_Y_), c, " "));
					}
					auto p = pc.front();
					actor->data().push_back(
							ToString(p.value(_X_), p.value(_Y_), c, " "));
					actor->data().push_back("");
                }
			}
		}
		return actor;
	}
    static spActor WireFrameCutNormalSide(
			const GhostLinearCut2& g,
			int color_idx = 1) {
		typedef CuboidToolPL2_<Vt> Tool;
		Tool tool;
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style() = "with line lc variable";
		int c = (color_idx == -1) ? 0 : color_idx;

		short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
		auto& grid = g.grid();
		auto gl = grid.ghost_layer();
		for (int j = -gl; j < grid.n(_Y_) + gl; j++) {
			for (int i = -gl; i < grid.n(_X_) + gl; i++) {
				typename Grid2::Index index(i, j);
				if (g.is_cut(index)) {

					auto spcell = g(index);
					auto arr = spcell->get_aperture_ratio();
					auto po = grid.v(0, index);
					auto sx = grid.s_(_X_, index);
					auto sy = grid.s_(_Y_, index);
					auto pc = tool.cut_cell_point_chain_normal_side(po.x(), po.y(), sx, sy,
							arr);
					for (auto p : pc) {
						actor->data().push_back(
								ToString(p.value(_X_), p.value(_Y_), c, " "));
					}
					auto p = pc.front();
					actor->data().push_back(
							ToString(p.value(_X_), p.value(_Y_), c, " "));
					actor->data().push_back("");
				}
			}
		}
		return actor;
	}


    static spActor Boundary(const Ghost2& g) {
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style() = "with line lc variable";
        int c = 0;

        short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };
        auto& grid = g.grid();
        auto gl = grid.ghost_layer();
        for (int j = 0; j < grid.n(_Y_); j++) {
            for (int i = 0; i < grid.n(_X_); i++) {
                typename Grid2::Index index(i, j);
                for (St d = 0; d < 2; d++) {
                    for (St o = 0; o < 2; o++) {
                        if (g.is_boundary(index, d, o)) {
                            auto idxg = index.shift(d, o);
                            auto bid  = g.boundary_id(index, idxg, d, o);

                            auto fc   = grid.f(d, o, index);
                            auto dalt = (d==_X_)? _Y_:_X_;
                            auto hs   = grid.hs_(dalt, index);
                            if(dalt == _X_){
                                actor->data().push_back(
                                    ToString(fc.x() - hs, fc.y(), bid, " "));
                                actor->data().push_back(
                                    ToString(fc.x() + hs, fc.y(), bid, " "));
                                actor->data().push_back("");
                            }else{
                                actor->data().push_back(
                                    ToString(fc.x(), fc.y() - hs, bid, " "));
                                actor->data().push_back(
                                    ToString(fc.x(), fc.y() + hs, bid, " "));
                                actor->data().push_back("");
                            }
                        }
                    }
                }
            }
        }
        return actor;
    }

    static void _AppendCutBoundary(spActor actor, const Grid2& grid, const Index2& index, const CellLinearCut2& cell){

    }

    static spActor WireFrame(
                const Grid1& grid,
                const Vt&    tik  = 0.1,
                int color_idx     = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with line lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };

        Vt xs, xe;
        for (St i = 0; i < grid.n(_X_); i++) {
            typename Grid1::Index index(i);
            auto p = grid.f(_X_, _M_, index);
            actor->data().push_back(
                    ToString(p.value(_X_), -0.0, c, " "));
            actor->data().push_back(
                    ToString(p.value(_X_),  tik, c, " "));
            actor->data().push_back("");
            if(i == 0){
                xs = p.value(_X_);
            }

        }
        typename Grid1::Index index(grid.n(_X_) - 1);
        auto p = grid.f(_X_, _P_, index);
        actor->data().push_back(
                ToString(p.value(_X_), -0.0, c, " "));
        actor->data().push_back(
                ToString(p.value(_X_),  tik, c, " "));
        actor->data().push_back("");
        xe = p.value(_X_);

        actor->data().push_back(ToString(xs, 0.0, c, " "));
        actor->data().push_back(ToString(xe, 0.0, c, " "));

        return actor;
    }


    static spActor Lines(const Field1& s, int color_idx = -1) {
        spActor actor    = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with line lc variable";

        for(auto& index: s.order()){
            auto x = s.grid().c_(_X_, index);
            auto v = s(index);
            if (color_idx >= 0) {
                actor->data().push_back(
                        ToString(x, v, color_idx, " "));
            } else {
                actor->data().push_back(
                        ToString(x, v, 0, " "));
            }
        }
        return actor;
    }

    static spActor Lines(const Field2& s, int color_idx = -1) {
        SHOULD_NOT_REACH;
    }

    static spActor LinesPoints(const Field1& s, int color_idx = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with linespoints lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        for (auto& index : s.order()) {
            auto x = s.grid().c_(_X_, index);
            auto v = s(index);
            actor->data().push_back(ToString(x, v, c, " "));
        }
        return actor;
    }

    static spActor LinesPoints(const Corner1& s, int color_idx = -1) {
        spActor actor    = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with linespoints lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        for (auto& index : s.order()) {
            if (s.ghost().is_boundary(index, _X_, _M_)) {
                auto p = s.grid().v(0, index);
                auto v = s(1, index);
                actor->data().push_back(ToString(p.x(), v, c, " "));
            }
            auto p = s.grid().v(1, index);
            auto v = s(1, index);
            actor->data().push_back(ToString(p.x(), v, c, " "));
        }
        return actor;
    }

    static spActor LinesPoints(const VC1& s, int color_idx = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with linespoints lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        for (auto& index : s.order()) {
            auto x = s.grid().c_(_X_, index);
            auto v = s[_X_](index);
            actor->data().push_back(ToString(x, v, c, " "));
        }
        return actor;
    }

    static spActor LinesPoints(const VF1& s, int color_idx = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3 title \"\" ";
        actor->style()   = "with linespoints lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        for (auto& index : s.order()) {
            if(s.ghost().is_boundary(index, _X_, _M_)){
                auto p = s.grid().f(_X_,_M_, index);
                auto v = s(_X_, _M_, index);
                actor->data().push_back(ToString(p.x(), v, c, " "));
            }
            auto p = s.grid().f(_X_,_P_, index);
            auto v = s(_X_, _P_, index);
            actor->data().push_back(ToString(p.x(), v, c, " "));
        }
        return actor;
    }

    static spActor Arrows(
            const VC1& s,
                   Vt  unit_length = -1.0,
                  int  color_idx   = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        Vt      color = color_idx;
        actor->command() = "using 1:2:3:4:5 title \"\" ";
        actor->style()   = "with vectors lc variable";
        if(unit_length <= 0){
            unit_length = s[_X_].max() * 2.0;
        }
        auto gmin_size = s.grid().min_size();

        for (auto& index : s.order()) {
            auto x  = s.grid().c_(_X_, index);
            Vt   y  = 0.0;
            auto v  = s[_X_](index);
            auto dx = v / unit_length * gmin_size;
            Vt   dy = 0.0;
            if(color_idx < 0){
                color = v;
            }
            actor->data().push_back(ToString(x, y, dx, dy, color, " "));
        }

        return actor;
    }

    static spActor Arrows(
                const VF1& s,
                       Vt  unit_length = -1.0,
                      int  color_idx   = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        Vt      color = color_idx;
        actor->command() = "using 1:2:3:4:5 title \"\" ";
        actor->style()   = "with vectors lc variable";
        if(unit_length <= 0){
            unit_length = s.max() * 2.0;
        }
        auto gmin_size = s.grid().min_size();

        for (auto& index : s.order()) {
            if(s.ghost().is_boundary(index, _X_, _M_)){
                auto fm = s.grid().f(_X_, _M_, index);
                auto v  = s(_X_, _M_, index);
                auto dx = v / unit_length * gmin_size;
                Vt   x  = fm(_X_) - (dx * 0.5);
                Vt   y  = 0.0;
                Vt   dy = 0.0;
                if(color_idx < 0){
                    color = v;
                }
                actor->data().push_back(ToString(x, y, dx, dy, color, " "));
            }
            auto fp = s.grid().f(_X_, _P_, index);
            auto v  = s(_X_, _P_, index);
            auto dx = v / unit_length * gmin_size;
            Vt x    = fp(_X_) - (dx * 0.5);
            Vt y    = 0.0;
            Vt dy   = 0.0;
            if (color_idx < 0) {
                color = v;
            }
            actor->data().push_back(ToString(x, y, dx, dy, color, " "));
        }
        return actor;
    }

    static spActor Arrows(
                const VF2& s,
                       Vt  unit_length = -1.0,
                      int  color_idx   = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        Vt      color = color_idx;
        actor->command() = "using 1:2:3:4:5 title \"\" ";
        actor->style()   = "with vectors lc variable";
        if(unit_length <= 0){
            unit_length = s.max() * 2.0;
        }
        auto gmin_size = s.grid().min_size();

        for (auto& index : s.order()) {
            for(St d = 0; d < 2; d++){
            if(s.ghost().is_boundary(index, d, _M_)){
                auto fm = s.grid().f(d, _M_, index);
                auto v  = s(d, _M_, index);
                auto dl = v / unit_length * gmin_size;
                Vt   x  = (d == _X_) ? fm(d) - (dl * 0.5) : fm.x();
                Vt   y  = (d == _X_) ? fm.y() : (fm(d) - (dl * 0.5));
                Vt   dx = (d == _X_) ? dl : 0.0;
                Vt   dy = (d == _X_) ? 0.0: dl;
                if(color_idx < 0){
                    color = v;
                }
                actor->data().push_back(ToString(x, y, dx, dy, color, " "));
            }
            auto fp = s.grid().f(d, _P_, index);
            auto v  = s(d, _P_, index);
            auto dl = v / unit_length * gmin_size;
            Vt   x  = (d == _X_) ? fp(d) - (dl * 0.5) : fp.x();
            Vt   y  = (d == _X_) ? fp.y() : (fp(d) - (dl * 0.5));
            Vt   dx = (d == _X_) ? dl : 0.0;
            Vt   dy = (d == _X_) ? 0.0: dl;
            if (color_idx < 0) {
                color = v;
            }
            actor->data().push_back(ToString(x, y, dx, dy, color, " "));
            }
        }
        return actor;
    }

    static spActor ArrowsAxesAlign(
                const VC2& s,
                       Vt  unit_length = -1.0,
                      int  color_idx   = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        Vt      colorx = color_idx;
        Vt      colory = color_idx;
        actor->command() = "using 1:2:3:4:5 title \"\" ";
        actor->style()   = "with vectors lc variable";
        if(unit_length <= 0){
            auto xmax = s[_X_].max() * 2.0;
            auto ymax = s[_Y_].max() * 2.0;
            unit_length = std::max(xmax, ymax);
        }
        auto gmin_size = s.grid().min_size();

        for (auto& index : s.order()) {
            auto x  = s.grid().c_(_X_, index);
            auto y  = s.grid().c_(_Y_, index);
            auto vx = s[_X_](index);
            auto vy = s[_Y_](index);
            auto dx = vx / unit_length * gmin_size;
            auto dy = vy / unit_length * gmin_size;
            if(color_idx < 0){
                colorx = vx;
                colory = vy;
            }
            actor->data().push_back(ToString(x, y, dx,  0.0, colorx, " "));
            actor->data().push_back(ToString(x, y, 0.0, dy,  colory, " "));
        }
        return actor;
    }

    static spActor Arrows(
                    const VC2& s,
                          Vt  unit_length = -1.0,
                          int  color_idx  = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        Vt color = color_idx;
        actor->command() = "using 1:2:3:4:5 title \"\" ";
        actor->style() = "with vectors lc variable";

        auto ss = SquareSum(s[_X_], s[_Y_]);
        if (unit_length <= 0) {
            unit_length = std::sqrt(ss.max()) * 2.0;
        }
        auto gmin_size = s.grid().min_size();

        for (auto& index : s.order()) {
            auto x = s.grid().c_(_X_, index);
            auto y = s.grid().c_(_Y_, index);
            auto vx = s[_X_](index);
            auto vy = s[_Y_](index);
            auto dx = vx / unit_length * gmin_size;
            auto dy = vy / unit_length * gmin_size;
            if (color_idx < 0) {
                color = std::sqrt(ss(index));
            }
            actor->data().push_back(ToString(x, y, dx, dy, color, " "));
        }
        return actor;
    }

    static spActor Arrows(
                    const VC3& s,
                          Vt   unit_length = -1.0,
                          int  color_idx  = -1) {
        spActor actor = spActor(new Gnuplot_actor());
        Vt color = color_idx;
        actor->command() = "using 1:2:3:4:5:6:7 title \"\" ";
        actor->style()   = "with vectors lc variable";

        auto ss = SquareSum(s[_X_], s[_Y_], s[_Z_]);
        if (unit_length <= 0) {
            unit_length = std::sqrt(ss.max()) * 2.0;
        }
        auto gmin_size = s.grid().min_size();

        for (auto& index : s.order()) {
            auto x = s.grid().c_(_X_, index);
            auto y = s.grid().c_(_Y_, index);
            auto z = s.grid().c_(_Z_, index);
            auto vx = s[_X_](index);
            auto vy = s[_Y_](index);
            auto vz = s[_Z_](index);
            auto dx = vx / unit_length * gmin_size;
            auto dy = vy / unit_length * gmin_size;
            auto dz = vz / unit_length * gmin_size;
            if (color_idx < 0) {
                color = std::sqrt(ss(index));
            }
            actor->data().push_back(ToString(x, y, z, dx, dy, dz, color, " "));
        }
        return actor;
    }
    static spActor WireFrame(
            const Corner2& corner, int color_idx = -1
            ){
        spActor actor = spActor(new Gnuplot_actor());
        actor->command() = "using 1:2:3:4 title \"\" ";
        actor->style()   = "with lines lc variable";
        int c = (color_idx == -1) ? 0 : color_idx;

        for (auto& idx : corner.order()) {
            short order[] = { 0, 1, 3, 2, 0};
            for (short o = 0; o < 5; ++o) {
                auto p = corner.grid().v(order[o], idx);
                auto vc = corner(order[o], idx);
                actor->data().push_back(
                            ToString(p.value(_X_),
                                     p.value(_Y_),
                                     vc,
                                     vc,
                                     " "));
//                std::cout << ToString(p.value(_X_),
//                         p.value(_Y_),
//                         vc,
//                         vc,
//                         " ") << std::endl;
            }
            actor->data().push_back(""); //splot sperated with two lines
            actor->data().push_back("");
        }
        return actor;
    }
    // splot
    static spActor WireFrame(
                const Grid3& grid, int color_idx = -1) {
            spActor actor = spActor(new Gnuplot_actor());
            actor->command() = "using 1:2:3:4 title \"\" ";
            actor->style()   = "with lines lc variable";
            int c = (color_idx == -1) ? 0 : color_idx;

            short order1[] = { 0, 1, 3, 2, 0 }; //xm
            short order2[] = { 4, 5, 7, 6, 4 }; //xp
            short order3[] = { 0, 4, 5, 1, 0 }; //ym
            short order4[] = { 2, 3, 7, 6, 2 }; //ym
            for (St k = 0; k < grid.n(_Z_); k++) {
            for (St j = 0; j < grid.n(_Y_); j++) {
            for (St i = 0; i < grid.n(_X_); i++) {
                typename Grid3::Index index(i, j, k);
                for (short o = 0; o < 5; ++o) {
                    typename Grid3::Poi p = grid.v(order1[o], index);
                    actor->data().push_back(
                            ToString(p.value(_X_),
                                     p.value(_Y_),
                                     p.value(_Z_),
                                     c,
                                     " "));
                }
                actor->data().push_back(""); //splot sperated with two lines
                actor->data().push_back("");
                for (short o = 0; o < 5; ++o) {
                    typename Grid3::Poi p = grid.v(order2[o], index);
                    actor->data().push_back(
                            ToString(p.value(_X_),
                                     p.value(_Y_),
                                     p.value(_Z_),
                                     c,
                             " "));
                }
                actor->data().push_back("");
                actor->data().push_back("");
                for (short o = 0; o < 5; ++o) {
                    typename Grid3::Poi p = grid.v(order3[o], index);
                    actor->data().push_back(
                                ToString(p.value(_X_),
                                         p.value(_Y_),
                                         p.value(_Z_),
                                         c,
                                          " "));
                }
                actor->data().push_back("");
                actor->data().push_back("");
                for (short o = 0; o < 5; ++o) {
                    typename Grid3::Poi p = grid.v(order4[o], index);
                    actor->data().push_back(
                                ToString(p.value(_X_),
                                         p.value(_Y_),
                                         p.value(_Z_),
                                         c,
                                           " "));
                }
                actor->data().push_back("");
                actor->data().push_back("");
            }
            }
            }
            return actor;
        }


    static Gnuplot& OrderLabel(Gnuplot& gnu, const Order& order){
        spActor actor = spActor(new Gnuplot_actor());
        auto& grid  = order.grid();
        auto& ghost = order.ghost();
        auto fun = [&actor, &grid, &ghost, &gnu, &order](const Index& idx, St tn){
            auto cp = grid.c(idx);
            auto o  = order.get_order(idx);
            std::string text = ToString(tn) + ", " + ToString(o);
            std::string adds = " center textcolor lt " + ToString(tn + 1);
            gnu.set_label(o+1, text, cp.x(), cp.y(), adds);
        };
        Loop::ForEachIndex(order, fun);
        return gnu;
    }
};


}

#endif
