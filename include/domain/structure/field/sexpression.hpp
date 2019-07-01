#ifndef _S_EXPRESSION_HPP_
#define _S_EXPRESSION_HPP_

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/xyz.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/misc/linear_polynomial.hpp"

namespace carpio{

template<St DIM>
class SExpField_{
public:
	typedef SIndex_<DIM> Index;
	typedef SGrid_<DIM>  Grid;
	typedef SGhost_<DIM> Ghost;
	typedef SOrder_<DIM> Order;
	typedef SField_<DIM> Field;

	typedef std::shared_ptr<SIndex_<DIM> > spIndex;
	typedef std::shared_ptr<SGrid_<DIM>  > spGrid;
	typedef std::shared_ptr<SGhost_<DIM> > spGhost;
	typedef std::shared_ptr<SOrder_<DIM> > spOrder;

	typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;

	typedef LinearPolynomial_<Vt, Index> Expression;

	typedef MultiArrayV_<Expression, DIM> Mat;
	typedef typename Mat::reference             reference;
	typedef typename Mat::const_reference const_reference;
	typedef SExpField_<DIM> Self;
protected:
	spGrid  _grid;
	spGhost _ghost;
	spOrder _order;

	Mat _mat;

public:
	SExpField_(spGrid spg, spGhost spgh):
	    _grid(spg), _ghost(spgh),
		_mat(spg->n(_X_), spg->n(_Y_), spg->n(_Z_)){
	    // Initall a default order_xyz
	    _order = spOrder(new SOrderXYZ_<DIM>(spg,spgh));
	}

	SExpField_(spGrid spg, spGhost spgh, spOrder spor) :
			_grid(spg), _ghost(spgh), _order(spor),
			_mat(spg->n(_X_), spg->n(_Y_), spg->n(_Z_)) {
	}

	SExpField_(const Self& other) :
			_grid(other._grid),
			_ghost(other._ghost),
			_order(other._order),
			_mat(other._mat) {
	}
	Self& operator=(const Self& a) {
		if (this == &a) {
			return *this;
		}
		ASSERT(is_compatible(a));
		_mat = a._mat;
		return *this;
	}

	St n(St dim) const {
		return _grid->n(dim);
	}

	St size() const {
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

	reference       operator()(St i, St j = 0, St k = 0){
	    return _mat.at(i, j, k);
	}

	const_reference operator()(St i, St j = 0, St k = 0) const{
	  	return _mat.at(i, j, k);
	}
	reference operator()(const Index& index) {
		ASSERT(_ghost->is_normal(index));
		return _mat.at(index.i(), index.j(), index.k());
	}
	const_reference operator()(const Index& index) const {
		ASSERT(_ghost->is_normal(index));
		return _mat.at(index.i(), index.j(), index.k());
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

	Self& operator+=(const Field& rhs){
		ASSERT(is_compatible(rhs));
		// actual addition of rhs to *this
		for(auto& idx : *(_order)){
			this->operator ()(idx) += rhs(idx);
		}
		return *this;
	}

	Self& operator+=(const Vt& rhs){
		// actual addition of rhs to *this
		for(auto& idx : *(_order)){
			this->operator ()(idx) += rhs;
		}
	    return *this;
	}

	Self& operator-=(const Self& rhs){
	    // actual addition of rhs to *this
		ASSERT(is_compatible(rhs));
		_mat -= rhs._mat;
	    return *this;
	}

	Self& operator-=(const Field& rhs){
		ASSERT(is_compatible(rhs));
		// actual addition of rhs to *this
		for(auto& idx : *(_order)){
			this->operator ()(idx) -= rhs(idx);
		}
		return *this;
	}

	Self& operator-=(const Vt& rhs){
		// actual addition of rhs to *this
		for(auto& idx : *(_order)){
			this->operator ()(idx) -= rhs;
		}
	    return *this;
	}

	Self& operator*=(const Field& rhs){
		ASSERT(is_compatible(rhs));
		// actual addition of rhs to *this
		for(auto& idx : *(_order)){
			this->operator ()(idx) *= rhs(idx);
		}
		return *this;
	}

	Self& operator*=(const Vt& rhs){
		// actual addition of rhs to *this
		for(auto& idx : *(_order)){
			this->operator ()(idx) *= rhs;
		}
	    return *this;
	}

	Self& operator/=(const Field& rhs){
		ASSERT(is_compatible(rhs));
		// actual addition of rhs to *this
		for(auto& idx : *(_order)){
			this->operator ()(idx) /= rhs(idx);
		}
		return *this;
	}

	Self& operator/=(const Vt& rhs){
		// actual addition of rhs to *this
		for(auto& idx : *(_order)){
			this->operator ()(idx) /= rhs;
		}
	    return *this;
	}

	bool is_compatible(const Field& o) const {
		return (   _grid  == o.spgrid()
				&& _ghost == o.spghost());
	}

	bool is_compatible(const Self& o) const {
		return (   _grid  == o._grid
				&& _ghost == o._ghost
				&& _order == o._order);
	}
	// return a new expression with compatible gird, ghost and order
	Self new_compatible() const {
		Self res(this->_grid, this->_ghost, this->_order);
		return res;
	}
protected:
	void _initial(){
		for(auto& idx : *(_order)){
			this->operator ()(idx) += idx;
		}
	}
};

template<St DIM>
inline SExpField_<DIM> operator+(SExpField_<DIM> lhs, const SExpField_<DIM>& rhs){
	lhs += rhs;
	return lhs;
}

template<St DIM>
inline SExpField_<DIM> operator+(SExpField_<DIM> lhs, const Vt& rhs){
	lhs += rhs;
	return lhs;
}

template<St DIM>
inline SExpField_<DIM> operator+(const Vt& lhs, SExpField_<DIM> rhs){
	rhs += lhs;
	return rhs;
}
template<St DIM>
inline SExpField_<DIM> operator-(SExpField_<DIM> lhs, const SExpField_<DIM>& rhs){
	lhs -= rhs;
	return lhs;
}

template<St DIM>
inline SExpField_<DIM> operator-(SExpField_<DIM> lhs, const Vt& rhs){
	lhs -= rhs;
	return lhs;
}

template<St DIM>
inline SExpField_<DIM> operator-(const Vt& lhs, SExpField_<DIM> rhs){
	rhs -= lhs;
	return rhs;
}

template<St DIM>
inline SExpField_<DIM> operator*(SExpField_<DIM> lhs, const Vt& rhs){
	lhs *= rhs;
	return lhs;
}

template<St DIM>
inline SExpField_<DIM> operator*(const Vt& lhs, SExpField_<DIM> rhs){
	rhs *= lhs;
	return rhs;
}
template<St DIM>
inline SExpField_<DIM> operator*(SExpField_<DIM> lhs, const SField_<DIM>& rhs){
	lhs *= rhs;
	return lhs;
}

template<St DIM>
inline SExpField_<DIM> operator*(const SField_<DIM>& lhs, SExpField_<DIM> rhs){
	rhs *= lhs;
	return rhs;
}
template<St DIM>
inline SExpField_<DIM> operator/(SExpField_<DIM> lhs, const Vt& rhs){
	lhs /= rhs;
	return lhs;
}
template<St DIM>
inline SExpField_<DIM> operator/(SExpField_<DIM> lhs, const SField_<DIM>& rhs){
	lhs /= rhs;
	return lhs;
}



template<typename VALUE, St DIM>
void BuildMatrix(const SExpField_<DIM>& expf,
		               MatrixSCR_<VALUE>& mat,
					   ArrayListV_<VALUE>& b){

}



}

#endif
