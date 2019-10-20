#ifndef _S_INTERPOLATION_LOCAL_HPP
#define _S_INTERPOLATION_LOCAL_HPP

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
#include "sinterpolation.hpp"

#include <array>

namespace carpio{

template<St DIM>
class SInterpolationLocal_ : public SInterpolation_<DIM>{
public:
	typedef SInterpolation_<DIM>      Base;
	typedef SInterpolationLocal_<DIM> Self;
    typedef SGrid_<DIM>   Grid;
    typedef SGhost_<DIM>  Ghost;
    typedef SOrder_<DIM>  Order;
    typedef SField_<DIM>  Field;
    typedef SIndex_<DIM>  Index;
    typedef std::shared_ptr<Field> spField;

    typedef SVectorCenter_<DIM> VectorCenter;
    typedef SVectorFace_<DIM>   VectorFace;
    typedef SCorner_<DIM>       Corner;
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;

    typedef Point_<Vt, DIM> Point;

//    typedef SValue_<DIM> Value;
//    typedef SValue_<1> Value1;
//    typedef SValue_<2> Value2;
//    typedef SValue_<3> Value3;

    typedef AInterpolate_<Vt, Vt> AInter;

    typedef SExpField_<DIM> ExpField;
    typedef typename ExpField::Expression Exp;
    typedef typename Exp::Term            Term;
    typedef typename Exp::Coe             Coe;

    SInterpolationLocal_(){}
    ~SInterpolationLocal_(){}

    Vt on_face(const Field& f, const Index& idx, int axe, int ori){}
    Vt on_axe(const Field& f,  const Index& idx, Vt distance, int axe){}
    Vt point(const Field& f,   const Index& idx, const Point& p){}

    Exp on_face_exp(const Field& f, const Index& idx, int axe, int ori){




    	return Exp();
    }
    Exp on_axe_exp(const Field& f,  const Index& idx, Vt distance, int axe){}
    Exp point_exp(const Field& f,   const Index& idx, const Point& p){}

protected:
    Exp _on_axe_exp_nn(const Field& f,
    		           const Index& idx, const Index& idxn,
					   Vt distance, int axe){
    	UNPACK_FIELD(f);
    	// the center of idx is original point
    	//
    	// original
    	//   idx(normal)           idxn(normal)
    	// ---+----------+--|---------+---  axe
    	//    |          |<---dis_o-->|
    	//    |<distance>|  |
    	//    |           face
    	//    |<-------discenter----->|
    	Vt discenter = std::abs(grid.c_(axe, idx) - grid.c_(axe, idxn));
    	Vt dis_o     = discenter - distance;
    	ASSERT(dis_o >= 0);


    }


};

}


#endif
