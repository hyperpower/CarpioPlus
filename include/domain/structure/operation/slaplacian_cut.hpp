#ifndef _S_LAPLACIAN_CUT_HPP
#define _S_LAPLACIAN_CUT_HPP

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

#include <array>

namespace carpio{

template<St DIM, class FIELD>
class SLaplacianCut_: public SLaplacian_<DIM, FIELD>{
public:
	typedef SLaplacian_<DIM, FIELD> Base;
    typedef FIELD  Field;
    typedef typename Field::Grid  Grid;
    typedef typename Field::Ghost Ghost;
    typedef typename Field::Order Order;
    typedef SIndex_<DIM>     Index;

    typedef SGhostLinearCut_<DIM, Grid> GhostLinearCut;
    typedef std::shared_ptr<GhostLinearCut> spGhostLinearCut;
    typedef typename GhostLinearCut::spCell spCell;

    typedef SVectorCenter_<DIM, Grid, Ghost, Order> VectorCenter;
    typedef SVectorFace_<DIM, Grid, Ghost, Order>   VectorFace;

    typedef SExpField_<DIM, Grid, Ghost, Order>                      ExpField;
    typedef typename ExpField::Expression Exp;
    typedef BoundaryIndex       BI;

    typedef SValue_<DIM, Field> Value;
    typedef SValueCut_<DIM, Field> ValueCut;

protected:
    typedef std::shared_ptr<BoundaryIndex> spBI;
public:

    SLaplacianCut_(){
        this->_spbi = nullptr;
    }

    SLaplacianCut_(spBI spbi) : Base(spbi){
    }

    virtual ~SLaplacianCut_(){
    }

    virtual Field operator()(const Field& phi,
                             const Vt& t = 0.0) {
        Field res        = phi.new_compatible();
        const Grid& grid = phi.grid();
        for (auto& idx : phi.order()) {
        	std::cout << "Index = " << idx << std::endl;
            std::array<Vt, DIM> arr;
            arr.fill(0.0);
            Vt sum = 0;

            FOR_EACH_DIM
            {
                sum += gradient_face_times_area(phi, idx, d, _M_, t);
                sum += gradient_face_times_area(phi, idx, d, _P_, t);
            }

            sum += gradient_front_times_area(phi, idx, t);
            res(idx) = sum;
        }

        return res;
    }

    Vt gradient_face_times_area(const Field& phi,
    		                   const Index& idx, St d, St o, Vt t){
    	auto spg  = phi.spghost();
    	auto gc   = std::dynamic_pointer_cast<GhostLinearCut>(spg);
    	auto idxn = idx.shift(d, o);
    	auto pcell = gc->operator()(idx);  // this cell
		if (pcell == nullptr) { // normal cell
			Vt gf = gradient_full_face(phi, idx, idxn, d, o, t);
			Vt a  = phi.grid().fa(d, o, idx);
			return gf * a;
		}else if (pcell->type() == _CUT_){
			std::cout <<idx << " CUT " << d << " o " << o <<std::endl;
		}
    }
    Vt gradient_front_times_area(const Field& phi,
    		                     const Index& idx, Vt t){
    	return 0.0;
    }

    Vt gradient_full_face(const Field& phi,
    		              const Index& idx,   // center index
						  const Index& idxn,  // index next
						  St           d,     // direction
						  St           o,     // orientation
						  Vt           t){    // time
    	auto spg  = phi.spghost();
    	auto gc   = std::dynamic_pointer_cast<GhostLinearCut>(spg);
    	auto pcell  = gc->operator()(idx);   // this cell is normal
    	auto pcelln = gc->operator()(idxn);  // neighbor cell
    	auto grid = phi.grid();
    	// neighbor cell is normal
    	if (pcelln == nullptr){
    		Vt gf   = (phi(idxn) - phi(idx)) /
    				std::abs((grid.c_(d, idxn) - grid.c_(d, idx)));
    		return gf;
    	}else{// neighbor cell is cut or ghost
    		if(pcelln->type() == _CUT_){

    		}else if(pcelln->type() == _GHOST_){

    		}
    	}
    }
    Vt gradient_partial_face(const Field& phi,
    		                 const Index& idx,   // center index
						     const Index& idxn,  // index next
						     St           d,     // direction
						     St           o,     // orientation
						     Vt           t){    // time

    }
    virtual ExpField expression_field(
                                const Field&    phi,
                                const Vt&       t = 0.0){
        ExpField res(phi.spgrid(), phi.spghost(), phi.sporder());
		const Grid& grid = phi.grid();
		auto spghost = std::dynamic_pointer_cast<GhostLinearCut>(phi.spghost());
		Exp sum;
		for (auto& idx : phi.order()) {
			std::cout << "Index = " << idx
					  << "type : "  << spghost->type(idx)
					  << " ==============" << std::endl;

			FOR_EACH_DIM
			{
				sum += exp_gradient_face_times_area(phi, idx, d, _M_, t);
				sum += exp_gradient_face_times_area(phi, idx, d, _P_, t);
			}

//			sum += gradient_front_times_area(phi, idx, t);
//			res(idx) = sum;
		}

		return res;
    }

    Exp exp_gradient_face_times_area(const Field& phi,
    		                         const Index& idx, St d, St o, Vt t){
    	std::cout << "  gradient f*a = " << d << " on " << o << std::endl;
    	auto spg   = phi.spghost();
    	auto gc    = std::dynamic_pointer_cast<GhostLinearCut>(spg);
    	auto idxn  = idx.shift(d, o);
    	auto pcell = gc->operator()(idx);  // this cell
		if (pcell == nullptr) { // normal cell
			Exp gf = exp_gradient_full_face(phi, idx, idxn, d, o, t);
			Vt  a  = phi.grid().fa(d, o, idx);
			return gf * a;
		} else if (pcell->type() == _CUT_) {
			auto ap = pcell->aperture_ratio(d, o);
			if(IsCloseToZero(ap)){ // full face
				std::cout << "full face = " << ap <<  std::endl;
				Exp gf = exp_gradient_full_face(phi, idx, idxn, d, o, t);
				Vt  a  = phi.grid().fa(d, o, idx);
				return gf * a;
			}else if(IsCloseTo(std::abs(ap), 1.0)){ // ghost face
				std::cout << "ghost face " << ap << std::endl;
				return Exp(0.0);
			}else{  // partial face
				std::cout << "partial face " << ap << std::endl;
			}
			return Exp(idx);
		}
    }

    Exp exp_gradient_full_face(
    		              const Field& phi,
       		              const Index& idx,   // center index
   						  const Index& idxn,  // index next
   						  St           d,     // direction
   						  St           o,     // orientation
   						  Vt           t){    // time
    	std::cout << "  full face " << idx << " n " << idxn << std::endl;
		auto spg    = phi.spghost();
		auto gc     = std::dynamic_pointer_cast<GhostLinearCut>(spg);
		auto pcell  = gc->operator()(idx);   // this cell is normal
		auto pcelln = gc->operator()(idxn);  // neighbor cell
		auto spgrid = phi.spgrid();
    	ValueCut value;
		// neighbor cell is normal
		if (pcelln == nullptr) {  // normal cell
			Exp expn(idxn);
			Exp exp(idx);
			auto gf = (expn - exp)
					/ std::abs((spgrid->c_(d, idxn) - spgrid->c_(d, idx)));
			return gf;
		} else {  // neighbor cell is cut or ghost
			auto expn = value.get_expression(phi,(*this->_spbi), idx, idxn, d, o, t);
			Exp  exp(idx);
			auto gf = (expn - exp) / Distance(spgrid->c(idxn), spgrid->c(idx));
			return gf;
		}
	}

    Exp exp_gradient_partial_face(
    		              const Field& phi,
       		              const Index& idx,   // center index
   						  const Index& idxn,  // index next
   						  St           d,     // direction
   						  St           o,     // orientation
   						  Vt           t){    // time
    	// idx is cut cell
    	std::cout << "  partial face " << idx << " n " << idxn << std::endl;
		auto spg    = phi.spghost();
		auto gc     = std::dynamic_pointer_cast<GhostLinearCut>(spg);
		auto pcell  = gc->operator()(idx);   // this cell is normal
		auto pcelln = gc->operator()(idxn);  // neighbor cell
		auto spgrid = phi.spgrid();
    	ValueCut value;
		// neighbor cell is normal
		if (pcelln == nullptr) {  // normal cell

		} else {  // neighbor cell is cut or ghost

		}
	}


    virtual ExpField expression_field(
                                    const Field&    phis,
                                    const Vt&       beta,
                                    const Vt&       t = 0.0){

    }
};

}
#endif
