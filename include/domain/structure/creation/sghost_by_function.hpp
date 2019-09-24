#ifndef _S_GHOST_BY_FUNCTION_HPP_
#define _S_GHOST_BY_FUNCTION_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "geometry/geometry.hpp"

#include <array>

namespace carpio{
template<typename TYPE, St DIM>
class SCreatGhostByFunction_{
public:
	static const St Dim         = 2;
    static const St NumVertex = DIM == 1 ? 2 : (DIM == 2 ? 4 : 8);
	static const St NumFace   = DIM == 1 ? 2 : (DIM == 2 ? 4 : 6);
	static const St NumEdge   = DIM == 1 ? 1 : (DIM == 2 ? 4 : 12);


	typedef SIndex_<DIM>                Index;
	typedef SGrid_<DIM>                 Grid;
	typedef Grid&                       ref_Grid;
	typedef const Grid&                 const_ref_Grid;
	typedef Grid*                       pGrid;
	typedef const Grid*                 pcGrid;
	typedef std::shared_ptr<Grid>       spGrid;
	typedef std::shared_ptr<const Grid> spcGrid;

	typedef SGhost_<DIM>                 Ghost;
	typedef Ghost&                       ref_Ghost;
	typedef const Ghost&                 const_ref_Ghost;
	typedef Ghost*                       pGhost;
	typedef const Ghost*                 pcGhost;
	typedef std::shared_ptr<Ghost>       spGhost;
	typedef std::shared_ptr<const Ghost> spcGhost;

	typedef SGhostRegular_<DIM>                       GhostRegular;
	typedef std::shared_ptr<SGhostRegular_<DIM> >   spGhostRegular;
	typedef SOrder_<DIM>                           Order;
	typedef std::shared_ptr<SOrder_<DIM> >       spOrder;
	typedef SOrderXYZ_<DIM>                        OrderXYZ;
	typedef std::shared_ptr<SOrderXYZ_<DIM> >    spOrderXYZ;

	typedef SGhostMask_<DIM>               GhostMask;
	typedef std::shared_ptr<GhostMask>   spGhostMask;
	typedef SCellMask_<DIM>               CellMask;
	typedef std::shared_ptr<CellMask>   spCellMask;

	typedef SGhostLinearCut_<DIM>              GhostLinearCut;
	typedef std::shared_ptr<GhostLinearCut>  spGhostLinearCut;
	typedef SCellLinearCut_<DIM>               CellLinearCut;
	typedef std::shared_ptr<CellLinearCut>   spCellLinearCut;

	typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;

	typedef SCorner_<DIM>              Corner;
	typedef std::shared_ptr<Corner>  spCorner;

	typedef typename std::conditional<DIM == 2,
                                      CuboidToolPL2_<TYPE>,
                                      CuboidToolPL3_<TYPE>>::type Tool;
	                                  // 3D is not correct

	SCreatGhostByFunction_(){
	}

	spGhostMask ghost_mask(spGrid spg, FunXYZT_Value fun, Vt time, Vt th, int bid = 10){
		spGhostMask spgm(new GhostMask(spg));
		spCellMask  spcm(new CellMask(bid));

		typename GhostMask::FunSetByXYZT fun2 = [spcm, fun, time, th](
			const Vt& x, const Vt& y, const Vt& z, const Vt& time){
			auto value = fun(x,y,z,time);
			if(value < th){
				return spcm;
			}else{
				return spCellMask(nullptr);
			}
		};
		spgm->set(fun2, time);
		return spgm;
	}

	spGhostLinearCut ghost_linear_cut(spGrid spg, FunXYZT_Value fun, Vt time, Vt th){
		spGhostLinearCut spgh(new GhostLinearCut(spg));

		spCellLinearCut spcg(new CellLinearCut());
		spcg->set_type(_GHOST_);

		auto spcorner = this->_corner_value(spg, fun, time);

		spGhostRegular spgr(new GhostRegular(spg));

		typename GhostLinearCut::FunSetByIndex funindex =
				[this, spg, spgr, fun,&time,&th,&spcg,&spcorner]
				 (const Index& index){
			if(spgr->is_ghost(index)){
				return spcg;
			}
			std::array<Vt, NumVertex> arrvv;
			auto& corner = *spcorner;
			short vf = 0;
			for(int vo = 0; vo < NumVertex; vo++){
				arrvv[vo] = corner(vo, index);
				if(arrvv[vo] >= th){
					vf++;
				}else{
					vf--;
				}
			}
			if(vf == NumVertex){
				// a normal cell
				return spCellLinearCut(nullptr);
			}else if(-vf == NumVertex){
				// a ghost cell
				return spcg;
			}else{
				// a cut cell
				if(DIM == 2){
					Vt tol = spg->min_size() * 1e-3;
					return this->_new_cell_linear_cut_2(*spg,
							index, time, th, fun, tol, arrvv);
				}
				SHOULD_NOT_REACH;
			}
		};

		spgh->set(funindex);

		return spgh;

	}

protected:
	spCellLinearCut _new_cell_linear_cut_2(
			const Grid& grid, const Index& index,
			const Vt&   time, const Vt& th, FunXYZT_Value fun,
			const Vt&   tol,  const std::array<Vt, NumVertex>& arrv){
		spCellLinearCut spcc(new CellLinearCut());
		spcc->set_type(_CUT_);
		auto pmin = grid.v(0, index);
		auto pmax = grid.v(3, index);  // only for 2D
		Tool t;
		std::array<Vt, NumFace> arre = t.cut_cell_aperture_ratios(
				pmin.value(_X_), pmin.value(_Y_),
				grid.s_(_X_, index), grid.s_(_Y_, index),
				time, th, fun, tol, arrv);
		short vf = 0;
		for(auto& ev : arre){
			if(IsCloseToZero(ev)){
				vf++;
			}
			if(IsCloseTo(ev,1.0) || IsCloseTo(ev,-1.0)){
				vf--;
			}
		}
		if(vf == NumVertex){
			// a normal cell
			return spCellLinearCut(nullptr);
		}else if(-vf == NumVertex){
			// a ghost cell
			spCellLinearCut spg(new CellLinearCut());
			spg->set_type(_GHOST_);
			return spg;
		} else {
			std::cout << "index = " << index << std::endl;  //---------------------
			for(auto& ev : arre){
				std::cout << "ev = " << ev << std::endl;
			}
			spCellLinearCut spc(new CellLinearCut());
			spc->set_type(_CUT_);
			spc->set_data(pmin, pmax, arre);
			return spc;
		}

	}
	spCorner _corner_value(spGrid spg, FunXYZT_Value fun, Vt time){
		spGhost  spgh(new GhostRegular(spg));
		spOrder  spo(new OrderXYZ(spg,spgh));
		spCorner spc(new Corner(spg, spgh, spo));
		spc->assign(fun, time);
		return spc;
	}



};

}

#endif
