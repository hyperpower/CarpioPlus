#ifndef _S_INTERPOLATE_HPP
#define _S_INTERPOLATE_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "svalue.hpp"

#include <array>

namespace carpio{


template<St DIM>
class SInterpolate_{
public:
	typedef SGrid_<DIM>   Grid;
	typedef SGhost_<DIM>  Ghost;
	typedef SOrder_<DIM>  Order;
	typedef SField_<DIM> Field;
	typedef SIndex_<DIM>  Index;
	typedef std::shared_ptr<Field> spField;

	typedef SVectorCenter_<DIM> VectorCenter;
	typedef SVectorFace_<DIM>   VectorFace;
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryCondition BC;
	typedef SValue_<DIM> Value;

	static void VectorCenterToFace(
			const VectorCenter& vc,
			      VectorFace&   vf,
		          spBI          bi,
			      spBI          bj = nullptr,
				  spBI          bk = nullptr) {
		spBI abi[] = { bi, bj, bk };
		for (auto& idx : vc.order()) {
			for (St d = 0; d < DIM; d++) {
				auto idxm = idx.m(d);
				auto v    = vc[d](idx);
				auto vm   = Value::Get(vc[d], *(abi[d]), idx, idxm, d, _M_);
				auto hs   = vc[d].grid().hs_(d, idx);
				auto hsm  = vc[d].grid().hs_(d, idxm);
				// m case
				vf(Axes(d), _M_, idx) = (v * hsm + vm * hs) / (hs + hsm);
				// p case, just for the last one
				if (vc.ghost().is_boundary(idx, d, _P_)) {
					auto idxp = idx.p(d);
					auto hsp  = vc[d].grid().hs_(d, idxp);
					auto vp   = Value::Get(vc[d], *(abi[d]), idx, idxp, d, _P_);
					vf(d, _P_, idx) = (v * hsp + vp * hs) / (hs + hsp);
				}
			}
		}
	}




};


}

#endif
