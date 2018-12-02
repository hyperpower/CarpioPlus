#ifndef _S_U_DOT_NABLA_PHI_HPP
#define _S_U_DOT_NABLA_PHI_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/sscalar.hpp"
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
	typedef SScalar_<DIM> Scalar;

	typedef SVectorCenter_<DIM> VectorCenter;
	typedef SVectorFace_<DIM>   VectorFace;
	typedef SExpression_<DIM>   Expression;

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

	virtual Scalar operator()(
			const VectorFace& U,
			const Scalar&     phi,
			const Vt&         t = 0.0){

	}
	virtual Expression operator()(
			const VectorFace& U,
			const Expression& phi,
			const Vt&         t = 0.0){

	}

};


template<St DIM>
class SUdotNabla_FOU: public SUdotNabla_<DIM>{
public:
	typedef SGrid_<DIM>   Grid;
	typedef SGhost_<DIM>  Ghost;
	typedef SOrder_<DIM>  Order;
	typedef SScalar_<DIM> Scalar;

	typedef SUdotNabla_<DIM> Base;

	typedef SVectorCenter_<DIM> VectorCenter;
	typedef SVectorFace_<DIM>   VectorFace;
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef SExpression_<DIM>   Expression;

	typedef SValue_<DIM> Value;

	SUdotNabla_FOU(): Base(){
	}

	SUdotNabla_FOU(spBI spbi) : Base(spbi){
	}

	virtual ~SUdotNabla_FOU(){
	}

	virtual Scalar operator()(const VectorFace& U, const Scalar& phi, const Vt& t = 0.0){
		Scalar res = phi.new_compatible();
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

	Expression operator()(
				const VectorFace& U,
				const Expression& phi,
				const Vt&         t = 0.0){

	}

};

template<St DIM>
class SUdotNabla_TVD: public SUdotNabla_<DIM>{
public:
	typedef SGrid_<DIM>   Grid;
	typedef SGhost_<DIM>  Ghost;
	typedef SOrder_<DIM>  Order;
	typedef SScalar_<DIM> Scalar;
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
	virtual Scalar operator()(const VectorFace& U, const Scalar& phi, const Vt& t = 0.0){
		Scalar res = phi.new_compatible();
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
//				fp = VCp + lim(r, R) / R * (VDp - VCp);
				// m ---------------------
				R = _RCD(phi, d, Cm, Dm);
				r = _rCD(phi, d, Um, Cm, Dm, VUm, VCm, VDm);
//				fm = VCm + lim(r, R) / R * (VDm - VCm);
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
				const Scalar& phi, St d,
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

	static Vt _RCD(const Scalar& phi, St d, const Index& C, const Index& D) {
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
