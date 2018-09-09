#ifndef _S_SCALAR_HPP_
#define _S_SCALAR_HPP_

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"

namespace carpio{

template<St DIM>
class SScalar_{
public:
	typedef SIndex_<DIM> Index;
	typedef SGrid_<DIM>  Grid;
	typedef SGhost_<DIM> Ghost;
	typedef SOrder_<DIM> Order;

	typedef std::shared_ptr<SIndex_<DIM> > spIndex;
	typedef std::shared_ptr<SGrid_<DIM>  > spGrid;
	typedef std::shared_ptr<SGhost_<DIM> > spGhost;
	typedef std::shared_ptr<SOrder_<DIM> > spOrder;

	typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;

	typedef MultiArrayV_<Vt, DIM> Mat;
	typedef typename Mat::reference reference;
	typedef typename Mat::const_reference const_reference;
	typedef SScalar_<DIM> Self;
protected:
	spGrid  _grid;
	spGhost _ghost;
	spOrder _order;

	Mat _mat;

public:
	SScalar_(spGrid spg, spGhost spgh): 
	    _grid(spg), _ghost(spgh),
		_mat(spg->n(_X_), spg->n(_Y_), spg->n(_Z_)){
	    // Initall a default order_xyz
	    _order = spOrder(new SOrderXYZ_<DIM>(spg,spgh));
	}
	SScalar_(spGrid spg, spGhost spgh, spOrder spor) :
			_grid(spg), _ghost(spgh), _order(spor),
			_mat(spg->n(_X_), spg->n(_Y_), spg->n(_Z_)) {
	}

	SScalar_(const Self& other):
	    _grid(other._grid), 
	    _ghost(other._ghost), 
	    _order(other._order), 
	    _mat(other._mat){
	}

	void assign(const Vt& value){
		_mat.assign(value);
	}

	void assign(FunXYZT_Value fun, Vt t = 0.0){
		for(auto& idx : (*_order)){
			auto cp = _grid->c(idx);
			this->operator ()(idx) = fun(cp.value(_X_),
					                     cp.value(_Y_),
										 cp.value(_Z_), t);
		}
	}

	Vt max() const {
		return _mat.max();
	}
	Vt min() const {
		return _mat.min();
	}

	Self& operator=(const Self& a) {
		if (this == &a) {
			return *this;
		}
		ASSERT(is_compatible(a));
		_mat = a._mat;
		return *this;
	}

	reference       operator()(St i, St j = 0, St k = 0){
	    return _mat.at(i, j, k);
	}
	const_reference operator()(St i, St j = 0, St k = 0) const{
	  	return _mat.at(i, j, k);
	}

	reference operator()(const Index& index) {
		ASSERT(_ghost->is_normal(index));
		return _mat.at(index.i(),
				       index.j(),
					   index.k());
	}
	const_reference operator()(const Index& index) const {
		ASSERT(_ghost->is_normal(index));
		return _mat.at(index.i(),
					   index.j(),
					   index.k());
	}

	Self operator-() const{
		Self res(*this);
		res._mat = -res._mat;
		return res;
	}

	Self& operator+=(const Self& rhs){
    	// actual addition of rhs to *this
		ASSERT(is_compatible(rhs));
		_mat += rhs._mat;
    	return *this;
    }

    Self& operator+=(const Vt& rhs){
    	// actual addition of rhs to *this
    	_mat += rhs;
    	return *this;
    }

    Self& operator-=(const Self& rhs) {
		// actual addition of rhs to *this
		ASSERT(is_compatible(rhs));
		_mat -= rhs._mat;
		return *this;
	}

	Self& operator-=(const Vt& rhs) {
		// actual addition of rhs to *this
		_mat -= rhs;
		return *this;
	}

	Self& operator*=(const Self& rhs) {
		// actual addition of rhs to *this
		ASSERT(is_compatible(rhs));
		_mat *= rhs._mat;
		return *this;
	}

	Self& operator*=(const Vt& rhs) {
		// actual addition of rhs to *this
		_mat *= rhs;
		return *this;
	}

	Self& operator/=(const Self& rhs) {
		// actual addition of rhs to *this
		ASSERT(is_compatible(rhs));
		_mat /= rhs._mat;
		return *this;
	}

	Self& operator/=(const Vt& rhs) {
		// actual addition of rhs to *this
		_mat /= rhs;
		return *this;
	}

    St n(St dim) const{
    	return _grid->n(dim);
    }

    St size() const{
    	return _ghost->size_normal();
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

    bool is_compatible(const Self& o) const{
    	return (  _grid  == o._grid
    			&&_ghost == o._ghost
				&&_order == o._order);
    }
    // return a new scalar with compatible gird, ghost and order
    Self new_compatible() const{
    	Self res(this->_grid, this->_ghost, this->_order);
    	return res;
    }

};
template<St DIM>
bool IsCompatible(const SScalar_<DIM>& lhs, const SScalar_<DIM>& rhs){
	return lhs.is_compatible(rhs);
}

template<St DIM>
std::ostream& operator<<(std::ostream& stream, const SScalar_<DIM>& s) {
	for (St d = 0; d < DIM; ++d) {
		tfm::format(stream, "%d", s.n(d));
		if(d < DIM - 1){
			tfm::format(stream, " x ");
		}
	}
	tfm::format(stream, "\n");
	if(DIM == 1){
		for(St i = 0; i<s.n(_X_); ++i){
			if( i < 3 || i > (s.n(_X_) - 3)){
				tfm::format(stream, "%.5e  ", 3);
			}
		}
		tfm::format(stream, "\n");
	}else if(DIM == 2){
		St nshow = 3;
		St nx = s.n(_X_);
		St ny = s.n(_Y_);
		for (int j = ny - 1; j >= 0; --j) {
			if (j < nshow || j > (ny - nshow - 1)) {
				for (St i = 0; i < nx; ++i) {
					if (i < nshow || i > (nx - nshow - 1)) {
						tfm::format(stream, "%.5e ", s(i, j));
					} else if (i == nshow && nx > nshow * 2) {
						tfm::format(stream, " ... ");
					}
				}
				tfm::format(stream, "\n");
			}else if (j == nshow && ny > nshow * 2){
				for (St i = 0; i < nshow; ++i) {
					tfm::format(stream, "    ...     ");
				}
				tfm::format(stream, " ... ");
				for (St i = 0; i < nshow; ++i) {
					tfm::format(stream, "    ...     ");
				}
				tfm::format(stream, "\n");
			}
		}
	}
	return stream;
}

template<St DIM>
inline SScalar_<DIM> operator+(SScalar_<DIM> lhs, const SScalar_<DIM>& rhs){
	lhs += rhs;
	return lhs;
}

template<St DIM>
inline SScalar_<DIM> operator+(SScalar_<DIM> lhs, const Vt& rhs){
	lhs += rhs;
	return lhs;
}

template<St DIM>
inline SScalar_<DIM> operator+(const Vt& lhs, SScalar_<DIM> rhs){
	rhs += lhs;
	return rhs;
}

template<St DIM>
inline SScalar_<DIM> operator-(SScalar_<DIM> lhs, const SScalar_<DIM>& rhs){
	lhs -= rhs;
	return lhs;
}

template<St DIM>
inline SScalar_<DIM> operator-(SScalar_<DIM> lhs, const Vt& rhs){
	lhs -= rhs;
	return lhs;
}

template<St DIM>
inline SScalar_<DIM> operator-(const Vt& lhs, SScalar_<DIM> rhs){
	rhs = -rhs + lhs;
	return rhs;
}

template<St DIM>
inline SScalar_<DIM> operator*(SScalar_<DIM> lhs, const SScalar_<DIM>& rhs){
	lhs *= rhs;
  	return lhs;
}

template<St DIM>
inline SScalar_<DIM> operator*(SScalar_<DIM> lhs, const Vt& rhs){
	lhs *= rhs;
	return lhs;
}

template<St DIM>
inline SScalar_<DIM> operator*(const Vt& lhs, SScalar_<DIM> rhs){
	rhs *= lhs;
	return rhs;
}

template<St DIM>
inline SScalar_<DIM> operator/(SScalar_<DIM> lhs, const SScalar_<DIM>& rhs){
	lhs /= rhs;
  	return lhs;
}

template<St DIM>
inline SScalar_<DIM> operator/(SScalar_<DIM> lhs, const Vt& rhs){
	lhs /= rhs;
	return lhs;
}

template<St DIM>
inline SScalar_<DIM> operator/(const Vt& lhs, const SScalar_<DIM>& rhs){
	SScalar_<DIM> res(rhs);
	res.assign(lhs);
	res /= rhs;
	return res;
}


}


#endif
