#ifndef _S_INTERPOLATE_HPP
#define _S_INTERPOLATE_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/operation/soperation.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "svalue.hpp"
#include "algebra/algebra.hpp"


#include <array>

namespace carpio{


template<St DIM, class FIELD>
class SInterpolate_{
public:
    typedef FIELD  Field;
    typedef typename Field::Grid  Grid;
    typedef typename Field::Ghost Ghost;
    typedef typename Field::Order Order;
    typedef SIndex_<DIM>  Index;
    typedef std::shared_ptr<Field> spField;

    typedef SVectorCenter_<DIM, Grid, Ghost, Order> VectorCenter;
    typedef SVectorFace_<DIM, Grid, Ghost, Order>   VectorFace;
    typedef SCorner_<DIM, Grid, Ghost, Order>       Corner;
    typedef std::shared_ptr<BoundaryIndex> spBI;
    typedef BoundaryCondition BC;
    typedef SValue_<DIM, Field> Value;
    typedef SValue_<1, Field> Value1;
    typedef SValue_<2, Field> Value2;
    typedef SValue_<3, Field> Value3;

    typedef AInterpolate_<Vt, Vt> AInter;

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

protected:
    typedef SField_<1, Grid, Ghost, Order>  Field1;
    typedef SField_<2, Grid, Ghost, Order>  Field2;
    typedef SField_<3, Grid, Ghost, Order>  Field3;

    typedef SCorner_<1, Grid, Ghost, Order> Corner1;
    typedef SCorner_<2, Grid, Ghost, Order> Corner2;
    typedef SCorner_<3, Grid, Ghost, Order> Corner3;
public:
    static void CenterToCorner(const Field1& f, Corner1& c, spBI bi){
        for(auto& idx : f.order()){
            auto idxm = idx.m(_X_);
            auto v    = f(idx);
            auto vm   = Value1::Get(f, *(bi), idx, idxm, _X_, _M_);
            auto hs   = f.grid().hs_(_X_, idx);
            auto hsm  = f.grid().hs_(_X_, idxm);

            // m case
            c(0, idx) = (v * hsm + vm * hs) / (hs + hsm);
            // p case, just for the last one
            if (c.ghost().is_boundary(idx, _X_, _P_)) {
                auto idxp = idx.p(_X_);
                auto hsp  = f.grid().hs_(_X_, idxp);
                auto vp   = Value1::Get(f, *(bi), idx, idxp, _X_, _P_);
//                std::cout << "vp = "  << vp << std::endl;
                c(1, idx) = (v * hsp + vp * hs) / (hs + hsp);
            }
        }
    }

    static void CenterToCorner(const Field2& f, Corner2& c, spBI bi){
        auto& grid = f.grid();
        for (auto& idx : f.order()) {
            // get index
            auto idxmx = idx.m(_X_);
            auto idxmy = idx.m(_Y_);
            auto idxmc = idxmy.m(_X_);
            // get point
            auto p    = f.grid().c(idx);     // point center
            auto pc   = f.grid().v(0, idx);  // mm corner
            auto pmc  = f.grid().c(idxmc);   // point center of cell mm
            // get value
            auto v    = f(idx);
            auto vmx  = Value2::Get(f, *(bi), idx, idxmx, _X_, _M_);
            auto vmy  = Value2::Get(f, *(bi), idx, idxmy, _Y_, _M_);
            if(f.ghost().is_ghost(idxmc)){
                if(grid.type_name() == "SGridUniform"){
                    c(0, idx) = (v + vmx + vmy) / 3.0;
                }else{
                    SHOULD_NOT_REACH; //unfinish
                }
            }else{
                if(grid.type_name() == "SGridUniform"){
                    c(0, idx) = (v + vmx + vmy + f(idxmc)) / 4.0;
                }else{
                    auto vmc = f(idxmc);
                    AInter::Bilinear(pc.x(), pc.y(),
                        pmc.x(), pmc.y(),
                        p.x(), p.y(),
                        vmc, vmx, v, vmy);
                    SHOULD_NOT_REACH; //unfinish
                }
            }
            // p direction is not finished
        }
    }

};


}

#endif
