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
	typedef SGhost_<DIM>  Ghost;
	typedef SGhost_<1>    Ghost1;
	typedef SGhost_<2>    Ghost2;
	typedef SGhost_<3>    Ghost3;
	typedef SField_<DIM> Field;
	typedef SField_<1>   Field1;
	typedef SField_<2>   Field2;

	static spActor WireFrame(
			const Grid2& grid, int color_idx = -1) {
		spActor actor = spActor(new Gnuplot_actor());
		actor->command() = "using 1:2:3 title \"\" ";
		actor->style()   = "with line lc variable";

		short order[] = { 0, 1, 3, 2, 6, 4, 5, 7 };

		for (St j = 0; j < grid.n(_Y_); j++) {
			for (St i = 0; i < grid.n(_X_); i++) {
				typename Grid2::Index index(i, j);
				for (short o = 0; o < grid.num_vertex(); ++o) {
					typename Grid::Poi p = grid.v(order[o], index);
					if (color_idx >= 0) {
						actor->data().push_back(
								ToString(p.value(_X_),
										 p.value(_Y_),
										 color_idx,
										 " "));
					} else {
						actor->data().push_back(
								ToString(p.value(_X_),
										 p.value(_Y_),
										 0, " "));
					}
				}
				typename Grid2::Poi p = grid.v(0, index);
				if (color_idx >= 0) {
					actor->data().push_back(
							ToString(p.value(_X_), p.value(_Y_), color_idx,
									" "));
				} else {
					actor->data().push_back(
							ToString(p.value(_X_), p.value(_Y_), 0, " "));
				}
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
};


}

#endif
