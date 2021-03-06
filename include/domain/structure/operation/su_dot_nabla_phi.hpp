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

#include "domain/structure/operation/soperation_define.hpp"

#include <array>

namespace carpio{


template<St DIM, class FIELD>
class SUdotNabla_{
public:
    typedef FIELD Field;
    typedef typename Field::Grid  Grid;
    typedef typename Field::Ghost Ghost;
    typedef typename Field::Order Order;

    typedef SVectorCenter_<DIM, Grid, Ghost, Order> VectorCenter;
    typedef SVectorFace_<DIM, Grid, Ghost, Order>   VectorFace;
    typedef SExpField_<DIM, Grid, Ghost, Order>     ExpField;

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

    virtual Field cal(
            const VectorFace& U,
            const Field&     phi,
            const Vt&         t = 0.0){

    }
    virtual ExpField cal(
            const VectorFace& U,
            const ExpField& phi,
            const Vt&         t = 0.0){
    }

};


template<St DIM, class FIELD>
class SUdotNabla_FOU: public SUdotNabla_<DIM, FIELD>{
public:
    typedef FIELD Field;
    typedef typename Field::Grid  Grid;
    typedef typename Field::Ghost Ghost;
    typedef typename Field::Order Order;
    typedef SIndex_<DIM>  Index;

    typedef SUdotNabla_<DIM, FIELD> Base;
    typedef SUdotNabla_FOU<DIM, FIELD> Self;

    typedef SVectorCenter_<DIM, Grid, Ghost, Order> VectorCenter;
    typedef SVectorFace_<DIM, Grid, Ghost, Order>   VectorFace;
    typedef BoundaryIndex       BI;
    typedef std::shared_ptr<BoundaryIndex> spBI;

    typedef SExpField_<DIM, Grid, Ghost, Order> ExpField;
    typedef typename ExpField::Expression Exp;

    typedef SValue_<DIM, Field> Value;

    typedef SLoop_<DIM, Field> Loop;

    SUdotNabla_FOU(): Base(){
    }

    SUdotNabla_FOU(spBI spbi) : Base(spbi){
    }

    virtual ~SUdotNabla_FOU(){
    }

    virtual Vt _local_one_dim(const Index&      idx,
                              const St&         d,
                              const VectorFace& U,
                              const Field&      phi,
                              const Vt&         t){
        Vt up = U(d, _P_, idx);
        Vt um = U(d, _M_, idx);
        Vt uc = (up + um) * 0.5;      //average velocity to center
        auto idxm = idx.m(d);
        auto idxp = idx.p(d);
        Vt phi_u, phi_d;
        if (uc >= 0) {
            phi_u = Value::Get(phi, *(this->_spbi), idx, idxm, d, _M_, t);
            phi_d = phi(idx);
            return (up * phi_u - um * phi_d) / (phi.grid().c_(d,idxm) - phi.grid().c_(d, idx));
        } else { // uc < 0
            phi_u = phi(idx);
            phi_d = Value::Get(phi, *(this->_spbi), idx, idxp, d, _P_, t);
            return (up * phi_u - um * phi_d) / (phi.grid().c_(d,idx) - phi.grid().c_(d, idxp));
        }

    }

    virtual Field cal(
               const VectorFace& U,
               const Field&      phi,
               const St&         d,
               const Vt&         t = 0.0){
        Field res = phi.new_compatible();
#ifdef OPENMP
        #pragma omp parallel for num_threads(phi.order().num_threads())
        for (St thread = 0; thread < phi.order().num_threads(); thread++) {
            for (auto iter = phi.order().begin(thread);
                    iter != phi.order().end(thread);
                    iter++) {
                auto& idx = (*iter);
                res(idx) = _local_one_dim(idx, d, U, phi, t);
            }
        }
#else
        for (auto& idx : phi.order()) {
            res(idx) = _local_one_dim(idx, d, U, phi, t);
        }
#endif
        return res;
    }

    virtual Field cal(
            const VectorFace& U,
            const Field& phi,
            const Vt& t = 0.0){
        Field res = phi.new_compatible();
        for (auto& idx : phi.order()) {
            std::array<Vt, DIM> arr;
            arr.fill(0.0);

            FOR_EACH_DIM
            {
                arr[d] = _local_one_dim(idx, d, U, phi, t);
            }

            Vt sum = 0;
            FOR_EACH_DIM{
                sum += arr[d];
            }
            res(idx) = sum;
        }

        return res;
    }

    ExpField cal_exp(
            const VectorFace& U,
            const Field& phi,
            const Vt& t = 0.0) {
        ExpField res(phi.spgrid(), phi.spghost(), phi.sporder());
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
                    phi_u = Value::GetExp(
                                phi, *(this->_spbi),
                                idx, idxm,
                                d,   _M_,  t);
                    phi_d = idx;
                    arr[d] = (up * phi_u - um * phi_d) / (phi.grid().c_(d, idxm) - phi.grid().c_(d, idx));
                } else { // uc < 0
                    phi_u = idx;
                    phi_d = Value::GetExp(
                                phi, *(this->_spbi),
                                idx, idxp,
                                d,   _P_,  t);
                    arr[d] = (up * phi_u - um * phi_d) / (phi.grid().c_(d, idx) - phi.grid().c_(d, idxp));
                }

            }

            Exp sum;
            FOR_EACH_DIM
            {
                sum += arr[d];
            }
            res(idx) = sum;
        }

        return res;
    }

};

template<St DIM, class FIELD>
class SUdotNabla_TVD: public SUdotNabla_<DIM, FIELD>{
public:
    typedef FIELD Field;
    typedef typename Field::Grid  Grid;
    typedef typename Field::Ghost Ghost;
    typedef typename Field::Order Order;
    typedef SIndex_<DIM>  Index;
    typedef SUdotNabla_<DIM, Field> Base;


    typedef SVectorCenter_<DIM, Grid, Ghost, Order> VectorCenter;
    typedef SVectorFace_<DIM, Grid, Ghost, Order>   VectorFace;
    typedef std::shared_ptr<BoundaryIndex> spBI;

    typedef SValue_<DIM, Field> Value;

    typedef Vt (*Limiter)(Vt, Vt);
    typedef std::unordered_map<std::string, Limiter> LimiterMap;

protected:
    Limiter lim;

    LimiterMap _map;
public:

    SUdotNabla_TVD(): Base(){
        // default limiter
        lim = SUdotNabla_TVD::_limiter_QUICK;
        _map = _build_limiter_map();
    }

    SUdotNabla_TVD(spBI spbi) : Base(spbi){
        lim = SUdotNabla_TVD::_limiter_QUICK;
        _map = _build_limiter_map();
    }
    SUdotNabla_TVD(spBI spbi, const std::string& scheme) : Base(spbi){
        _map = _build_limiter_map();
        this->set_scheme(scheme);
    }

    void set_scheme(const std::string& name){
        auto it = _map.find(name);
        if (it != _map.end()) {
            lim = _map[name];
        }else{
            std::cerr << " !> No such scheme : " << name << std::endl;
            lim = SUdotNabla_TVD::_limiter_QUICK;
        }
    }

    bool has_scheme(const std::string& name) const{
        auto it = _map.find(name);
        if (it != _map.end()) {
            return true;
        }else{
            return false;
        }
    }

    virtual ~SUdotNabla_TVD(){
    }

    // Improved total variation diminishing schemes for advection
    // simulation on arbitrary grids
    // J. Hou , F. Simons and R. Hinkelmann
    // Int. J. Numer. Meth. Fluids 2012; 70:359–382
    virtual Field cal(const VectorFace& U, const Field& phi, const Vt& t = 0.0){
        Field res = phi.new_compatible();
#ifdef OPENMP
        #pragma omp parallel for num_threads(phi.order().num_threads())
        for (St thread = 0; thread < phi.order().num_threads(); thread++) {
            for (auto iter = phi.order().begin(thread);
                    iter != phi.order().end(thread);
                    iter++) {
                auto& idx = (*iter);
#else
        for (auto& idx : phi.order()) {
#endif
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
                fp = VCp + lim(r, R) / R * (VDp - VCp);
                // m ---------------------
                R = _RCD(phi, d, Cm, Dm);
                r = _rCD(phi, d, Um, Cm, Dm, VUm, VCm, VDm);
                fm = VCm + lim(r, R) / R * (VDm - VCm);
                //
                //
                //
                arr[d] = (fp -  fm) * uc
                        / phi.grid().s_(d, idx);
            }

            Vt sum = 0;
            FOR_EACH_DIM{
                sum += arr[d];
            }
            res(idx) = sum;
        }
#ifdef OPENMP
     }
#endif
        return res;
    }

protected:
    LimiterMap _build_limiter_map() const{
        LimiterMap map;
        map["SOU"]      = SUdotNabla_TVD::_limiter_SOU;
        map["Fromm"]    = SUdotNabla_TVD::_limiter_Fromm;
        map["CUI"]      = SUdotNabla_TVD::_limiter_CUI;
        map["QUICK"]    = SUdotNabla_TVD::_limiter_QUICK;
        map["CDS"]      = SUdotNabla_TVD::_limiter_CDS;
        map["VanLeer"]  = SUdotNabla_TVD::_limiter_VanLeer;
        map["WAHYD"]    = SUdotNabla_TVD::_limiter_WAHYD;
        map["BSOU"]     = SUdotNabla_TVD::_limiter_BSOU;
        map["Superbee"] = SUdotNabla_TVD::_limiter_Superbee;
        map["Minmod"]   = SUdotNabla_TVD::_limiter_Minmod;
        map["MUSCL"]    = SUdotNabla_TVD::_limiter_MUSCL;
        map["Koren"]    = SUdotNabla_TVD::_limiter_Koren;
        map["WACEB"]    = SUdotNabla_TVD::_limiter_WACEB;
        map["UMIST"]    = SUdotNabla_TVD::_limiter_UMIST;
        map["SPLmax12"] = SUdotNabla_TVD::_limiter_SPLmax12;
        map["SPL13"]    = SUdotNabla_TVD::_limiter_SPL13;
        map["SPLmax13"] = SUdotNabla_TVD::_limiter_SPLmax13;
        map["Harmonic"] = SUdotNabla_TVD::_limiter_Harmonic;
        map["OSPER"]    = SUdotNabla_TVD::_limiter_OSPER;
        map["Albada"]   = SUdotNabla_TVD::_limiter_Albada;
        map["GVA12"]    = SUdotNabla_TVD::_limiter_GVA12;
        map["GVA13"]    = SUdotNabla_TVD::_limiter_GVA13;
        map["GPR0"]     = SUdotNabla_TVD::_limiter_GPR0;
        map["GPR12"]    = SUdotNabla_TVD::_limiter_GPR12;
        map["GPR13"]    = SUdotNabla_TVD::_limiter_GPR13;
        return map;
    }

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
    static Vt _limiter_VanLeer(Vt r, Vt R) {
        return 0.5 * R * (r + std::abs(r)) / (R - 1 + r);
    }
    static Vt _limiter_WAHYD(Vt r, Vt R) {
        if (r <= 1.0) {
            return 0.5 * R * (r + std::abs(r)) / (R - 1 + r);
        } else {
            return std::min((r + R * r * std::abs(r)) / (R + r * r), R);
        }
    }
    // Piece-wise Linear Steady-State Limiter
    static Vt _limiter_BSOU(Vt r, Vt R){
        return std::max(0.0, std::min(2.0 * r, 1.0));
    }
    static Vt _limiter_Superbee(Vt r, Vt R) {
        return std::max(std::max(0.0, std::min(R * r, 1.0)), std::min(r, R));
    }
    static Vt _limiter_Minmod(Vt r, Vt R){
        return std::max(0.0, std::min(r, 1.0));
    }
    static Vt _limiter_MUSCL(Vt r, Vt R){
        return std::max(0.0, std::min(std::min(2.0 * r, (r + 1.0) * 0.5), 2.0));
    }
    static Vt _limiter_Koren(Vt r, Vt R){
        return std::max(0.0, std::min(std::min(2.0 * r, (2.0 * r + 1.0) / 3.0), 2.0));
    }
    static Vt _limiter_WACEB(Vt r, Vt R){
        return std::max(0.0, std::min(std::min(2.0 * r, (3.0 * r + 1.0) / 4.0), 2.0));
    }
    static Vt _limiter_UMIST(Vt r, Vt R){
        Vt m1 = std::min(2.0 * r, (3.0 * r + 1.0) / 4.0);
        Vt m2 = std::min(m1, (r + 3.0) / 4.0);
        return std::max(0.0, std::min(m2, 2.0));
    }
    static Vt _limiter_SPLmax12(Vt r, Vt R){
        Vt m1 = std::max((3.0 * r + 1.0)/ 4.0, (r + 3.0) / 4.0);
        Vt m2 = std::min(m1, 2.0 * r);
        return std::max(0.0, std::min(m2, 2.0));
    }
    static Vt _limiter_SPL13(Vt r, Vt R){
        Vt m1 = std::min((2.0 * r + 1.0)/ 3.0, (r + 2.0) / 3.0);
        Vt m2 = std::min(m1, 2.0 * r);
        return std::max(0.0, std::min(m2, 2.0));
    }
    static Vt _limiter_SPLmax13(Vt r, Vt R){
        Vt m1 = std::min((2.0 * r + 1.0) / 3.0, (r + 2.0) / 3.0);
        Vt m2 = std::min(m1, 2.0 * r);
        return std::max(0.0, std::min(m2, 2.0));
    }
    // Smooth SS-TVD
    static Vt _limiter_Harmonic(Vt r, Vt R){
        return (r + std::abs(r)) / (r + 1.0);
    }
    // Symmetric polynomial-ratio (SPR) schemes
    static Vt _limiter_OSPER(Vt r, Vt R){
        return 3.0 * r * (r + 1.0) / 2.0 / (r * r + r + 1.0);
    }
    static Vt _limiter_Albada(Vt r, Vt R){
        return r * (r + 1.0) / (r * r + 1.0);
    }

    static Vt _limiter_GVA12(Vt r, Vt R){
        return r * (r + 2.0) / (r * r + 2.0);
    }
    static Vt _limiter_GVA13(Vt r, Vt R){
        return r * (r + 3.0) / (r * r + 3.0);
    }
    // Generalized polynomial-ratio (GPR) schemes
    static Vt _limiter_GPR0(Vt r, Vt R){
        return r * (3.0 * r + 1.0) / (2.0 * r * r + r + 1.0);
    }
    static Vt _limiter_GPR12(Vt r, Vt R){
        return 2.0 * r * ( r + 1.0) / (r * r + r + 2.0);
    }
    static Vt _limiter_GPR13(Vt r, Vt R){
        return r * ( 2.0 * r + 1.0) / (r * r + r + 1.0);
    }

};


}


#endif
