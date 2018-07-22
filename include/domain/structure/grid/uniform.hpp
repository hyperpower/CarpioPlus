#ifndef _S_SGRIDUNIFORM_HPP
#define _S_SGRIDUNIFORM_HPP

#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "sgrid.hpp"
#include <array>

namespace carpio{

template<St DIM>
class SGridUniform_ :public SGrid_<DIM>{
public:
	static const St Dim = DIM;
	typedef ArrayListV<double> Arr;
	typedef Point_<double, Dim> Poi;
	typedef SIndex_<Dim> Index;
	static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
	static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
protected:
	Poi   _min;
	Index _n, _ng;    // number of node and number of node and ghost
	St    _gl;        // ghost layer
	Vt    _cs;        // cell size
	Arr   _c[Dim];    // coordinate center
public:
	SGridUniform_(const Poi&   minp,
			      const Index& n,
				  const Vt&    cellsize,
				  const St&    ghostlayer):
			_min(minp), _n(n), _cs(cellsize), _gl(ghostlayer){
		for (St i = 0; i < Dim; ++i) {
			_ng[i] = _n[i] + 2 * _gl;
		}
		for (St d = 0; d < Dim; ++d) {
			_c[d].reconstruct(_ng[d]);
			Vt orignal = _cs * (0.5 - _gl);
			for (typename Arr::size_type ii = 0; ii < _c[d].size(); ++ii) {
				_c[d][ii] = orignal + ii * _cs;
			}
		}
	}


	SGridUniform_(
			const Poi& minp,
			const Idx& nx,
			const Vt&  length_on_x,
			const St&  ghostlayer) :
			_min(minp), _gl(ghostlayer) {
		_cs = length_on_x / nx;
		for (St i = 0; i < Dim; ++i) {
			_n[i]  = nx;
			_ng[i] = _n[i] + 2 * _gl;
		}
		for (St d = 0; d < Dim; ++d) {
			_c[d].reconstruct(_ng[d]);
			Vt orignal = _cs * (0.5 - _gl);
			for (typename Arr::size_type ii = 0; ii < _c[d].size(); ++ii) {
				_c[d][ii] = orignal + ii * _cs;
			}
		}
	}

	// index ==================================
	const Index& n() const{
		return _n;
	};
	const Index& N() const{
		return _ng;
	};
	// The number of the nodes on each direction are same
	Idx n(St dim) const{
		return dim < Dim ? _n[dim] : 0;
	};

	Idx N(St dim) const{
		return dim < Dim ? _ng[dim] : 0;
	};

	// size ===================================
	// The size for each cells are same
	inline Vt s_(St dim, Idx i) const{
		return _cs;
	};
	inline Vt s_(St dim, Index idx) const{
		return _cs;
	};

	// half size
	inline Vt hs_(St dim, Idx i) const {
		return _cs * 0.5;
	};
	inline Vt hs_(St dim, Index idx) const {
		return _cs * 0.5;
	};
	// volume ==================================
	inline Vt volume(Index idx) const {
		Vt res = 1.0;
		FOR_EACH_DIM
		{
			res *= _cs;
		}
		return res;
	};
	// center ==================================
	Poi c(Idx i, Idx j = 0, Idx k = 0) const {
		Poi res;
		Idx ai[] = { i, j, k };
		for (St d = 0; d < Dim; ++d) {
			res[d] = _c[d][_IDX(ai[d])];
		}
		return std::move(res);
	}
	Poi c(const Index& index) const {
		return c(index.i(), index.j(), index.k());
	}
	Vt  c_(const St& dim, const Idx& idx) const {
		return (dim < Dim) ? _c[dim][_IDX(idx)] : 0;
	}
	Vt  c_(const St& dim, const Index& idx) const {
		return (dim < Dim) ? _c[dim][_IDX(idx[dim])] : 0;
	}

	Vt cx(Idx i, Idx j = 0, Idx k = 0) const {
		return _c[0][_IDX(i)];
	}
	Vt cy(Idx i, Idx j = 0, Idx k = 0) const {
		ASSERT(Dim >= 2);
		return _c[1][_IDX(j)];
	}
	Vt cz(Idx i, Idx j = 0, Idx k = 0) const {
		ASSERT(Dim >= 3);
		return _c[2][_IDX(k)];
	}

	St num_cells() const {
		St res = 1;
		for (St d = 0; d < Dim; ++d) {
			res *= _n[d];
		}
		return res;
	}
	St num_vertex() const {
		return NumVertex;
	}
	St num_face() const {
		return NumFace;
	}

	Vt min_size() const {
		return _cs;
	}
	Vt min_size(St dim) const {
		return _cs;
	}

	// face  ===================================
	Poi f(St dim, int fb, const Index& index) const {
		return f(dim, fb, index.i(), index.j(), index.k());
	}

	Poi f(St dim, int fb, Idx i, Idx j = 0, Idx k = 0) const {
		Poi pc = c(i, j, k);
		Vt halfs = _cs * 0.5;
		if (fb == _P_) { //right face
			pc[dim] = pc[dim] + halfs;
		} else if (fb == _M_){  //left face
			pc[dim] = pc[dim] - halfs;
		} else{
			SHOULD_NOT_REACH;
		}
		return pc;
	}

	Vt f_(St dim, int ori, Idx idx) const {
		Vt halfs = hs_(dim, idx);
		Vt cen = c_(dim, idx);
		if (ori == _P_) {         //right face
			return cen + halfs;
		} else if (ori == _M_) {  //left face
			return cen - halfs;
		} else {
			SHOULD_NOT_REACH;
			return 0;
		}
	}

	// distance to face
	Vt df_(St dim, Idx idx) const {
		Vt halfs = hs_(dim, idx);
		Vt cen   = c_(dim, idx);
		return std::abs(halfs - cen);
	}

	// vertex ================================
	Poi v(Idx order, Idx i, Idx j = 0, Idx k = 0) const {
		static const short VERTEX_IDX[][3] = {
					//     x    y    z
					{ _M_, _M_, _M_ }, //
					{ _P_, _M_, _M_ }, //
					{ _M_, _P_, _M_ }, //
					{ _P_, _P_, _M_ }, //
					{ _M_, _M_, _P_ }, //
					{ _P_, _M_, _P_ }, //
					{ _M_, _P_, _P_ }, //
					{ _P_, _P_, _P_ }  //
		};
		return v(i, VERTEX_IDX[order][0],
				 j, VERTEX_IDX[order][1],
				 k, VERTEX_IDX[order][2]);
	}
	Poi v(Idx order, Index index) const{
		return v(order, index.i(), index.j(), index.k());
	}


	Poi v(Idx i,     short oi,
		  Idx j = 0, short oj = 0,
		  Idx k = 0, short ok = 0) const {
		Poi res;
		Idx ai[] = { i, j, k };
		short ao[] = { oi, oj, ok };
		for (St d = 0; d < Dim; ++d) {
			Vt cen = c_(d, ai[d]);
			Vt halfs = hs_(d, ai[d]);
			if (ao[d] == _P_) {
				res[d] = cen + halfs;
			} else if (ao[d] == _M_) {
				res[d] = cen - halfs;
			} else {
				res[d] = cen;
			}
		}
		return res;
	}

	Index to_Index(const Index& INDEX) const{
		Index c(INDEX.value(0) - _gl, INDEX.value(1) - _gl, INDEX.value(2) - _gl);
		return c;
	}

	// Point is in the range
	inline bool is_in_on(Poi p) {
		bool res = true;
		FOR_EACH_DIM
		{
			Vt min = this->_min[d];
			Vt max = _cs * _n[d] + this->_min[d];
			Vt val = p[d];
			res = res && IsInRange(min, val, max, _cc_);
		}
		return res;
	}

	//  find closest index on the negative direction
	//  for example:
	//
	//      1.2   1.4   1.6   1.8   2.0
	//  -----+-----+-----+-----+-----+
	//       0     1     2     3     4
	//                      ^
	//               cor = 1.5
	//  return 2
	inline Idx find_close_idx_m(St dim, Vt cor) {
		ASSERT(dim < Dim);
		St res = 0;
		// this->_c is a order array
		for (St i = 0; i < this->_c[dim].size(); i++) {
			if (this->_c[dim][i] > cor) {
				res = i;
				break;
			}
		}
		res = (res == 0) ? 0 : res - 1;
		return _idx(res);
	}
	//  find closest index on the positive direction
	//  for example:
	//
	//      1.2   1.4   1.6   1.8   2.0
	//  -----+-----+-----+-----+-----+
	//       0     1     2     3     4
	//                      ^
	//               cor = 1.5
	//  return 3
	inline Idx find_close_idx_p(St dim, Vt cor) {
		ASSERT(dim < Dim);
		St res = 0;
		// this->_c is a order array
		for (St i = 0; i < this->_c[dim].size(); i++) {
			if (this->_c[dim][i] > cor) {
				res = i;
				break;
			}
		}
		return _idx(res);
	}

	inline St _IDX(const Idx& i) const {
		return St(i + _gl);
	}

	inline Idx _idx(const St& I) const {
		return Idx(I - _gl);
	}

};



}

#endif
