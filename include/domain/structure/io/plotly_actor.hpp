#ifndef _S_PLOTLY_ACTOR_HPP
#define _S_PLOTLY_ACTOR_HPP

#include "io/plotly.hpp"
#include "domain/structure/grid/grid.hpp"

namespace PlotlyActor{

template<carpio::St DIM>
spPA_scatter3d WireFrame(const carpio::SGrid_<DIM>& grid) {
	typedef carpio::SGrid_<DIM> Grid;
	typedef std::list<double> Listd;
	typedef typename Grid::size_type St;
	Listd lx;
	Listd ly;
	Listd lz;
	short order[]  = { 0, 1, 3, 2, 6, 4, 5, 7 };
	short order3[] = { 0, 1, 1, 3, 3, 2, 2, 0, 2, 6, 3, 7, 1, 5, 0, 4, 4, 5, 5,
			7, 7, 6, 6, 4 };
	for (St k = 0; k < ((DIM==3) ? grid.n(carpio::_Z_) : 1); k++) {
		for (St j = 0; j < grid.n(carpio::_Y_); j++) {
			for (St i = 0; i < grid.n(carpio::_X_); i++) {
				typename Grid::Index index(i,j,k);
				if (DIM == 2) {
					for (short o = 0; o < grid.num_vertex(); ++o) {
						typename Grid::Poi p = grid.v(order[o], index);
						lx.push_back(p.value(0));
						ly.push_back(p.value(1));
						lz.push_back(p.value(2));
					}
					typename Grid::Poi p = grid.v(0, index);
					lx.push_back(p.value(0));
					ly.push_back(p.value(1));
					lz.push_back(p.value(2));
				}
				if (DIM == 3) {
					for (short i = 0; i < 23; i += 2) {
						typename Grid::Poi p = grid.v(order3[i], index);
						lx.push_back(p.value(0));
						ly.push_back(p.value(1));
						lz.push_back(p.value(2));
						typename Grid::Poi pp = grid.v(order3[i + 1], index);
						lx.push_back(pp.value(0));
						ly.push_back(pp.value(1));
						lz.push_back(pp.value(2));
					}
				}
			}
		}
	}
	spPA_scatter3d res;
	if (DIM == 2) {
		res = spPA_scatter3d(new carpio::Plotly_actor_scatter3d(lx, ly, lz, 5));
	} else if (DIM == 3) {
		res = spPA_scatter3d(new carpio::Plotly_actor_scatter3d(lx, ly, lz, 2));
	}
	res->set_mode("lines");
	return res;
}

}

#endif
