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


template<St DIM>
class SLaplacianCut_: public SLaplacian_<DIM>{
public:
//	typedef SOperation_<DIM> Base;
    typedef SGrid_<DIM>      Grid;
    typedef SGhost_<DIM>     Ghost;
    typedef SOrder_<DIM>     Order;
    typedef SField_<DIM>     Field;
    typedef SIndex_<DIM>     Index;

    typedef SGhostLinearCut_<DIM> GhostLinearCut;
    typedef std::shared_ptr<SGhostLinearCut_<DIM> > spGhostLinearCut;
    typedef typename GhostLinearCut::spCell spCell;



    typedef SVectorCenter_<DIM> VectorCenter;
    typedef SVectorFace_<DIM>   VectorFace;

    typedef SExpField_<DIM>                      ExpField;
    typedef typename SExpField_<DIM>::Expression Exp;
    typedef BoundaryIndex       BI;

    typedef SValue_<DIM> Value;
    typedef SValueCut_<DIM> ValueCut;

protected:
    typedef std::shared_ptr<BoundaryIndex> spBI;

    spBI _spbi;
public:

    SLaplacianCut_(){
        _spbi = nullptr;
    }

    SLaplacianCut_(spBI spbi) : _spbi(spbi){
    }

    void set_boundary_index(spBI spbi){
        ASSERT(spbi != nullptr);
        this->_spbi = spbi;
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
                                const Field&    phis,
                                const Vt&       t = 0.0){
        ExpField res(phis.spgrid(), phis.spghost(), phis.sporder());
		const Grid& grid = phis.grid();
		auto spghost = std::dynamic_pointer_cast<GhostLinearCut>(phis.spghost());
		for (auto& idx : phis.order()) {
			std::cout << "Index = " << idx << "type : " << spghost->type(idx)<< std::endl;
			std::array<Exp, DIM> arr;

			FOR_EACH_DIM
			{
//				sum += exp_gradient_face_times_area(phi, idx, d, _M_, t);
//				sum += exp_gradient_face_times_area(phi, idx, d, _P_, t);
			}

//			sum += gradient_front_times_area(phi, idx, t);
//			res(idx) = sum;
		}

		return res;
    }

    Exp exp_gradient_face_times_area(const Field& phi,
    		                         const Index& idx, St d, St o, Vt t){
    	auto spg   = phi.spghost();
    	auto gc    = std::dynamic_pointer_cast<GhostLinearCut>(spg);
    	auto idxn  = idx.shift(d, o);

    	auto pcell = gc->operator()(idx);  // this cell
		if (pcell == nullptr) { // normal cell
			Vt gf = exp_gradient_full_face(phi, idx, idxn, d, o, t);
			Vt a  = phi.grid().fa(d, o, idx);
			return gf * a;
		} else if (pcell->type() == _CUT_) {
			std::cout << idx << " CUT " << d << " o " << o << std::endl;
		}
    }

    Exp exp_gradient_full_face(const Field& phi,
       		              const Index& idx,   // center index
   						  const Index& idxn,  // index next
   						  St           d,     // direction
   						  St           o,     // orientation
   						  Vt           t){    // time
		auto spg    = phi.spghost();
		auto gc     = std::dynamic_pointer_cast<GhostLinearCut>(spg);
		auto pcell  = gc->operator()(idx);   // this cell is normal
		auto pcelln = gc->operator()(idxn);  // neighbor cell
		auto grid   = phi.grid();
		// neighbor cell is normal
		Exp expn(idxn);
		Exp exp(idx);
		if (pcelln == nullptr) {
			auto gf = (expn - exp)
					/ std::abs((grid.c_(d, idxn) - grid.c_(d, idx)));
			return gf;
		} else {  // neighbor cell is cut or ghost
			if (pcelln->type() == _CUT_) {

			} else if (pcelln->type() == _GHOST_) {

			}
		}
	}

    virtual ExpField expression_field(
                                    const Field&    phis,
                                    const Vt&       beta,
                                    const Vt&       t = 0.0){
        ExpField res(phis.spgrid(), phis.spghost(), phis.sporder());
        const Grid& grid = phis.grid();
        for (auto& idx : phis.order()) {
            std::array<Exp, DIM> arr;
            FOR_EACH_DIM
            {
                Index idxp = idx.p(d);
                Index idxm = idx.m(d);
                Exp phi_m, phi_p;
                Exp phi(idx);
                if (phis.ghost().is_ghost(idxm)) {
                    phi_m += Value::GetExp(phis, *(this->_spbi), idx, idxm, d, _M_, t);
                } else {
                    phi_m += idxm;
                }
                if (phis.ghost().is_ghost(idxp)) {
                    phi_p += Value::GetExp(phis, *(this->_spbi), idx, idxp, d, _P_, t);
                } else {
                    phi_p += idxp;
                }
                auto dfdx_m = (phi - phi_m) * beta
                        / (grid.c_(d, idx) - grid.c_(d, idxm));
                auto dfdx_p = (phi_p - phi) * beta
                        / (grid.c_(d, idxp) - grid.c_(d, idx));

                arr[d] = (dfdx_p * grid.fa(d, _P_, idx)
                        - dfdx_m * grid.fa(d, _M_, idx));

            }
            FOR_EACH_DIM
            {
                res(idx) += arr[d];
            }
        }
        return res;
    }
};

}
#endif
