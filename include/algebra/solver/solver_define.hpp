#ifndef _SOLVER_DEFINE_HPP_
#define _SOLVER_DEFINE_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/array/array_list.hpp"

#include <list>

namespace carpio{

template<class VALUE>
class Solver_ {
public:
	typedef VALUE Vt;
	typedef typename ArrayListV<Vt>::size_type St;
	typedef MatrixSCR_<Vt> MatSCR;
	typedef ArrayListV<Vt> Arr;
	typedef std::list<double> Listr;
protected:
	int _max_iter;   //max_iter
	double _tol;     // Tolerance

	int _num_iter;
	double _resid;
	Listr _lresid; //
public:

	Solver_(int max_iter = 100, Vt tol = 1e-3) {
		_max_iter = max_iter;
		_tol = tol;

		_num_iter = 0;
		_resid = 1e10;
	}

	virtual int solve(const MatSCR &A, // A  The matrix
			Arr &x,          // x
			const Arr& b    // b
			) {
		SHOULD_NOT_REACH;
		return 0;
	}
	;

	int max_iter() const {
		return this->_max_iter;
	}

	int tolerance() const {
		return this->_tol;
	}

	int num_iter() const {
		return this->_num_iter;
	}

	double residual() const {
		return this->_resid;
	}

	Arr get_residual_array() const{
		Arr res(this->_lresid.size());
		St i = 0;
		for(auto val : this->_lresid)
		{
			res[i] = val;
			i++;
		}
		return res;
	}

	virtual ~Solver_() {
	}
	;
protected:
	void _init() {
		this->_lresid;
	}

};



}

#endif
