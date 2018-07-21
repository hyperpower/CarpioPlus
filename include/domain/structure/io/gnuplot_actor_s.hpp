#ifndef _S_GNUPLOT_ACTOR_HPP
#define _S_GNUPLOT_ACTOR_HPP

#include "domain/structure/grid/grid.hpp"
#include "io/gnuplot.hpp"

namespace carpio{

namespace GnuplotActor{

inline spActor WireFrame(
		const SGrid_<2>& grid,
		int   color_idx = -1) {
	typedef carpio::SGrid_<2> Grid;
	spActor actor = spActor(new Gnuplot_actor());
	actor->command() = "using 1:2:3 title \"\" ";
	actor->style()   = "with line lc variable";

	short order[]  = { 0, 1, 3, 2, 6, 4, 5, 7 };

	for (St j = 0; j < grid.n(_Y_); j++) {
		for (St i = 0; i < grid.n(_X_); i++) {
			typename Grid::Index index(i,j);
			for (short o = 0; o < grid.num_vertex(); ++o) {
				typename Grid::Poi p = grid.v(order[o], index);
				if (color_idx >= 0) {
					actor->data().push_back(
							ToString(p.value(_X_),
									 p.value(_Y_), color_idx, " "));
				} else {
					actor->data().push_back(
							ToString(p.value(_X_),
									 p.value(_Y_), 0, " "));
				}
			}
			typename Grid::Poi p = grid.v(0, index);
			if (color_idx >= 0) {
				actor->data().push_back(
						ToString(p.value(_X_), p.value(_Y_), color_idx, " "));
			} else {
				actor->data().push_back(
						ToString(p.value(_X_), p.value(_Y_), 0, " "));
			}
			actor->data().push_back("");
		}
	}

	return actor;
}


}

}

#endif
