#ifndef _S_GHOST_REGULAR_HPP_
#define _S_GHOST_REGULAR_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "domain/structure/grid/grid.hpp"
#include "ghost.hpp"

namespace carpio{

// y   ____3___
// ^  |     /4 |
// | 0|    /   |1
// |  |___/____|
// |     5 2
// O-----> x
///
//z


template<St DIM>
class SGhostRegular_ : public SGhost_<DIM>{
public:
	typedef	SIndex_<DIM> Index;
	typedef SGrid_<DIM> Grid;
	typedef std::shared_ptr<Grid> spGrid;

protected:
	spGrid _grid;

public:
	SGhostRegular_(spGrid spg): _grid(spg){

	}
	~SGhostRegular_(){

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
		return false;
	};

	bool is_normal(const Index& index) const{
		return !(is_ghost(index));
	}


	int boundary_id(
				const Index& indexc,
				const Index& indexg,
				const Axes& axe,
			    const Orientation& ori) const{
		// get seg idx in BCID
		St ABI[3][2] = { { 0, 1 }, { 2, 3 }, { 4, 5 } };
		Index n = this->_grid->n();
		for (St d = 0; d < DIM; ++d) {
			Idx res = indexg.value(d);
			if (res < 0) {
				return ABI[d][0];
			} else if (res >= n.value(d)) {
				return ABI[d][1];
			}
		}
		SHOULD_NOT_REACH;
		return 0;
	};

	St size_normal() const{
		return _grid->num_cells();
	}
};


}



#endif
