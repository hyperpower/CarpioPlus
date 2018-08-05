#ifndef _S_VECTOR_CENTER_HPP_
#define _S_VECTOR_CENTER_HPP_

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "sscalar.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"

namespace carpio{

template<St DIM>
class SVectorCenter_{
public:
	typedef SIndex_<DIM> Index;
	typedef SGrid_<DIM>  Grid;
	typedef SGhost_<DIM> Ghost;
	typedef SOrder_<DIM> Order;

	typedef std::shared_ptr<SIndex_<DIM> > spIndex;
	typedef std::shared_ptr<SGrid_<DIM> >  spGrid;
	typedef std::shared_ptr<SGhost_<DIM> > spGhost;
	typedef std::shared_ptr<SOrder_<DIM> > spOrder;

	typedef MultiArrayV_<Vt, DIM> Mat;
	typedef typename Mat::reference reference;
	typedef typename Mat::const_reference const_reference;
	typedef SVectorCenter_<DIM> Self;
	typedef SScalar_<DIM> Scalar;
	typedef std::shared_ptr<Scalar> spScalar;

protected:
	std::array<spScalar, DIM> _arrs;
public:
	SVectorCenter_() {
		FOR_EACH_DIM{
			_arrs[d] = nullptr;
		}
	}

	SVectorCenter_(
			spScalar u,
			spScalar v = nullptr,
			spScalar w = nullptr){
		spScalar a[] = {u,v,w};
		FOR_EACH_DIM{
			ASSERT(a[d] != nullptr);
			_arrs[d] = a[d];
		}
	}

	void set(Axes a, spScalar sps){
		ASSERT(a < DIM);
		_arrs[a] = sps;
	}

	const Order& order() const {
		return _arrs[_X_]->order();
	}

	const Grid& grid() const {
		return _arrs[_X_]->grid();
	}

	const Ghost& ghost() const {
		return _arrs[_X_]->ghost();
	}

	Scalar& operator[](St d){
		return *(_arrs[d]);
	}

	const Scalar& operator[](St d) const{
		return *(_arrs[d]);
	}




};

}

#endif
