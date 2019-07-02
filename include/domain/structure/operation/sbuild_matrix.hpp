#ifndef _S_BUILDMATRIX_HPP
#define _S_BUILDMATRIX_HPP


#include "domain/structure/structure_define.hpp"
#include "domain/structure/grid/sgrid.hpp"
#include "domain/structure/ghost/ghost.hpp"
#include "domain/structure/order/order.hpp"
#include "domain/structure/field/sfield.hpp"
#include "domain/structure/field/svector_center.hpp"
#include "domain/structure/field/svector_face.hpp"
#include "domain/structure/field/sexpression.hpp"
#include "domain/boundary/boundary_index.hpp"
#include "algebra/misc/interpolate.hpp"
#include "algebra/matrix/matrix_scr.hpp"

#include <array>

namespace carpio{

template<St DIM>
class SBuildMatrix_{
public:
	typedef SGrid_<DIM>   Grid;
	typedef SGhost_<DIM>  Ghost;
	typedef SOrder_<DIM>  Order;
	typedef SField_<DIM>  Field;
	typedef Field*       pField;
	typedef SIndex_<DIM>  Index;
	typedef std::shared_ptr<Field> spField;
	typedef MatrixSCR_<Vt>  Mat;
	typedef ArrayListV_<Vt> Arr;
	typedef ArrayListV_<St> ArrSt;

	typedef SExpField_<DIM> ExpField;
	typedef typename ExpField::Expression Exp;
	typedef typename Exp::Term            Term;
	typedef typename Exp::Coe             Coe;

	typedef SVectorCenter_<DIM> VectorCenter;
	typedef SVectorFace_<DIM>   VectorFace;
	typedef std::shared_ptr<BoundaryIndex> spBI;
	typedef BoundaryIndex* pBI;
	typedef BoundaryCondition BC;

	typedef AInterpolate_<Vt, Vt> Interpolate;
public:
	SBuildMatrix_(){}

	// The boundary condition has been applied
	static void Get(const ExpField&         expf,
				          Mat&              mat,
				          Arr&              b){
		auto& grid  = expf.grid();
		auto& order = expf.order();
		St    n     = order.size();

		typedef std::list<St> ListST;
		// Loop 1 ================
		//   1 get size of none zero
		//   2 build b
//		std::cout << "n = " << n << std::endl;
		b.reconstruct(n);
		St nz = 0;
		for(auto& index : order){
			auto& exp = expf(index);
			St   nr  = exp.size();
			nz      += nr;
			St   row = order.get_order(index);
			b[row]   = -(exp.value());  // negative is here !!!
		}
		// Loop 2 ================
		//   1 build matrix
		mat.newsize(n, n, nz);
		mat.row_ptr(0) = 0;
		for (auto& index : order) {
			auto& exp = expf(index);
			St    row = order.get_order(index);
			St    nr  = exp.size();
			mat.row_ptr(row + 1) = mat.row_ptr(row) + nr;
			// Inner Loop ---------------------
			St     ii = mat.row_ptr(row);
			auto iter = exp.begin();
			for(;ii < mat.row_ptr(row + 1) ;){
				mat.col_ind(ii) = order.get_order(iter->first);
				mat.val(ii)     = iter->second;
				ii++;
				iter++;
			}
		}
	}

	static void CopyToArray(const Field& f, Arr& arr){
		auto& order = f.order();
		St i = 0;
		for(auto& index : order){
			arr[i] = f(index);
			i++;
		}
	}

	static void CopyToField(const Arr& arr, Field& f){
			auto& order = f.order();
			St i = 0;
			for(auto& index : order){
				f(index) = arr[i];
				i++;
			}
		}
};




}

#endif
