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

	virtual Scalar operator()(const VectorFace& U, const Scalar& phi){

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

	typedef SValue_<DIM> Value;

	SUdotNabla_FOU(): Base(){
	}

	SUdotNabla_FOU(spBI spbi) : Base(spbi){
	}

	virtual ~SUdotNabla_FOU(){
	}

	virtual Scalar operator()(const VectorFace& U, const Scalar& phi, const Vt& t = 0.0){
		std::cout << "FOU\n";
		Scalar res = phi.new_compatible();
		for (auto& idx : phi.order()) {
			std::array<Vt, DIM> arr;
			arr.fill(0.0);

			FOR_EACH_DIM
			{
				Vt up = U(Axes(d), _P_, idx);
				Vt um = U(Axes(d), _M_, idx);
				Vt uc = (up + um) * 0.5;      //average velocity to center
				auto idxm = idx.m(d);
				auto idxp = idx.p(d);
				Vt phi_u, phi_d;
				Vt s      = phi.grid().s_(d, idx);
				if (uc >= 0) {
					phi_u = Value::Get(
							phi, *(this->_spbi),
							idx, idxm, Axes(d),_M_, t);
					phi_d = phi(idx);

				} else { // uc < 0
					phi_u = phi(idx);
					phi_d = Value::Get(
							phi, *(this->_spbi),
							idx, idxp, Axes(d), _P_, t);
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

};


}


#endif
