#ifndef _S_ORDER_XYZ_HPP_
#define _S_ORDER_XYZ_HPP_


#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "domain/structure/grid/sgrid.hpp"
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
        _grid(spgrid), _ghost(spghost), _ov(_ghost->size_not_ghost()),
        _om(_grid->n(_X_), _grid->n(_Y_), _grid->n(_Z_)){
        _build_ov();
        _build_om();
    }

    St size() const{
        return _ov.size();
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

    const Grid& grid() const {
        return (*_grid);
    }
    const Ghost& ghost() const {
        return (*_ghost);
    }
    spGrid spgrid() const{
        return _grid;
    }
    spGhost spghost() const{
        return _ghost;
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

template<St DIM>
class SOrderXYZParallel_ : public SOrderParallel_<DIM>{
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

    St _nt;
    std::vector<iterator> _ov_iter; // store the separate index of _ov
    std::vector<const_iterator> _ov_citer; // store the separate index of _ov

public:
    SOrderXYZParallel_(spGrid spgrid, spGhost spghost, St num_threads):
        _grid(spgrid), _ghost(spghost), _ov(_ghost->size_normal()),
        _om(_grid->n(_X_), _grid->n(_Y_), _grid->n(_Z_)),
        _nt(num_threads),
        _ov_iter(num_threads + 1),
        _ov_citer(num_threads + 1){
        _build_ov();
        _build_om();
        _build_ovt();
    }

    St num_threads() const{
        return _nt;
    }

    St size() const{
        return _ov.size();
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

    iterator begin(St tn) { // input thread number
        ASSERT(tn < _nt);
        return _ov_iter[tn];
    }
    const_iterator begin(St tn) const {
        ASSERT(tn < _nt);
        return _ov_citer[tn];
    }
    iterator end(St tn) {
        ASSERT(tn < _nt);
        return _ov_iter[tn + 1];
    }
    const_iterator end(St tn) const {
        ASSERT(tn < _nt);
        return _ov_citer[tn + 1];
    }

    St get_order(const Index& index) const{
        if(!(_ghost->is_ghost(index))){
            return _om(index.i(), index.j(), index.k());
        }
        std::cerr<< ">! Error Index = " << index << std::endl;
        SHOULD_NOT_REACH;
        return 0;
    }
    const Grid& grid() const {
        return (*_grid);
    }
    const Ghost& ghost() const {
        return (*_ghost);
    }
    spGrid spgrid() const {
        return _grid;
    }
    spGhost spghost() const {
        return _ghost;
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

    void _build_ovt(){
        St d = _ov.size() / _nt;
        for(St i = 0; i < _nt; i++ ){
            St step = d * i;
            auto it = this->begin();
            std::advance(it, step);
            _ov_iter[i]  = it;
            auto cit = this->begin();
            _ov_citer[i]  = std::next(cit, step);;
        }
        _ov_iter[_nt] = this->end();
        _ov_citer[_nt] = this->end();
    }
};


}



#endif
