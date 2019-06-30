/************************
 //  \file   ICpre.h
 //  \brief
 //
 //  \author zhou
 //  \date   12 mai 2014
 ***********************/
#ifndef _SOVLER_PRECONDITIONER_HPP_
#define _SOVLER_PRECONDITIONER_HPP_

#include "algebra/algebra_define.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/misc/sort.hpp"
#include "algebra/array/array_list.hpp"

namespace carpio
{

template<class VALUE>
class Preconditioner_{
public:
	typedef VALUE             Vt;
	typedef MatrixV_<VALUE>   Matrix;
	typedef MatrixSCR_<VALUE> MatrixSCR;
	typedef ArrayListV_<VALUE> Array;
	typedef ArrayListV_<St>    ArraySt;
protected:

public:
	Preconditioner_() {}

	virtual ~Preconditioner_(){};

	virtual Array solve(const Array &x) const{
		SHOULD_NOT_REACH;
		return Array();
	}
	virtual Array trans_solve(const Array &x) const{
		SHOULD_NOT_REACH;
		return Array();
	}
	virtual MatrixSCR getPre(){
		SHOULD_NOT_REACH;
		return MatrixSCR();
	}

	virtual MatrixSCR getPreTrans(){
		SHOULD_NOT_REACH;
		return MatrixSCR();
	}
};


//template<class VALUE>
//class PreconditionerIC_ : public Preconditioner_<VALUE>{
//public:
//	typedef VALUE             Vt;
//	typedef Preconditioner_<VALUE> Base;
//	typedef MatrixV_<VALUE>   Matrix;
//	typedef MatrixSCR_<VALUE> MatrixSCR;
//	typedef ArrayListV_<VALUE> Array;
//	typedef ArrayListV_<St>    ArraySt;
//protected:
//	Array   val_;
//	ArraySt pntr_;
//	ArraySt indx_;
//	int nz_;
//	int dim_[2];
//public:
//	PreconditionerIC_(const Matrix &A) :
//			val_(0), pntr_(A.size_i() + 1), indx_(0), nz_(0){
//		dim_[0] = A.size_i();
//		dim_[1] = A.size_j();
//		//Count non-zero number in upper triangular part (including diagonal)
//		for (int i = 0; i < dim_[0]; i++) {
//			for (int j = i; j < dim_[1]; j++) {
//				if (A[i][j] != 0.0)
//					nz_++;
//			}
//		}
//		val_.reconstruct(nz_);
//		indx_.reconstruct(nz_);
//		pntr_[0] = 0;
//		for (int i = 0; i < dim_[0]; i++) {
//			pntr_[i + 1] = pntr_[i];
//			for (int j = i; j < dim_[1]; j++) {
//				if (A[i][j] != 0.0) {
//					int k = pntr_[i + 1]++;
//					val_[k] = A[i][j];
//					indx_[k] = j;
//				}
//			}
//		}
//		ICFactor(pntr_, indx_, val_);
//	}
//
//	PreconditionerIC_(const MatrixSCR &A) :
//			val_(0), pntr_(A.iLen() + 1), indx_(0), nz_(0) {
//		dim_[0] = A.size_i();
//		dim_[1] = A.size_j();
//
//		int i, j, k;
//		//Count non-zero number in upper triangular part (including diagonal)
//		for (k = 0; k < dim_[0]; k++) {
//			for (j = A.row_ptr(k); j < A.row_ptr(k + 1); j++) {
//				if (A.col_ind(j) >= k)
//					nz_++;
//			}
//		}
//
//		val_.reconstruct(nz_);
//		indx_.reconstruct(nz_);
//
//		// Copy just triangular part (including diagonal)
//		pntr_[0] = 0;
//		for (k = 0; k < dim_[0]; k++) {
//			pntr_[k + 1] = pntr_[k];
//			for (j = A.row_ptr(k); j < A.row_ptr(k + 1); j++) {
//				if (A.col_ind(j) >= k) {
//					i = pntr_[k + 1]++;
//					val_[i] = A.val(j);
//					indx_[i] = A.col_ind(j);
//				}
//			}
//		}
//
//		for (i = 0; i < dim_[0]; i++)
//			QSort_ascend(indx_, val_, pntr_[i], pntr_[i + 1] - pntr_[i]);
//		for (i = 0; i < dim_[0]; i++)
//			if (indx_[pntr_(i)] != i) {
//				std::cerr << "IC Preconditioner: diagonal not found!" << i
//						<< "\n";
//				exit(1);
//			}
//		ICFactor(pntr_, indx_, val_);
//	}
//
//
//	Array solve(const Array &x) const {
//		Array y(x);
//		for (int i = 0; i < dim_[1]; i++) {
//			Float z = y[i] / val_[pntr_[i]];
//			y[i] = z;
//			for (int j = pntr_[i] + 1; j < pntr_[i + 1]; j++) {
//				y[indx_[j]] -= z * val_[j];
//			}
//		}
//		for (int i = dim_[0] - 1; i >= 0; i--) {
//			Float z = 0;
//			for (int j = pntr_[i] + 1; j < pntr_[i + 1]; j++) {
//				z += y[indx_[j]] * val_[j];
//			}
//			y[i] = (y[i] - z) / val_[pntr_[i]];
//		}
//		return y;
//	}
//
//	Array trans_solve(const Array &x) const {
//		return this->solve(x);
//	}
//
//	MatrixSCR get_preconditioner_mat() {
//		MatrixSCR res(dim_[0], dim_[1], nz_, val_, pntr_, indx_);
//		return res;
//	}
//
//	MatrixSCR get_preconditioner_mat_trans() {
//		MatrixSCR res(dim_[0], dim_[1], nz_, val_, pntr_, indx_);
//		res.trans();
//		return res;
//	}
//
//	~PreconditionerIC_(){};
//};







}

#endif
