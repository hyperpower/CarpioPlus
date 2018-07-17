#ifndef _S_VECTOR_FACE_HPP_
#define _S_VECTOR_FACE_HPP_

#include "domain/structure/grid/grid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "scalar.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"

namespace carpio{

// ----x----|----x----|----x----|
//    i-1        i        i+1       center index
//          i        i+1       i+2  face   index

template<St DIM>
class SVectorFace_{
public:
	typedef SIndex_<DIM> Index;
	typedef SGrid_<DIM>  Grid;
	typedef SGhost_<DIM> Ghost;
	typedef SOrder_<DIM> Order;

	typedef std::shared_ptr<SIndex_<DIM> > spIndex;
	typedef std::shared_ptr<SGrid_<DIM>  > spGrid;
	typedef std::shared_ptr<SGhost_<DIM> > spGhost;
	typedef std::shared_ptr<SOrder_<DIM> > spOrder;

	typedef MultiArrayV_<Vt, DIM> Mat;
	typedef typename Mat::reference reference;
	typedef typename Mat::const_reference const_reference;
	typedef SVectorFace_<DIM> Self;
protected:
	spGrid  _grid;
	spGhost _ghost;
	spOrder _order;

	Mat _mat[DIM];
public:
	SVectorFace_(spGrid spg, spGhost spgh, spOrder spo):
		_grid(spg), _ghost(spgh), _order(spo){
		for(St i = 0; i< DIM; ++i){
			_mat[i].reconstruct(spg->n(_X_) + 1,
					            spg->n(_Y_) + 1,
								spg->n(_Z_) + 1);
		}
	}

	Vt& operator()(Axes a, Orientation o,
			       St i, St j = 0, St k = 0) {
		ASSERT(a < DIM);
		St index[] = { i, j, k };
		if (o == _M_) {
			return _mat[a](i, j, k);
		} else if (o == _P_) {
			index[a] += 1;
			return _mat[a](index[0], index[1], index[2]);
		} else {
			SHOULD_NOT_REACH;
			return 0;
		}
	}

	const Vt& operator()(Axes a, Orientation o,
			       St i, St j = 0, St k = 0) const {
		ASSERT(a < DIM);
		St index[] = { i, j, k };
		if (o == _M_) {
			return _mat[a](i, j, k);
		} else if (o == _P_) {
			index[a] += 1;
			return _mat[a](index[0], index[1], index[2]);
		} else {
			SHOULD_NOT_REACH;
			return 0;
		}
	}

	Vt& operator()(Axes a, Orientation o, const Index& index){
		return this->operator ()(a, o, index.i(), index.j(), index.k());
	}

	const Vt& operator()(Axes a, Orientation o, const Index& index) const{
		return this->operator ()(a, o, index.i(), index.j(), index.k());
	}

};



}

#endif
