#ifndef _S_GHOST_MASK_HPP_
#define _S_GHOST_MASK_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "ghost.hpp"

namespace carpio{

// A additional matrix used to record mask value
// mask value    cell type
//    0          normal
//
template<St DIM>
class SCellMask_{
protected:
	typedef SCellMask_<DIM> Self;
public:
	static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
	static const St NumFace = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
protected:
	int _type;
	std::array<int,NumFace> _bid;
public:
	SCellMask_(): _type(_NORMAL_){
		_bid.fill(_NORMAL_);
	}

	SCellMask_(const int& t): _type(t){
		_bid.fill(_NORMAL_);
	}

	SCellMask_(const Self& self):_type(self._type){
		_bid = self._bid;
	}

	Self& operator=(const Self& o) {
		if (this == &o) {
			return *this;
		}
		_type = o._type;
		_bid = o._bid;
		return *this;
	}

	const int& type() const{
		return this->_type;
	}
	int& type(){
		return this->_type;
	}

	void set_boundary_id(Axes a, Orientation o, int id){
		ASSERT(a < DIM);
		ASSERT(o == _M_ || o == _P_);
		int _IDX[][2] = {{0, 1},{2, 3},{4, 5}};
		_bid[_IDX[int(a)][int(o)]] = id;
	}

	int get_boundary_id(Axes a, Orientation o) {
		ASSERT(a < DIM);
		ASSERT(o == _M_ || o == _P_);
		int _IDX[][2] = { { 0, 1 }, { 2, 3 }, { 4, 5 } };
		return _bid[_IDX[int(a)][int(o)]];
	}
};

template<St DIM>
class SGhostMask_ : public SGhost_<DIM>{
public:
	typedef	SIndex_<DIM> Index;
	typedef SGrid_<DIM> Grid;
	typedef std::shared_ptr<Grid> spGrid;

	typedef SCellMask_<DIM> CellMask;

	typedef MultiArray_<CellMask, DIM> Mat;
	typedef typename Mat::reference reference;
	typedef typename Mat::const_reference const_reference;

protected:
	spGrid _grid;

	Mat _mat;
public:
	SGhostMask_(spGrid spg): _grid(spg),
		_mat(spg->n(_X_), spg->n(_Y_), spg->n(_Z_)){
	}
	~SGhostMask_(){

	}

	Grid& grid(){
		return _grid;
	}

	const Grid& grid() const{
		return _grid;
	}

	bool is_ghost(const Index& index) const{
		for (St d = 0; d < DIM; ++d) {
			Idx res = index.value(d);
			if (res < 0) {
				return true;
			} else if (res >= this->_grid->n().value(d)) {
				return true;
			}
		}
		if(_mat(index.i(), index.j(), index.k()).type() != _NORMAL_){
			return true;
		}
		return false;
	};

	bool is_boundary(
				const Index& index,
				const St& a,
				const St& o) const{
		ASSERT(a < DIM);
		Idx idx = index.value(a);
		// index should be in normal range
		ASSERT(idx >= 0 && idx < _grid->n(a));

		auto& mask = _mat(index.i(), index.j(), index.k()); // cell mask
		if(mask.type() == _GHOST_){
			return false;
		}else if(mask.type() == _CUT_){
			return true;
		}else{ // NORMAL
			FOR_EACH_DIM{
				auto idxm = index.m(d);
				if(this->is_ghost(idxm)){
					return true;
				}
				auto idxp = index.p(d);
				if(this->is_ghost(idxp)){
					return true;
				}
			}
		}
		return false;
	}

	bool is_normal(const Index& index) const{
		return !(is_ghost(index));
	}


	int boundary_id(
				const Index& idxc,
				const Index& idxg,
				const St& axe,
			    const St& ori) const{

	};

	St size_normal() const{

	}
};


}



#endif
