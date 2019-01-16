#ifndef _S_CORNER_HPP_
#define _S_CORNER_HPP_

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "sfield.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"
#include "algebra/matrix/matrix_small.hpp"

namespace carpio{

// ----x----|----x----|----x----|
//    i-1        i        i+1       center index
//          i        i+1       i+2  corner index
//
//         1         3
// --------o---------o---------|
//         |         |
//         |         |
//         |    x    |
//         |    i    |
//         |         |
// --------o---------o---------
//         0         2
//         i
// 1D          di dj dk
//  x
// _M_  = 0,   0  0  0
// _P_  = 1,   1  0  0
// 2D
//  yx
// _MM_ = 0,   0  0  0
// _MP_ = 1,   1  0  0
// _PM_ = 2,   0  1  0
// _PP_ = 3,   1  1  0
// 3D
//  zyx
// _MMM_ = 0,  0  0  0
// _MMP_ = 1,  1  0  0
// _MPM_ = 2,  0  1  0
// _MPP_ = 3,  1  1  0
// _PMM_ = 4,  0  0  1
// _PMP_ = 5,  1  0  1
// _PPM_ = 6,  0  1  1
// _PPP_ = 7,  1  1  1
template<St DIM>
class SCorner_{
public:
	static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
	static const St NumFace = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);

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
	typedef SCorner_<DIM> Self;
protected:
	spGrid  _grid;
	spGhost _ghost;
	spOrder _order;

	Mat _mat;
	
	MatrixS_<St, 8, 3> _didx;  //delta index

public:
	SCorner_(spGrid spg, spGhost spgh, spOrder spo):
		_grid(spg), _ghost(spgh), _order(spo) {
		_mat.reconstruct(spg->n(_X_) + 1,
	             		 spg->n(_Y_) + 1,
			             spg->n(_Z_) + 1);
		initial_didx();
	}

	Vt& operator()(St o,
				   St i, St j = 0, St k = 0) {
		St index[] = { i, j, k };
		FOR_EACH_DIM{
			auto didx = _didx(o, d);
			index[d] += didx;
		}
//		std::cout<<"op () idx = " << ToString(index[0], index[1],index[2], " ") << std::endl;
		return _mat(index[0], index[1], index[2]);
	}

	Vt& operator()(St o,
			const Index& index) {
		return this->operator ()(o, index.i(), index.j(), index.k());
	}

	const Vt& operator()(St o,
			             St i, St j = 0, St k = 0) const{
		St index[] = { i, j, k };
		FOR_EACH_DIM
		{
			auto didx = _didx(o, d);
			index[d] += didx;
		}
		return _mat(index[0], index[1], index[2]);
	}

	const Vt& operator()(St o, const Index& index) const{
		return this->operator ()(o, index.i(), index.j(), index.k());
	}

	const Grid& grid() const {
			return (*_grid);
	}
	const Ghost& ghost() const {
		return (*_ghost);
	}
	const Order& order() const {
		return (*_order);
	}
	spGrid spgrid() const{
		return _grid;
	}
	spGhost spghost() const{
		return _ghost;
	}
	spOrder sporder() const{
		return _order;
	}

protected:
	void initial_didx(){
		//  order = 0,  index = i j k
		_didx(0,0) = 0;  _didx(0,1) = 0; _didx(0,2) = 0;
		_didx(1,0) = 1;  _didx(1,1) = 0; _didx(1,2) = 0;
		_didx(2,0) = 0;  _didx(2,1) = 1; _didx(2,2) = 0;
		_didx(3,0) = 1;  _didx(3,1) = 1; _didx(3,2) = 0;
		_didx(4,0) = 0;  _didx(4,1) = 0; _didx(4,2) = 1;
		_didx(5,0) = 1;  _didx(5,1) = 0; _didx(5,2) = 1;
		_didx(6,0) = 0;  _didx(6,1) = 1; _didx(6,2) = 1;
		_didx(7,0) = 1;  _didx(7,1) = 1; _didx(7,2) = 1;
	}
};
}

#endif
