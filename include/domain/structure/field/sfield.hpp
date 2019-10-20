#ifndef _S_SCALAR_HPP_
#define _S_SCALAR_HPP_

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"

namespace carpio{

template<St DIM>
class SField_{
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
    typedef SField_<DIM> Self;
protected:
    spGrid  _grid;
    spGhost _ghost;
    spOrder _order;

    Mat _mat;

public:
    SField_(spGrid spg, spGhost spgh):
        _grid(spg), _ghost(spgh),
        _mat(spg->n(_X_), spg->n(_Y_), spg->n(_Z_)){
        // Initall a default order_xyz
        _order = spOrder(new SOrderXYZ_<DIM>(spg,spgh));
    }
    SField_(spGrid spg, spGhost spgh, spOrder spor) :
            _grid(spg), _ghost(spgh), _order(spor),
            _mat(spg->n(_X_), spg->n(_Y_), spg->n(_Z_)) {
    }

    SField_(const Self& other):
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
    Vt sum() const{
    	Vt sum = 0.0;
        for(auto& idx : (*_order)){
        	sum += this->operator ()(idx);
        }
    	return sum;
    }

    Vt max() const {
        return _mat.max();
    }
    Vt min() const {
        return _mat.min();
    }

    Vt norm1() const {
        return _mat.norm1();
    }

    void abs(){
        _mat.abs();
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
        return _mat.at(index.i(),
                       index.j(),
                       index.k());
    }
    const_reference operator()(const Index& index) const {
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

    spGrid spgrid() const{
        return _grid;
    }
    spGhost spghost() const{
        return _ghost;
    }
    spOrder sporder() const{
        return _order;
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
    // returen a new scalar with cell volume
    Self volume_field() const{
        Self res = this->new_compatible();
        auto& grid = res.grid();
        for(auto& idx : (*_order)){
            res(idx) = grid.volume(idx);
        }
        return res;
    }

    // returen a new scalar with cell inverse volume
    Self inverse_volume_field() const {
        Self res(this->_grid, this->_ghost, this->_order);
        auto& grid = res.grid();
        for (auto& idx : (*_order)) {
            res(idx) = 1.0 / grid.volume(idx);
        }
        return res;
    }


};
template<St DIM>
bool IsCompatible(const SField_<DIM>& lhs, const SField_<DIM>& rhs){
    return lhs.is_compatible(rhs);
}

template<St DIM>
std::ostream& operator<<(std::ostream& stream, const SField_<DIM>& s) {
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
inline SField_<DIM> operator+(SField_<DIM> lhs, const SField_<DIM>& rhs){
    lhs += rhs;
    return lhs;
}

template<St DIM>
inline SField_<DIM> operator+(SField_<DIM> lhs, const Vt& rhs){
    lhs += rhs;
    return lhs;
}

template<St DIM>
inline SField_<DIM> operator+(const Vt& lhs, SField_<DIM> rhs){
    rhs += lhs;
    return rhs;
}

template<St DIM>
inline SField_<DIM> operator-(SField_<DIM> lhs, const SField_<DIM>& rhs){
    lhs -= rhs;
    return lhs;
}

template<St DIM>
inline SField_<DIM> operator-(SField_<DIM> lhs, const Vt& rhs){
    lhs -= rhs;
    return lhs;
}

template<St DIM>
inline SField_<DIM> operator-(const Vt& lhs, SField_<DIM> rhs){
    rhs = -rhs + lhs;
    return rhs;
}

template<St DIM>
inline SField_<DIM> operator*(SField_<DIM> lhs, const SField_<DIM>& rhs){
    lhs *= rhs;
      return lhs;
}

template<St DIM>
inline SField_<DIM> operator*(SField_<DIM> lhs, const Vt& rhs){
    lhs *= rhs;
    return lhs;
}

template<St DIM>
inline SField_<DIM> operator*(const Vt& lhs, SField_<DIM> rhs){
    rhs *= lhs;
    return rhs;
}

template<St DIM>
inline SField_<DIM> operator/(SField_<DIM> lhs, const SField_<DIM>& rhs){
    lhs /= rhs;
      return lhs;
}

template<St DIM>
inline SField_<DIM> operator/(SField_<DIM> lhs, const Vt& rhs){
    lhs /= rhs;
    return lhs;
}

template<St DIM>
inline SField_<DIM> operator/(const Vt& lhs, const SField_<DIM>& rhs){
    SField_<DIM> res(rhs);
    res.assign(lhs);
    res /= rhs;
    return res;
}
// a^2
template<St DIM>
SField_<DIM> Square(const SField_<DIM>& a){
    auto res = a.new_compatible();

    for(auto& idx : res.order()){
        auto va = a(idx);
        res(idx)= va * va;
    }
    return res;
}


// a^2 + b^2
template<St DIM>
SField_<DIM> SquareSum(const SField_<DIM>& a, const SField_<DIM>& b){
    ASSERT(a.is_compatible(b));
    auto res = a.new_compatible();

    for(auto& idx : res.order()){
        auto va = a(idx);
        auto vb = b(idx);
        res(idx)= va * va + vb * vb;
    }
    return res;
}

template<St DIM>
SField_<DIM> SquareSum(const SField_<DIM>& a,
                       const SField_<DIM>& b,
                       const SField_<DIM>& c){
    ASSERT(a.is_compatible(b));
    ASSERT(a.is_compatible(c));
    auto res = a.new_compatible();

    for(auto& idx : res.order()){
        auto va = a(idx);
        auto vb = b(idx);
        auto vc = c(idx);
        res(idx)= va * va + vb * vb + vc * vc;
    }
    return res;
}

template<St DIM>
SField_<DIM> Sqrt(const SField_<DIM>& a){
    auto res = a.new_compatible();

    for(auto& idx : res.order()){
        auto va = a(idx);
        res(idx)= std::sqrt(va);
    }
    return res;
}

template<St DIM>
SField_<DIM> Abs(const SField_<DIM>& f){
    auto res(f);
    res.abs();
    return res;
}

template<St DIM>
Vt Norm1(const SField_<DIM>& f, const SField_<DIM>& exact){
    auto res = f - exact;
    return res.norm1();
}

template<St DIM>
Vt Norm2(const SField_<DIM>& f, const SField_<DIM>& exact){
    auto res = Square(f - exact);
    return res.sum();
}

template<St DIM>
Vt NormInf(const SField_<DIM>& f, const SField_<DIM>& exact){
    auto res = Abs(f - exact);
    return res.max();
}

#define UNPACK_FIELD(field)  \
	auto& grid  = field.grid();  \
	auto& ghost = field.ghost(); \
	auto& order = field.order();


}


#endif
