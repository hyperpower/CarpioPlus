#ifndef _S_U_DOT_NABLA_PHI_HPP
#define _S_U_DOT_NABLA_PHI_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/boundary/boundary_index.hpp"

#include "domain/structure/operation/soperation.hpp"

#include <array>

namespace carpio{


template<St DIM>
class SUdotNabla_{
public:
    typedef SGrid_<DIM>   Grid;
    typedef SGhost_<DIM>  Ghost;
    typedef SOrder_<DIM>  Order;
    typedef SField_<DIM> Field;

    typedef SVectorCenter_<DIM> VectorCenter;
    typedef SVectorFace_<DIM>   VectorFace;

    typedef SExpField_<DIM>     ExpField;

protected:
    typedef std::shared_ptr<BoundaryIndex> spBI;

    spBI _spbi;
public:

    SUdotNabla_(){
        _spbi = nullptr;
    }

    SUdotNabla_(spBI spbi) : _spbi(spbi){
    }

    void set_boundary_index(spBI spbi){
        ASSERT(spbi != nullptr);
        this->_spbi = spbi;
    }

    virtual ~SUdotNabla_(){

    }

    virtual Field operator()(
            const VectorFace& U,
            const Field&     phi,
            const Vt&         t = 0.0){

    }
    virtual ExpField operator()(
            const VectorFace& U,
            const ExpField& phi,
            const Vt&         t = 0.0){
    }

};


template<St DIM>
class SUdotNabla_FOU: public SUdotNabla_<DIM>{
public:
    typedef SIndex_<DIM>  Index;
    typedef SGrid_<DIM>   Grid;
    typedef SGhost_<DIM>  Ghost;
    typedef SOrder_<DIM>  Order;
    typedef SField_<DIM>  Field;

    typedef SUdotNabla_<DIM> Base;
    typedef SUdotNabla_FOU<DIM> Self;

    typedef SVectorCenter_<DIM> VectorCenter;
    typedef SVectorFace_<DIM>   VectorFace;
    typedef BoundaryIndex       BI;
    typedef std::shared_ptr<BoundaryIndex> spBI;

    typedef SExpField_<DIM>                      ExpField;
    typedef typename SExpField_<DIM>::Expression Exp;

    typedef SValue_<DIM> Value;

    typedef SLoop_<DIM> Loop;

    SUdotNabla_FOU(): Base(){
    }

    SUdotNabla_FOU(spBI spbi) : Base(spbi){
    }

    virtual ~SUdotNabla_FOU(){
    }

    static Vt Local(const Index& idx,
                    const VectorFace& U,
                    const Field& phi,
                    const BI&    bi,
                    const Vt&    t = 0.0){
        std::array<Vt, DIM> arr;
        arr.fill(0.0);

        FOR_EACH_DIM
        {
            Vt up = U(d, _P_, idx);
            Vt um = U(d, _M_, idx);
            Vt uc = (up + um) * 0.5;      //average velocity to center
            auto idxm = idx.m(d);
            auto idxp = idx.p(d);
            Vt phi_u, phi_d;
            Vt s = phi.grid().s_(d, idx);
            if (uc >= 0) {
                phi_u = Value::Get(phi, bi, idx, idxm, d, _M_, t);
                phi_d = phi(idx);

            } else { // uc < 0
                phi_u = phi(idx);
                phi_d = Value::Get(phi, bi, idx, idxp, d, _P_, t);
            }
            arr[d] = uc * (phi_u - phi_d) / s;
        }

        Vt sum = 0;
        FOR_EACH_DIM
        {
            sum += arr[d];
        }
        return sum;
    }

    virtual Field operator()(
            const VectorFace& U,
            const Field& phi,
            const Vt& t = 0.0){
        Field res = phi.new_compatible();
        for (auto& idx : phi.order()) {
            std::array<Vt, DIM> arr;
            arr.fill(0.0);

            FOR_EACH_DIM
            {
                Vt up = U(d, _P_, idx);
                Vt um = U(d, _M_, idx);
                Vt uc = (up + um) * 0.5;      //average velocity to center
                auto idxm = idx.m(d);
                auto idxp = idx.p(d);
                Vt phi_u, phi_d;
                Vt s      = phi.grid().s_(d, idx);
                if (uc >= 0) {
                    phi_u = Value::Get(
                            phi, *(this->_spbi),
                            idx, idxm, d, _M_, t);
                    phi_d = phi(idx);

                } else { // uc < 0
                    phi_u = phi(idx);
                    phi_d = Value::Get(
                            phi, *(this->_spbi),
                            idx, idxp, d, _P_, t);
                }
                arr[d] = uc * (phi_u - phi_d) / s;
            }

            Vt sum = 0;
            FOR_EACH_DIM{
                sum += arr[d];
            }
            res(idx) = sum;
        }

        return res;
    }

    ExpField operator()(
                const VectorFace& U,
                const ExpField&   phi,
                const Vt&         t = 0.0){
        ExpField res = phi.new_compatible();
        for (auto& idx : phi.order()) {
            std::array<Exp, DIM> arr;
            FOR_EACH_DIM
            {
                Vt up = U(d, _P_, idx);
                Vt um = U(d, _M_, idx);
                Vt uc = (up + um) * 0.5;      //average velocity to center
                auto idxm = idx.m(d);
                auto idxp = idx.p(d);
                Exp phi_u, phi_d;
                Vt s = phi.grid().s_(d, idx);
                if (uc >= 0) {
                    if(phi.ghost().is_ghost(idxm)){
                        phi_u += Value::Get(phi,
                                *(this->_spbi),
                                idx, idxm,
                                d, _M_, t);
                    }else{
                        phi_u += idxm;
                    }
                    phi_d += idx;
                } else { // uc < 0
                    phi_u += phi(idx);
                    if(phi.ghost().is_ghost(idxp)){
                        phi_d += Value::Get(phi, *(this->_spbi),
                                idx, idxp, d, _P_,t);
                    }else{
                        phi_d += idxp;
                    }
                }
                arr[d] = (phi_u - phi_d) * (uc / s);
            }

            Vt sum = 0;
            FOR_EACH_DIM
            {
//                sum += arr[d];
            }
            res(idx) = sum;
        }
    }

};

template<St DIM>
class SUdotNabla_TVD: public SUdotNabla_<DIM>{
public:
    typedef SGrid_<DIM>   Grid;
    typedef SGhost_<DIM>  Ghost;
    typedef SOrder_<DIM>  Order;
    typedef SField_<DIM> Field;
    typedef SIndex_<DIM>  Index;
    typedef SUdotNabla_<DIM> Base;


    typedef SVectorCenter_<DIM> VectorCenter;
    typedef SVectorFace_<DIM>   VectorFace;
    typedef std::shared_ptr<BoundaryIndex> spBI;

    typedef SValue_<DIM> Value;

    typedef Vt (*Limiter)(Vt, Vt);

protected:
    Limiter lim;
public:

    SUdotNabla_TVD(): Base(){
        // default limiter
        lim = SUdotNabla_TVD::_limiter_QUICK;
    }

    SUdotNabla_TVD(spBI spbi) : Base(spbi){
        lim = SUdotNabla_TVD::_limiter_QUICK;
    }

    virtual ~SUdotNabla_TVD(){
    }

    // Improved total variation diminishing schemes for advection
    // simulation on arbitrary grids
    // J. Hou , F. Simons and R. Hinkelmann
    // Int. J. Numer. Meth. Fluids 2012; 70:359–382
    virtual Field operator()(const VectorFace& U, const Field& phi, const Vt& t = 0.0){
        Field res = phi.new_compatible();
        for (auto& idx : phi.order()) {
            std::array<Vt, DIM> arr;
            arr.fill(0.0);

            FOR_EACH_DIM
            {
                Index idxp = idx.p(d);
                Index idxm = idx.m(d);
                Vt up = U(d, _P_, idx);
                Vt um = U(d, _M_, idx);
                Vt uc = (up + um) * 0.5;      //average velocity to center
                Index Cm,Um,Dm,Cp,Up,Dp;
                Vt fp,fm,R,r;
                Vt VDm, VCm, VUm, VDp, VCp, VUp;
                if (uc >= 0.0) {
                    // fm ------------------
                    Dm = idx;
                    Cm = idxm;
                    Um = idxm.m(d);
                    // get value
                    VDm = phi(idx);
                    VCm = Value::Get(phi, *(this->_spbi), idx, Cm, d, _M_, t);
                    VUm = Value::Get(phi, *(this->_spbi), idx, Um, d, _M_, t);
                    // fp ------------------
                    Dp = idxp;
                    Cp = idx;
                    Up = idxm;
                    // get value
                    VDp = Value::Get(phi, *(this->_spbi), idx, Dp, d, _P_, t);
                    VCp = VDm;
                    VUp = VCm;
                } else if (uc < 0.0) {
                    // fm ------------------
                    Dm = idxm;
                    Cm = idx;
                    Um = idxp;
                    // get value
                    VDm = Value::Get(phi, *(this->_spbi), idx, Dm, d, _M_, t);
                    VCm = phi(idx);
                    VUm = Value::Get(phi, *(this->_spbi), idx, Um, d, _P_, t);
                    // fp ------------------
                    Dp = idx;
                    Cp = idxp;
                    Up = idxp.p(d);
                    // get value
                    VDp = VCm;
                    VCp = VUm;
                    VUp = Value::Get(phi, *(this->_spbi), idx, Up, d, _P_, t);
                }
                // p -----------------------
                R = _RCD(phi, d, Cp, Dp);
                r = _rCD(phi, d, Up, Cp, Dp, VUp, VCp, VDp);
//                fp = VCp + lim(r, R) / R * (VDp - VCp);
                // m ---------------------
                R = _RCD(phi, d, Cm, Dm);
                r = _rCD(phi, d, Um, Cm, Dm, VUm, VCm, VDm);
//                fm = VCm + lim(r, R) / R * (VDm - VCm);
                //
                arr[d] = (up * fp - um * fm)
                        / phi.grid().s_(d, idx);
            }

            Vt sum = 0;
            FOR_EACH_DIM{
                sum += arr[d];
            }
            res(idx) = sum;
        }

        return res;
    }

protected:

    static Vt _rCD(
                const Field& phi, St d,
                const Index&  U,
                const Index&  C,
                const Index&  D,
                const Vt&     VU,
                const Vt&     VC,
                const Vt&     VD) {
            const Grid& grid = phi.grid();
            Vt sU = grid.s_(d, U);
            Vt sC = grid.s_(d, C);
            Vt sD = grid.s_(d, D);
            return (VC - VU) * (sD + sC) / (VD - VC + SMALL) / (sC + sU);
        }

    static Vt _RCD(const Field& phi, St d, const Index& C, const Index& D) {
        const Grid& grid = phi.grid();
        Vt sC = grid.s_(d, C);
        Vt sD = grid.s_(d, D);
        return (sD + sC) / sC;
    }

    //  A review on TVD schemes and a refined flux-limiter
    //  for steady-state calculations
    //  Di Zhang, Chunbo Jiang, Dongfang Liang, Liang Cheng
    //  Journal of Computational Physics 302 (2015) 114–154
    //
    // k-scheme
    //            1 + k           1 - k
    // limiter = ------- r(CD) + -------
    //              2               2
    //          d(phi)/dx (CU)
    // r(CD) = ----------------
    //          d(phi)/dx (DC)
    //
    // SOU                      k = -1  (Second order upwind   upwind2)
    // Fromm                    k = 0
    // CUI                      k = 1/3
    // QUICK                    k = 1/2
    // CDS                      k = 1   (Center Difference Scheme,  center)

    static Vt _limiter_SOU(Vt r, Vt R) {
        return r + 1.0;
    }

    static Vt _limiter_Fromm(Vt r, Vt R){
        return 0.5 * r + 0.5;
    }

    static Vt _limiter_CUI(Vt r, Vt R) {
        return 2.0 / 3.0 * r + 1.0 / 3.0;
    }

    static Vt _limiter_QUICK(Vt r, Vt R){
        return 0.75 * r + 0.25;
    }

    static Vt _limiter_CDS(Vt r, Vt R) {
        return r;
    }




};


}


#endif
