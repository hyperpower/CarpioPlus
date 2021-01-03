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

template<St DIM, class FIELD>
class SLoop_{
public:
    typedef FIELD  Field;
    typedef typename Field::Grid  Grid;
    typedef typename Field::Ghost Ghost;
    typedef typename Field::Order Order;
    typedef SIndex_<DIM>  Index;
    typedef std::shared_ptr<Field> spField;

    typedef SVectorCenter_<DIM, Grid, Ghost, Order> VectorCenter;
    typedef SVectorFace_<DIM, Grid, Ghost, Order>   VectorFace;
    typedef SCorner_<DIM, Grid, Ghost, Order>       Corner;
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef SValue_<DIM, Field> Value;
    typedef SValue_<1, Field> Value1;
    typedef SValue_<2, Field> Value2;
    typedef SValue_<3, Field> Value3;

    typedef AInterpolate_<Vt, Vt> AInter;
    typedef std::function<void(const Index&, const Grid&, const Ghost&) > FunIndexGridGhost;
    typedef std::function<void(const Index&, St) > FunIndex;
//    typedef std::function<void(const Index&, St) > FunIndexThreadNum;

    static int ForEachIndex(const Order& order,
                            FunIndex fun){
#ifdef OPENMP
        #pragma omp parallel for
        for (St thread = 0; thread < order.num_threads(); thread++) {
            for (auto iter = order.begin(thread); iter != order.end(thread);
                    iter++) {
                auto& idx = (*iter);
                St nt = thread;
                nt = omp_get_thread_num();
                fun(idx, nt);
            }
        }
        return _SUCCESS;
#else
        for (auto& idx : order) {
            fun(idx,0);
        }
        return _SUCCESS;
#endif
    }
};





template<St DIM, class FIELD>
class SOperation_{
public:
    typedef FIELD  Field;
    typedef typename Field::Grid  Grid;
    typedef typename Field::Ghost Ghost;
    typedef typename Field::Order Order;
    typedef SIndex_<DIM>  Index;

    typedef SVectorCenter_<DIM, Grid, Ghost, Order> VectorCenter;
    typedef SVectorFace_<DIM, Grid, Ghost, Order>   VectorFace;
    typedef SCorner_<DIM, Grid, Ghost, Order>       Corner;

    typedef SExpField_<DIM, Grid, Ghost, Order>  ExpField;
    typedef typename ExpField::Expression Exp;
    typedef BoundaryIndex     BI;

    typedef SValue_<DIM, FIELD> Value;

};

}

#endif
