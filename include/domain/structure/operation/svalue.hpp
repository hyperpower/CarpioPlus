#ifndef _S_Value_HPP
#define _S_Value_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/boundary/boundary_index.hpp"

#include <array>

namespace carpio{


template<St DIM>
class SValue_{
public:
	typedef SGrid_<DIM>   Grid;
	typedef SGhost_<DIM>  Ghost;
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
public:
	SValue_(){
	}

	static Vt Get(
			const Field&         fc,
			const BoundaryIndex& bi,
		    const Index&         idxc,
			const Index&         idxg,
			const St&            axe,
			const St&            ori,
			const Vt&            time = 0.0){
		if(fc.ghost().is_ghost(idxg)){
			auto bid  = fc.ghost().boundary_id(idxc, idxg, axe, ori);
			auto spbc = bi.find(bid);
			if(spbc->type() == BC::_BC1_){
				return _value_type1(fc, *spbc, idxc, idxg, axe, ori, time);
			}else if(spbc->type() == BC::_BC2_){
				return _value_type2(fc, *spbc, idxc, idxg, axe, ori, time);
			}
		}else{
			return fc(idxg);
		}
	}

	static Vt Max(const Field& s){
		return s.max();
	}

	static Vt Min(const Field& s) {
		return s.min();
	}

	static Exp Get(
			const ExpField&      fc,
			const BoundaryIndex& bi,
		    const Index&         idxc,
			const Index&         idxg,
			const St&            axe,
			const St&            ori,
			const Vt&            time = 0.0){
		if(fc.ghost().is_ghost(idxg)){
			auto bid  = fc.ghost().boundary_id(idxc, idxg, axe, ori);
			auto spbc = bi.find(bid);
			if (spbc->type() == BC::_BC1_) {
				return _value_type1(fc, *spbc, idxc, idxg, axe, ori, time);
			} else if (spbc->type() == BC::_BC2_) {
				return _value_type2(fc, *spbc, idxc, idxg, axe, ori, time);
			}
		} else {
			return idxg;
		}
	}

protected:

	static Vt _value_type1(
			        const Field&      fc,
			        const BC&          bc,
					const Index&       idxc,
					const Index&       idxg,
					const St&          axe,
					const St&          ori,
					const Vt&          time = 0.0){
		// boundary condition must be type 1
		// walk back
		auto oori = Opposite(Orientation(ori));  // opposite oritation
		auto idxb = idxg.shift(axe, oori);
		int step  = 0;
		while(fc.ghost().is_ghost(idxb)){
			Shift(idxb, axe, oori);
			step++;
		}
		auto fp = fc.grid().f(axe, ori, idxb);   // face point
		for(int i = 0; i < step; ++i){
			Shift(idxb, axe, oori);
		}
		ASSERT(fc.ghost().is_normal(idxb));
		//  idxb   face  ghost
		// ---x-----|-----g-----
	    //    +--dx-+--dg-+
		// equation:
		//  vx - vg     vbc - vx
		// --------- = ----------  ==> vx - vg = (vbc - vx) * (dx + dg) / dx;
		//  dx + dg        dx          vg = vx - (vbc - vx) * (dx + dg) / dx;
		Vt dx  = std::abs(fc.grid().c_(axe, idxb) - fp[axe]);
		Vt dg  = std::abs(fc.grid().c_(axe, idxg) - fp[axe]);
		Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
		Vt vx  = fc(idxb);
		return vx - (vbc - vx) * (dx + dg) / dx;
	}

	static Vt _value_type2(
			        const Field&      fc,
			        const BC&          bc,
					const Index&       idxc,
					const Index&       idxg,
					const St&          axe,
					const St&          ori,
					const Vt&          time = 0.0){
		// boundary condition must be type 2
		// walk back
		auto oori = Opposite(Orientation(ori));
		auto idxb = idxg.shift(axe, oori);
		int step  = 0;
		while(fc.ghost().is_ghost(idxb)){
			Shift(idxb, axe, oori);
			step++;
		}
		auto fp = fc.grid().f(axe, ori, idxb);
		for(int i = 0; i < step; ++i){
			Shift(idxb, axe, oori);
		}
		ASSERT(fc.ghost().is_normal(idxb));
		//  idxb   face  ghost
		// ---x-----|-----g-----
	    //    +--dx-+--dg-+
		// equation:
		//  vx - vg
		// --------- = vbc  ==> vx - vg = vbc * (dx + dg);
		//  dx + dg                  vg = vx - vbc * (dx + dg);
		Vt dx = std::abs(fc.grid().c_(axe, idxb) - fp[axe]);
		Vt dg = std::abs(fc.grid().c_(axe, idxg) - fp[axe]);
		Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
		Vt vx = fc(idxb);
		return vx - vbc * (dx + dg);
	}

};

}

#endif
