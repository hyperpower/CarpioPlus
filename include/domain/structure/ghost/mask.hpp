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
	SCellMask_(): _type(_GHOST_){
		_bid.fill(10);
	}

	SCellMask_(const int& id): _type(_GHOST_){
		_bid.fill(id);
	}

	SCellMask_(const Self& self):
		_type(self._type), _bid(self._bid){
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

	void set_boundary_id(int a, int o, int id){
		ASSERT(a < DIM);
		ASSERT(o == _M_ || o == _P_);
		int _IDX[][2] = {{0, 1},{2, 3},{4, 5}};
		_bid[_IDX[a][o]] = id;
	}

	int get_boundary_id(int a, int o) {
		ASSERT(a < DIM);
		ASSERT(o == _M_ || o == _P_);
		int _IDX[][2] = { { 0, 1 }, { 2, 3 }, { 4, 5 } };
		return _bid[_IDX[a][o]];
	}
};

template<St DIM>
class SGhostMask_ : public SGhost_<DIM>{
public:
	typedef	SIndex_<DIM> Index;
	typedef SGrid_<DIM> Grid;
	typedef std::shared_ptr<Grid> spGrid;
	typedef SCellMask_<DIM> CellMask;
	typedef std::shared_ptr<CellMask> spCellMask;

	typedef typename Grid::FunIndex  FunIndex;

	typedef std::function<
			spCellMask(const Index&, const Grid&)
			> FunSetByIndex;

	typedef std::function<
		  //input cell center coordinates (x,y,z)
			spCellMask(const Vt&, const Vt&, const Vt&)
			> FunSetByXYZ;


	typedef MultiArray_<spCellMask, DIM> Mat;
	typedef typename Mat::reference reference;
	typedef typename Mat::const_reference const_reference;

protected:
	spGrid _grid;

	Mat _mat;
public:
	SGhostMask_(spGrid spg): _grid(spg),
		_mat(spg->n(_X_), spg->n(_Y_), spg->n(_Z_)){
		_init_mat();
	}
	virtual ~SGhostMask_(){

	}

	Grid& grid(){
		return *(this->_grid);
	}

	const Grid& grid() const{
		return *(this->_grid);
	}

	spCellMask&
	operator()(const Index& idx){
	    return (_mat.at(idx.i(), idx.j(), idx.k()));
	}
	const spCellMask&
	operator()(const Index& idx) const{
	    return (_mat.at(idx.i(), idx.j(), idx.k()));
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
		if(_mat(index.i(), index.j(), index.k()) != nullptr){
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
		if (is_normal(index)) {
			auto idxs = index.shift(a,o);
			if (this->is_ghost(idxs)) {
				return true;
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
		St ABI[3][2] = { { 0, 1 }, { 2, 3 }, { 4, 5 } };
		Index n = this->_grid->n();
		for (St d = 0; d < DIM; ++d) {
			Idx res = idxg.value(d);
			if (res < 0) {
				return ABI[d][0];
			} else if (res >= n.value(d)) {
				return ABI[d][1];
			}
		}
		auto op   = Opposite(ToOrientation(ori));
		auto spcm = this->operator ()(idxg);
		ASSERT(spcm != nullptr);
		return spcm->get_boundary_id(axe, op);
	};

	void set_mask(FunSetByIndex fun){
		FunIndex funi = [&fun, this](const Index& idx){
			auto& grid = *(this->_grid);
			auto res = fun(idx, grid);
			this->operator ()(idx) = res;
		};
		this->_grid->for_each(funi);
	}

	void set_mask(FunSetByXYZ fun){
		FunIndex funi = [&fun, this](const Index& idx){
			auto& grid = *(this->_grid);
			auto cp  = grid.c(idx);
			auto x   = cp.value(_X_);
			auto y   = cp.value(_Y_);
			auto z   = cp.value(_Z_);
			auto res = fun(x, y, z);
			this->operator ()(idx) = res;
		};
		this->_grid->for_each(funi);
	}

	St size_normal() const{
		SHOULD_NOT_REACH;
	}
protected:
	void _init_mat(){
		for(auto& v : this->_mat){
			v = nullptr;
		}
	}
};


}



#endif
