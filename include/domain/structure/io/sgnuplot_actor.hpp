#ifndef _S_GNUPLOT_ACTOR_HPP
#define _S_GNUPLOT_ACTOR_HPP

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/field/sfield.hpp"
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
	typedef SField_<DIM>  Field;
	typedef SField_<1>    Field1;
	typedef SField_<2>    Field2;
	typedef SField_<3>    Field3;

	typedef SIndex_<1>    Index1;
	typedef SIndex_<2>    Index2;
	typedef SIndex_<3>    Index3;

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
					typename Grid::Poi p = grid.v(order[o], index);
					actor->data().push_back(
								ToString(p.value(_X_),
										 p.value(_Y_),
										 c,
										 " "));
				}
				typename Grid2::Poi p = grid.v(0, index);
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

	static spActor Contour(const Field2& f){
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3:4:5:6:7 title \"\" ";
		actor->style()   = "with boxxy fs solid palette";
		for (St i = 0; i < f.grid().n(_X_); i++) {
			for (St j = 0; j < f.grid().n(_Y_); j++) {
				Index2 index(i, j);
				auto pc   = f.grid().c(index);
				auto pmin = f.grid().v(0, index);
				auto pmax = f.grid().v(3, index);
				actor->data().push_back(
						ToString(pc(_X_),   pc(_Y_),
								 pmin(_X_), pmax(_X_),
								 pmin(_Y_), pmax(_Y_), f(index), " "));
			}
		}
		return actor;
	}

	static spActor WireFrame(
				const Grid1& grid, int color_idx = -1) {
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
					ToString(p.value(_X_), -0.1, c, " "));
			actor->data().push_back(
					ToString(p.value(_X_),  0.1, c, " "));
			actor->data().push_back("");
			if(i == 0){
				xs = p.value(_X_);
			}

		}
		typename Grid1::Index index(grid.n(_X_) - 1);
		auto p = grid.f(_X_, _P_, index);
		actor->data().push_back(
				ToString(p.value(_X_), -0.1, c, " "));
		actor->data().push_back(
				ToString(p.value(_X_),  0.1, c, " "));
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

	static spActor WireFrame(
				const Grid3& grid, int color_idx = -1) {
			spActor actor = spActor(new Gnuplot_actor());
			actor->command() = "using 1:2:3:4 title \"\" ";
			actor->style()   = "with lines lc variable";
			int c = (color_idx == -1) ? 0 : color_idx;

			short order1[] = { 0, 1, 3, 2, 0 };
			short order2[] = { 4, 5, 7, 6, 4 };
			for (St k = 0; k < grid.n(_Z_); k++) {
			for (St j = 0; j < grid.n(_Y_); j++) {
			for (St i = 0; i < grid.n(_X_); i++) {
				typename Grid3::Index index(i, j);
				for (short o = 0; o < 5; ++o) {
					typename Grid3::Poi p = grid.v(order1[o], index);
					actor->data().push_back(
							ToString(p.value(_X_),
									 p.value(_Y_),
									 p.value(_Z_),
									 c,
								     " "));
				}
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
			}
			actor->data().push_back("");
			}
			actor->data().push_back("");
			}
			return actor;
		}
};


}

#endif
