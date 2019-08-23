#ifndef _S_GHOST_BY_FUNCTION_HPP_
#define _S_GHOST_BY_FUNCTION_HPP_

#include "domain/domain_define.hpp"
#include "domain/structure/structure_define.hpp"
#include "domain/structure/index.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/mask.hpp"
#include "ghost.hpp"

namespace carpio{
template<typename TYPE, St DIM>
class SCreatGhostByFunction{
public:
	static const St Dim = 2;
	static const St NumFaces = 4;
	static const St NumVertexes = 4;

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

	typedef SGhostMask_<DIM>               GhostMask;
	typedef std::shared_ptr<GhostMask>   spGhostMask;

	typedef std::function<Vt(Vt, Vt, Vt, Vt)> FunXYZT_Value;

	spGhostMask ghost_mask(spGrid spg, FunXYZT_Value fun, Vt th){

	}

};

}

#endif
