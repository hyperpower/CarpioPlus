#ifndef _S_ORDER_XYZ_HPP_
#define _S_ORDER_XYZ_HPP_


#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "domain/structure/grid/grid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "order.hpp"
#include "algebra/array/multi_array.hpp"

namespace carpio{

template<St DIM>
class SOrderXYZ_ : public SOrder_<DIM>{
public:
	typedef SIndex_<DIM> Index;
	typedef SGrid_<DIM> Grid;
	typedef std::shared_ptr<Grid> spGrid;
	typedef SGhost_<DIM> Ghost;
	typedef std::shared_ptr<Ghost> spGhost;

	typedef std::vector<Index> OrderVector;
	typedef typename OrderVector::iterator iterator;
	typedef typename OrderVector::const_iterator const_iterator;

	typedef MultiArray_<St, DIM> OrderMatrix;
protected:
	spGrid _grid;
	spGhost _ghost;

	OrderVector _ov;  // order vector
	OrderMatrix _om;  // order matrix

public:
	SOrderXYZ_(spGrid spgrid, spGhost spghost):
		_grid(spgrid), _ghost(spghost), _ov(_ghost->size_normal()),
		_om(_grid->n(_X_), _grid->n(_Y_), _grid->n(_Z_)){
		_build_ov();
		_build_om();
	}

	iterator begin(){
		return _ov.begin();
	}

	iterator end(){
		return _ov.end();
	}

	const_iterator begin() const{
			return _ov.begin();
	}

	const_iterator end() const{
		return _ov.end();
	}

	St get_order(const Index& index) const{
		if(!(_ghost->is_ghost(index))){
			return _om(index.i(), index.j(), index.k());
		}
		std::cerr<< ">! Error Index = " << index << std::endl;
		SHOULD_NOT_REACH;
		return 0;
	}
protected:
	void _build_ov() {
		St count = 0;
		for (St k = 0; k < ((DIM == 3) ? _grid->n(_Z_) : 1); k++) {
			for (St j = 0; j < ((DIM >= 2) ? _grid->n(_Y_) : 1); j++) {
				for (St i = 0; i < _grid->n(_X_); i++) {
					Index index(i,j,k);
					if(!(_ghost->is_ghost(index))){
						_ov[count] = index;
						count++;
					}
				}
			}
		}
	}

	void _build_om() {
		// call after _build_ov
		for(typename OrderVector::size_type ii = 0; ii < _ov.size(); ++ii ){
			_om(_ov[ii].i(), _ov[ii].j(), _ov[ii].k()) = ii;
		}
	}



};


}



#endif
