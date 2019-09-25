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
	typedef Point_<Vt, DIM>  Point;
    typedef SGrid_<DIM>   Grid;
    typedef std::shared_ptr<Grid> spGrid;
    typedef SGhost_<DIM>  Ghost;
    typedef SGhostLinearCut_<DIM> GhostLinearCut;
    typedef std::shared_ptr<GhostLinearCut> spGhostLinearCut;
    typedef typename GhostLinearCut::spCell spCell;
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
    		auto bid  = spgc->boundary_id(idxc, idxg, axe, ori);
    		auto spbc = bi.find(bid);
    		auto spcell = spgc->operator()(idxg);
    		ASSERT(spcell != nullptr);
			if (spbc->type() == BC::_BC1_) {
				return _value_type1_cut_exp(
						fc, spgrid, spgc, spcell, idxg, *spbc, time);
			} else if (spbc->type() == BC::_BC2_) {
				return _value_type2_cut_exp(
						fc, spgrid, spgc, spcell, idxg, *spbc, time);
			}
		}
    	// case 3 idxg is ghost
    	if(spgc->type(idxg) == _GHOST_){
    		// find cut cell between idxc and idxg
    		auto oori = Opposite(Orientation(ori));  // opposite oritation
    		auto idxb = idxg;
    		int  step = 0;
    		while(spghost->is_ghost(idxb)){ // find nearest normal cell or cut;
    		      Shift(idxb, axe, oori);
    		      step++;
    		}
    		auto spcell = spgc->operator()(idxb);
    		ASSERT(spcell != nullptr);
    		ASSERT(spcell->type() == _CUT_);
    		auto bid  = spcell->get_boundary_id();
    		auto spbc = bi.find(bid);
			if (spbc->type() == BC::_BC1_) {
				return _value_type1_cut_ghost_exp(
						fc, spgrid, spgc, spcell, idxb, idxg,
						*spbc, time);
			} else if (spbc->type() == BC::_BC2_) {
				return _value_type2_cut_ghost_exp(
						fc, spgrid, spgc, spcell, idxb, idxg,
						*spbc, time);
			}

    	}
    }

protected:
    Exp _value_type1_cut_exp(
                    const Field&           field,
					const spGrid           spgrid,
					const spGhostLinearCut spghost,
					const spCell           spcell,
                    const Index&           idxg,
                    const BC&              bc,
                    const Vt&              time = 0.0){
		//  normal face  cell
		// ---x-----|-----g-----
		//    +--nf-+--fc-+
		// equation:
		//  vc - vx      vf - vx
		// --------- = ----------  ==> vc - vx = (vf - vx) * (dx + dg) / dx;
		//     nc         nf           vg = vx + (vf - vx) * (dx + dg) / dx;
    	Point cc = spgrid->c(idxg);   // cell center
    	Point nc = spcell->nc();      // normal center
		Point fc = spcell->intersect_front(cc, nc);  //front intersect
		Vt disnc = Distance(cc, nc);
		Vt disnf = Distance(nc, fc);
		Vt vbc = bc.value(fc.value(_X_), fc.value(_Y_), fc.value(_Z_), time);
		Exp expx(idxg);
		return expx + (vbc - expx) * disnc / disnf;
    }

    Exp _value_type1_cut_ghost_exp(
    		const Field& field,
			const spGrid spgrid,
			const spGhostLinearCut spghost,
			const spCell spcell,
			const Index& idxc,
			const Index& idxg,
			const BC& bc,
			const Vt& time = 0.0) {
		//  normal face  cell
		// ---x-----|-----g-----
		//    +--nf-+--fc-+
		// equation:
		//  vc - vx      vf - vx
		// --------- = ----------  ==> vc - vx = (vf - vx) * (dx + dg) / dx;
		//     nc         nf           vg = vx + (vf - vx) * (dx + dg) / dx;
		Point cc = spgrid->c(idxg);   // cell center
		Point nc = spcell->nc();      // normal center
		Point fc = spcell->intersect_front(cc, nc);  //front intersect
		Vt disnc = Distance(cc, nc);
		Vt disnf = Distance(nc, fc);
		Vt vbc = bc.value(fc.value(_X_), fc.value(_Y_), fc.value(_Z_), time);
		Exp expx(idxc);
		return expx + (vbc - expx) * disnc / disnf;
	}

    Exp _value_type2_cut_exp(
                        const Field&           field,
    					const spGrid           spgrid,
    					const spGhostLinearCut spghost,
    					const spCell           spcell,
                        const Index&           idxg,
                        const BC&              bc,
                        const Vt&              time = 0.0){
    	SHOULD_NOT_REACH;
    }

    Exp _value_type2_cut_ghost_exp(
        		const Field& field,
    			const spGrid spgrid,
    			const spGhostLinearCut spghost,
    			const spCell spcell,
    			const Index& idxc,
    			const Index& idxg,
    			const BC& bc,
    			const Vt& time = 0.0) {
    	SHOULD_NOT_REACH;
    }
};

}

#endif
