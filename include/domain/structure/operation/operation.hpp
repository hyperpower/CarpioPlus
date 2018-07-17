#ifndef _S_U_DOT_NABLA_PHI_HPP
#define _S_U_DOT_NABLA_PHI_HPP


#include "structure_define.hpp"
#include "domain/structure/grid/grid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/scalar.hpp"
#include "domain/structure/field/vector_center.hpp"
#include "domain/structure/field/vector_face.hpp"

#include <array>

namespace carpio{

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
