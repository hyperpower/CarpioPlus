#ifndef _S_INTERPOLATION_HPP
#define _S_INTERPOLATION_HPP

#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/structure/operation/svalue_cut.hpp"
#include "domain/boundary/boundary_index.hpp"

#include "domain/structure/operation/soperation_define.hpp"

#include <array>

namespace carpio{

template<St DIM, class FIELD>
class SInterpolation_{
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
//    typedef SValue_<DIM> Value;
//    typedef SValue_<1> Value1;
//    typedef SValue_<2> Value2;
//    typedef SValue_<3> Value3;

    typedef AInterpolate_<Vt, Vt> AInter;

public:
    SInterpolation_(){}

    ~SInterpolation_(){}

protected:
     inline Index _locating1(const Field& f, const Index& idx, int axe, int ori) const{
    	 return idx.shift(axe, ori);
     }
     inline Index _locating2(const Field& f, const Index& idx, int axe, int ori) const{
         auto idx1 = idx.shift(axe, ori);
         return idx1.shift(axe, ori);
     }
     inline Index _locating(const Field& f, const Index& idx, int axe, int ori, int step) const{
         if(step == 1){
        	 return _locating1(f, idx, axe, ori);
         }else if(step == 2){
        	 return _locating2(f, idx, axe, ori);
         }else{
        	 SHOULD_NOT_REACH;
        	 return Index();
         }
     }




};

}


#endif
