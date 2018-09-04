#ifndef _S_PLOTLY_ACTOR_HPP
#define _S_PLOTLY_ACTOR_HPP

#include "io/plotly.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/field/sscalar.hpp"
namespace carpio{
namespace PlotlyActor{

template<St DIM>
spPA_scatter3d WireFrame(const SGrid_<DIM>& grid) {
	typedef SGrid_<DIM> Grid;
	typedef std::list<double> Listd;
	typedef typename Grid::size_type St;
	Listd lx;
	Listd ly;
	Listd lz;
	short order[]  = { 0, 1, 3, 2, 6, 4, 5, 7 };
	short order3[] = { 0, 1, 1, 3, 3, 2, 2, 0, 2, 6, 3, 7, 1, 5, 0, 4, 4, 5, 5,
			7, 7, 6, 6, 4 };
	for (St k = 0; k < ((DIM==3) ? grid.n(_Z_) : 1); k++) {
		for (St j = 0; j < grid.n(_Y_); j++) {
			for (St i = 0; i < grid.n(_X_); i++) {
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

template<St DIM>
spPA_scatter ScalarCenter(const SScalar_<DIM>& v, St dim = 2, Idx i = 0) {
	typedef SGrid_<DIM> Grid;
	typedef SScalar_<DIM> Scalar;
	typedef std::list<double> Listd;
	Listd lx, ly, lz, lv;
	for(auto& index : v.order()){
		auto p = v.grid().c(index);
		lx.push_back(p.value(0));
		ly.push_back(p.value(1));
		lz.push_back(p.value(2));
		lv.push_back(v(index));
	}
	spPA_scatter res;
	if (DIM == 3) {
		Listd* al[] = { &lx, &ly, &lz };
		St dim1, dim2;
		NormalPlane(dim, dim1, dim2);
		res = spPA_scatter(
				new carpio::Plotly_actor_scatter(*(al[dim1]), *(al[dim2])));
		res->set_colorscale(lv);
		res->set_add_val(lv);
		res->set_mode("markers");
	} else if (DIM == 2) {
		res = spPA_scatter(new carpio::Plotly_actor_scatter(lx, ly));
		res->set_colorscale(lv);
		res->set_add_val(lv);
		res->set_mode("markers");
	} else { // DIM == 1
		res = spPA_scatter(new carpio::Plotly_actor_scatter(lx, lv));
		res->set_mode("lines + markers");
	}
	return res;
}

template<St DIM>
spPA_scatter3d ScalarCenter3d(const SScalar_<DIM>& v, St dim = 2, Idx i = 0) {
	typedef SGrid_<DIM> Grid;
	typedef SScalar_<DIM> Scalar;
	typedef std::list<double> Listd;
	Listd lx, ly, lz, lv;
	for(auto& index : v.order()){
		typename Grid::Poi p = v.grid().c(index);
		lx.push_back(p.value(0));
		ly.push_back(p.value(1));
		lz.push_back(p.value(2));
		lv.push_back(v(index));
	}
	spPA_scatter3d res;
	if (DIM == 3) {
		Listd* al[] = { &lx, &ly, &lz };
		St dim1, dim2;
		NormalPlane(dim, dim1, dim2);
		res = spPA_scatter3d(
				new carpio::Plotly_actor_scatter3d(*(al[dim1]), *(al[dim2]),
						lv));
	} else {
		res = spPA_scatter3d(new carpio::Plotly_actor_scatter3d(lx, ly, lv));
	}
	res->set_colorscale(lv, 7);
	res->set_add_val(lv);
	res->set_mode("markers");
	return res;
}

template<St DIM>
spPA_heatmap Heatmap( //
		const SGrid_<DIM>& grid, //
		const SScalar_<DIM>& csfield, //
		St dim1 = _X_, St dim2 = _Y_, Idx idx = 0) {
	typedef SGrid_<DIM> Grid;
	typedef SIndex_<DIM> Index;
	typedef std::list<double> Listd;
	Listd lx, ly, lz;
	lx.push_back(grid.f_(dim1, _M_, 0));
	for (Idx i = 0; i < grid.n(dim1); i++) {
		Vt f = grid.f_(dim1, _P_, i);
		lx.push_back(f);
	}
	ly.push_back(grid.f_(dim2, _M_, 0));
	for (Idx j = 0; j < grid.n(dim2); j++) {
		Vt f = grid.f_(dim2, _P_, j);
		ly.push_back(f);
	}

	for (Idx j = 0; j < grid.n(dim2); j++) {
		for (Idx i = 0; i < grid.n(dim1); i++) {
			Index index;
			index.set(dim1, i);
			index.set(dim2, j);
			index.set(Normal(dim1, dim2), idx);
			//
			lz.push_back(csfield(index));
		}
	}

	spPA_heatmap res;
	res = spPA_heatmap(new carpio::Plotly_actor_heatmap(lx, ly, lz));
	return res;
}




}
}

#endif
