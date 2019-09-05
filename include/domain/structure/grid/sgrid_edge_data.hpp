#ifndef _S_GRID_EDGE_DATA_HPP_
#define _S_GRID_EDGE_DATA_HPP_

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/grid/sgrid_face_data.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"

namespace carpio {

template<class NUM, St DIM>
class SGridEdgeData_: public SGridFaceData_<NUM, DIM>{
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
    typedef SGridEdgeData_<NUM, DIM> Self;
    typedef SGridFaceData_<NUM, DIM> Base;
protected:
    // Mat _mat[DIM];
public:
    SGridEdgeData_(spGrid spg) : Base(spg){
    }
    //

    Vt& operator()(
    		St af, St of,  // which face
    		St a,  St o,  // which edge on this edge
			St i, St j = 0, St k = 0) {
		ASSERT(DIM == 3);
		ASSERT(a != af);
		St index[] = { i, j, k };
		if(o == _P_){
			index[a] += 1;
		}
		if (of == _P_) {
			index[af] += 1;
		}
		auto an = Normal(af, a);
		this->_mat[an](index[0], index[1], index[2]);
	}


	const Vt& operator()(
			St af, St of,  // which face
			St a, St o,  // which edge on this edge
			St i, St j = 0, St k = 0) const {
		ASSERT(DIM == 3);
		ASSERT(a != af);
		St index[] = { i, j, k };
		if(o == _P_){
			index[a] += 1;
		}
		if (of == _P_) {
			index[af] += 1;
		}
		auto an = Normal(af, a);
		this->_mat[an](index[0], index[1], index[2]);
	}

	Vt& operator()(St af, St of, St a, St o, const Index& index) {
		return this->operator ()(af, of, a, o, index.i(), index.j(), index.k());
	}

	const Vt& operator()(St af, St of, St a, St o, const Index& index) const {
		return this->operator ()(af, of, a, o, index.i(), index.j(), index.k());
	}

};

}

#endif
