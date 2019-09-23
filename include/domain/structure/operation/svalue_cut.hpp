#ifndef _S_VALUECUT_HPP
#define _S_VALUECUT_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/ghost/linear_cut.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/sexpression.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "algebra/misc/interpolate.hpp"

#include <array>

namespace carpio{

template<St DIM>
class SValueCut_{
public:
    typedef SGrid_<DIM>   Grid;
    typedef SGhost_<DIM>  Ghost;
    typedef SGhostLinearCut_<DIM> GhostLinearCut;
    typedef std::shared_ptr<GhostLinearCut> spGhostLinearCut;
    typedef SOrder_<DIM>  Order;
    typedef SField_<DIM>  Field;
    typedef Field*       pField;
    typedef SIndex_<DIM>  Index;
    typedef std::shared_ptr<Field> spField;

    typedef SExpField_<DIM> ExpField;
    typedef typename ExpField::Expression Exp;
    typedef typename Exp::Term            Term;
    typedef typename Exp::Coe             Coe;

    typedef SVectorCenter_<DIM> VectorCenter;
    typedef SVectorFace_<DIM>   VectorFace;
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryIndex* pBI;
    typedef BoundaryCondition BC;

    typedef AInterpolate_<Vt, Vt> Interpolate;
public:
    SValueCut_(){
    }

    Vt get( const Field&         fc,
            const BoundaryIndex& bi,
            const Index&         idxc,
            const Index&         idxg, //get value at idxg
            const St&            axe,
            const St&            ori,
            const Vt&            time = 0.0){
        // no cut between idxc and idxg

    }

    Vt get_face(
            const Field&         fc,
            const BoundaryIndex& bi,
            const Index&         idx,
            const St&            axe,
            const St&            ori,
            const Vt&            time){

    }

    Exp get_expression(
                const Field&         fc,
                const BoundaryIndex& bi,
                const Index&         idxc,
                const Index&         idxg, // expression on ghost
                const St&            axe,
                const St&            ori,  //  c to g
                const Vt&            time = 0.0){
    	auto spgrid  = fc.spgrid();
    	auto spghost = fc.spghost();
    	ASSERT(spghost->type() == "SGhostLinearCut");
    	spGhostLinearCut spgc = std::dynamic_pointer_cast<GhostLinearCut>(spghost);
    	// case 1 idxg is normal
    	if(spgc->type(idxg) == _NORMAL_){
    		return Exp(idxg);
    	}
    	// case 2 idxg is cut
    	if(spgc->type(idxg) == _CUT_){

    	}

        // walkback
    	auto oori = Opposite(Orientation(ori));  // opposite orientation
		auto idxb = idxg.shift(axe, oori);
		int  step = 0;
		while (spgc->is_ghost(idxb) || spgc->is_cut(idxb)) {
			Shift(idxb, axe, oori);
			step++;
		}
    	// cut between idxc and idxg
    }

protected:



    Vt _value_type1(
                    const Field&       fc,
                    const BC&          bc,
                    const Index&       idxc,
                    const Index&       idxg,
                    const St&          axe,
                    const St&          ori,
                    const Vt&          time = 0.0){

    }

    static Vt _value_type2(
                    const Field&       fc,
                    const BC&          bc,
                    const Index&       idxc,
                    const Index&       idxg,
                    const St&          axe,
                    const St&          ori, // center --> ghost
                    const Vt&          time = 0.0){

    }
};

}

#endif
