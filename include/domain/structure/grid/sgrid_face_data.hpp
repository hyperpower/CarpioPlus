#ifndef _S_GRID_FACE_DATA_HPP_
#define _S_GRID_FACE_DATA_HPP_

#include "domain/structure/grid/sgrid.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"

namespace carpio {

// ----x----|----x----|----x----|
//    i-1        i        i+1       center index
//          i        i+1       i+2  face   index

template<class NUM, St DIM>
class SGridFaceData_{
public:
	static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
	static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
	static const St NumEdge   = DIM == 1 ? 1 : (DIM == 2 ? 4 : 12);

    typedef SIndex_<DIM> Index;
    typedef SGrid_<DIM>  Grid;

    typedef std::shared_ptr<SIndex_<DIM> > spIndex;
    typedef std::shared_ptr<SGrid_<DIM>  > spGrid;

    typedef NUM Vt;

    typedef MultiArrayV_<Vt, DIM> Mat;
    typedef typename Mat::reference reference;
    typedef typename Mat::const_reference const_reference;
    typedef SGridFaceData_<NUM, DIM> Self;
protected:
    spGrid  _grid;

    Mat _mat[DIM];

public:
    SGridFaceData_(spGrid g) : _grid(g){
    }

	Vt& operator()(St a,  // Axes
			       St o,  // Orientation
			       St i, St j = 0, St k = 0) {
		ASSERT(a < DIM);
		if (o == _M_) {
			return _mat[a](i, j, k);
		} else if (o == _P_) {
			St index[] = { i, j, k };
			index[a] += 1;
			return _mat[a](index[0], index[1], index[2]);
		} else {
			SHOULD_NOT_REACH;
		}
	}

    const Vt& operator()(
    		St a,  // Axes
    		St o,  // Orientation
			St i, St j = 0, St k = 0) const {
		ASSERT(a < DIM);
		if (o == _M_) {
			return _mat[a](i, j, k);
		} else if (o == _P_) {
			St index[] = { i, j, k };
			index[a] += 1;
			return _mat[a](index[0], index[1], index[2]);
		} else {
			SHOULD_NOT_REACH;
		}
	}

    Vt& operator()(St a, St o, const Index& index){
        return this->operator ()(a, o, index.i(), index.j(), index.k());
    }

    const Vt& operator()(St a, St o, const Index& index) const{
        return this->operator ()(a, o, index.i(), index.j(), index.k());
    }

    const Grid& grid() const {
        return (*_grid);
    }

    spGrid spgrid() const {
        return _grid;
    }


};

}

#endif
