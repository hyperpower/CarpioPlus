#ifndef _S_GHOST_BY_FUNCTION_HPP_
#define _S_GHOST_BY_FUNCTION_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "geometry/geometry.hpp"

namespace carpio{
template<typename TYPE, St DIM>
class SCreatGhostByFunction_{
public:
	static const St Dim = 2;
	static const St NumFaces = 4;
	static const St NumVertexes = 4;

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

	typedef SGhostLinearCut_<DIM>         GhostLinearCut;
	typedef std::shared_ptr<GhostMask>  spGhostLinearCut;
	typedef SCellLinearCut_<DIM>          CellLinearCut;
	typedef std::shared_ptr<CellMask>   spCellLinearCut;

	typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;

	typedef SCorner_<DIM>              Corner;
	typedef std::shared_ptr<Corner>  spCorner;

	typedef typename std::conditional<DIM == 2,
                                      CuboidToolPL_<TYPE>,
                                      CuboidToolPL_<TYPE>>::type Tool;
	                                  // 3D is not correct

	SCreatGhostByFunction_(){
	}

	spGhostMask ghost_mask(spGrid spg, FunXYZT_Value fun, Vt time, Vt th){
		spGhostMask spgm(new GhostMask(spg));
		spCellMask  spcm(new CellMask());

		typename GhostMask::FunSetByXYZT fun2 = [spcm, fun, time, th](
			const Vt& x, const Vt& y, const Vt& z, const Vt& time){
			auto value = fun(x,y,z,time);
			if(value < th){
				return spcm;
			}else{
				return spCellMask(nullptr);
			}
		};
		spgm->set_mask(fun2, time);
		return spgm;
	}

	spGhostLinearCut ghost_linear_cut(spGrid spg, FunXYZT_Value fun, Vt time, Vt th){
		spGhostLinearCut spgh(new GhostLinearCut());
		spCellLinearCut spcc(new CellLinearCut());
		spcc->set_type(_CUT_);
		spCellLinearCut spcg(new CellLinearCut());
		spcc->set_type(_GHOST_);

		auto spcorner = this->_corner_value(spg, fun, time);

		typename CellLinearCut::FunSetByIndex funindex =
				[spg,fun,&time,&th,&spcc,&spcg,&spcorner]
				 (const Index& index){

		};

	}

protected:
	spCorner _corner_value(spGrid spg, FunXYZT_Value fun, Vt time){
		spGhost  spgh(new GhostRegular(spg));
		spOrder  spo(new OrderXYZ(spg,spgh));
		spCorner spc(new Corner(spg, spgh, spo));
		spc->assgin(fun, time);
		return spc;
	}



};

}

#endif
