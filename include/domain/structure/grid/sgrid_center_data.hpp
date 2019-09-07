#ifndef _S_GRID_CENTER_DATA_
#define _S_GRID_CENTER_DATA_
#include "domain/structure/grid/sgrid.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"

namespace carpio{

template<class NUM, St DIM>
class SGridCenterData_{
public:
    typedef SIndex_<DIM> Index;
    typedef SGrid_<DIM>  Grid;

    typedef std::shared_ptr<SIndex_<DIM> > spIndex;
    typedef std::shared_ptr<SGrid_<DIM>  > spGrid;

    typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;

    typedef MultiArrayV_<NUM, DIM> Mat;
    typedef typename Mat::reference reference;
    typedef typename Mat::const_reference const_reference;
    typedef SGridCenterData_<NUM, DIM> Self;
protected:
    spGrid  _grid;

    Mat _mat;

public:
    SGridCenterData_(spGrid spg):
        _grid(spg),
        _mat(spg->n(_X_), spg->n(_Y_), spg->n(_Z_)){
        // Initall a default order_xyz
    }

    SGridCenterData_(const Self& other):
        _grid(other._grid),
        _mat(other._mat){
    }

    void assign(const Vt& value){
        _mat.assign(value);
    }

    void assign(FunXYZT_Value fun, Vt t = 0.0){
    	SHOULD_NOT_REACH;
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

    St n(St dim) const{
        return _grid->n(dim);
    }

    St size() const{
    	SHOULD_NOT_REACH;
    }

    const Grid& grid() const {
        return (*_grid);
    }

    spGrid spgrid() const{
        return _grid;
    }

    bool is_compatible(const Self& o) const{
        return (  _grid  == o._grid);
    }



};







}


#endif
