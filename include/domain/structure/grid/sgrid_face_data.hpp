#ifndef _S_GRID_CENTER_DATA_HPP_
#define _S_GRID_CENTER_DATA_HPP_

#include "domain/structure/grid/sgrid.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"

namespace carpio {

// ----x----|----x----|----x----|
//    i-1        i        i+1       center index
//          i        i+1       i+2  face   index

template<class TYPE, St DIM>
class SGridFaceData_{
public:
	static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
	static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);

    typedef SIndex_<DIM> Index;
    typedef SGrid_<DIM>  Grid;

    typedef std::shared_ptr<SIndex_<DIM> > spIndex;
    typedef std::shared_ptr<SGrid_<DIM>  > spGrid;

    typedef MultiArray_<TYPE, DIM> Mat;
    typedef typename Mat::reference reference;
    typedef typename Mat::const_reference const_reference;
    typedef SGridFaceData_<DIM> Self;
protected:
    spGrid  _grid;

    Mat _mat[DIM];

public:
    SGridFaceData_(spGrid g) : _grid(g){
    }

};

}
