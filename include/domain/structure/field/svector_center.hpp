#ifndef _S_VECTOR_CENTER_HPP_
#define _S_VECTOR_CENTER_HPP_

#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "sfield.hpp"
#include "utility/tinyformat.hpp"

#include "algebra/array/multi_array.hpp"

#include <limits>

namespace carpio{

template<St DIM, class GRID, class GHOST, class ORDER>
class SVectorCenter_{
public:
    typedef SIndex_<DIM> Index;
    typedef GRID  Grid;
    typedef GHOST Ghost;
    typedef ORDER Order;

    typedef std::shared_ptr<SIndex_<DIM> > spIndex;
    typedef std::shared_ptr<Grid>  spGrid;
    typedef std::shared_ptr<Ghost> spGhost;
    typedef std::shared_ptr<Order> spOrder;

    typedef MultiArrayV_<Vt, DIM> Mat;
    typedef typename Mat::reference reference;
    typedef typename Mat::const_reference const_reference;
    typedef SVectorCenter_<DIM, GRID, GHOST, ORDER> Self;
    typedef SField_<DIM, GRID, GHOST, ORDER> Field;
    typedef std::shared_ptr<Field> spField;

protected:
    std::array<spField, DIM> _arrs;
public:
    SVectorCenter_() {
        FOR_EACH_DIM{
            _arrs[d] = nullptr;
        }
    }

    SVectorCenter_(
            spField u,
            spField v = nullptr,
            spField w = nullptr){
        spField a[] = {u,v,w};
        FOR_EACH_DIM{
            ASSERT(a[d] != nullptr);
            _arrs[d] = a[d];
        }
    }

    void set(Axes a, spField sps){
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

    Field& operator[](St d){
        return *(_arrs[d]);
    }

    const Field& operator[](St d) const{
        return *(_arrs[d]);
    }

    Vt max() const{
        Vt m = _arrs[_X_]->max();
        for(St d = 1; d< DIM; d++){
            Vt md = _arrs[d]->max();
            if(m < md){
                m = md;
            }
        }
        return m;
    }

    Vt max_norm2() const{
        if(DIM == 1){
            return max();
        }else if(DIM == 2){
            auto sum = SqareSum(*(_arrs[_X_]), *(_arrs[_Y_]));
            return std::sqrt(sum.max());
        }else{
            auto sum = SqareSum(_arrs[_X_], _arrs[_Y_], _arrs[_Z_]);
            return std::sqrt(sum.max());
        }
    }



};

}

#endif
