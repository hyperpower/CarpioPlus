#ifndef _S_OPERATION_DEFINE_HPP
#define _S_OPERATION_DEFINE_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/scorner.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/boundary/boundary_index.hpp"

#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/bc_value.hpp"
#include "domain/boundary/boundary_index.hpp"

#include "sinterpolate.hpp"
#include "svalue.hpp"

#ifdef OPENMP
#include <omp.h>
#endif

#include <array>

namespace carpio{

template<St DIM>
class SLoop_{
public:
    typedef SGrid_<DIM>   Grid;
    typedef SGhost_<DIM>  Ghost;
    typedef SOrder_<DIM>  Order;
    typedef SOrderParallel_<DIM>  OrderParallel;
    typedef SField_<DIM>  Field;
    typedef SIndex_<DIM>  Index;
    typedef std::shared_ptr<Field> spField;

    typedef SVectorCenter_<DIM> VectorCenter;
    typedef SVectorFace_<DIM>   VectorFace;
    typedef SCorner_<DIM>       Corner;
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef SValue_<DIM> Value;
    typedef SValue_<1> Value1;
    typedef SValue_<2> Value2;
    typedef SValue_<3> Value3;

    typedef AInterpolate_<Vt, Vt> AInter;
    typedef std::function<void(const Index&, const Grid&, const Ghost&) > FunIndexGridGhost;
    typedef std::function<void(const Index&) > FunIndex;
    typedef std::function<void(const Index&, St) > FunIndexThreadNum;

    static int ForEachIndex(const Order& order,
                            FunIndex fun){
        for (auto& idx : order) {
            fun(idx);
        }
        return _SUCCESS;
    }

    static int ForEachIndexParallel(
            const OrderParallel& order,
                  FunIndexThreadNum fun){
        #pragma omp parallel for
        for (St thread = 0; thread < order.num_threads(); thread++){
            for (auto iter = order.begin(thread); iter != order.end(thread); iter++) {
                auto& idx = (*iter);
                St nt = thread;
#ifdef OPENMP
                nt = omp_get_thread_num();
#endif
                fun(idx, nt);
            }
        }
        return _SUCCESS;
    }
};





template<St DIM>
class SOperation_{
public:
    typedef SGrid_<DIM>   Grid;
    typedef SGhost_<DIM>  Ghost;
    typedef SOrder_<DIM>  Order;
    typedef SField_<DIM>  Field;
    typedef SIndex_<DIM>  Index;

    typedef SVectorCenter_<DIM> VectorCenter;
    typedef SVectorFace_<DIM>   VectorFace;

    typedef SExpField_<DIM>                      ExpField;
    typedef typename SExpField_<DIM>::Expression Exp;
    typedef BoundaryIndex       BI;

    typedef SValue_<DIM> Value;

};

}

#endif
