#ifndef _S_OPERATION_HPP
#define _S_OPERATION_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/grid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/scalar.hpp"
#include "domain/structure/field/vector_center.hpp"
#include "domain/structure/field/vector_face.hpp"

#include "domain/boundary/boundary_condition.hpp"
#include "domain/boundary/bc_value.hpp"
#include "domain/boundary/boundary_index.hpp"

#include <array>

namespace carpio{
template<St DIM>
Vt _value_type1(const SScalar_<DIM>      fc,
		        const BoundaryCondition& bc,
				const SIndex_<DIM>&  idxc,
				const SIndex_<DIM>&  idxg,
				const Axes&          axe,
				const Orientation&   ori,
				const Vt&            time = 0.0){
	// boundary condition must be type 1
	// walk back
	auto oori = Opposite(ori);
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
	//  vx - vg     vbc - vx
	// --------- = ----------  ==> vx - vg = (vbc - vx) * (dx + dg) / dx;
	//  dx + dg        dx          vg = vx - (vbc - vx) * (dx + dg) / dx;
	Vt dx  = std::abs(fc.grid().c_(axe, idxb) - fp[axe]);
	Vt dg  = std::abs(fc.grid().c_(axe, idxg) - fp[axe]);
	Vt vbc = bc.value(fp.value(_X_), fp.value(_Y_), fp.value(_Z_), time);
	Vt vx  = fc(idxb);
	return vx - (vbc - vx) * (dx + dg) / dx;
}


template<St DIM>
Vt _value_type2(const SScalar_<DIM>      fc,
		        const BoundaryCondition& bc,
				const SIndex_<DIM>&  idxc,
				const SIndex_<DIM>&  idxg,
				const Axes&          axe,
				const Orientation&   ori,
				const Vt&            time = 0.0){
	// boundary condition must be type 2
	// walk back
	auto oori = Opposite(ori);
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


template<St DIM>
Vt Value(const SScalar_<DIM>& fc,
		 const BoundaryIndex& bi,
		 const SIndex_<DIM>&  idxc,
		 const SIndex_<DIM>&  idxg,
		 const Axes&          axe,
		 const Orientation&   ori,
		 const Vt&            time = 0.0){
	typedef BoundaryCondition BC;
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

template<St DIM>
void VectorCenterToVectorFace(
		const SVectorCenter_<DIM>& vc,
		      SVectorFace_<DIM>&   vf){
	for(auto& idx : vc.order()){
		for(St d =0; d< DIM; d++){
			auto idxm = idx.m(d);
			auto idxp = idx.p(d);

			if(vc.ghost().is_ghost(idxm)){

			}

		}
	}
}

}
#endif
