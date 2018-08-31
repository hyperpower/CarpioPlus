#ifndef ALGEBRA_TEST_MATRIX_SPARSE_HPP_
#define ALGEBRA_TEST_MATRIX_SPARSE_HPP_

#include "algebra/matrix/matrix_sco.hpp"
#include "algebra/matrix/matrix_scr.hpp"
#include "algebra/io/mmio.hpp"
#include "algebra/solver/solver_define.hpp"
#include "algebra/solver/jacobi.hpp"
#include "gtest/gtest.h"

#include "algebra/io/gnuplot_actor_a.hpp"

namespace carpio{

TEST(matrxi_sparse, matrix){
	typedef MatrixSCO_<double> MatSCO;
	typedef ArrayListV<double> Arr;
	std::cout << "This test show how to read sparse matrix" << std::endl;
	MatSCO mat;
	mm_read_mtx_sparse("./test/input_files/mm/cavity01.mtx", mat);

	std::cout << "matrix size = " << mat.size_i() << " x " << mat.size_j() << std::endl;
	std::cout << "       size = " << mat.size() << std::endl;
	std::cout << " non-zeros  = " << mat.non_zeros() << std::endl;

	Arr arr;
	mm_read_array("./test/input_files/mm/cavity01_rhs1.mtx", arr);
	std::cout << "array size  = " << arr.size() << std::endl;
}

TEST(matrxi_sparse, matrix_write){
	typedef MatrixSCO_<double> MatSCO;
	typedef ArrayListV<double> Arr;
	std::cout << "This test show how to read sparse matrix" << std::endl;
	MatSCO mat;
	mm_read_mtx_sparse("./test/input_files/mm/cavity01.mtx", mat);

	std::cout << "matrix size = " << mat.size_i() << " x " << mat.size_j() << std::endl;
	std::cout << "       size = " << mat.size() << std::endl;
	std::cout << " non-zeros  = " << mat.non_zeros() << std::endl;

	Arr arr;
	mm_read_array("./test/input_files/mm/cavity01_rhs1.mtx", arr);
	std::cout << "array size  = " << arr.size() << std::endl;

	mm_write_mtx_sparse("./mat.mtx", mat);
}

TEST(matrxi_sparse, matrix_jacobi){
	typedef MatrixSCO_<double> MatSCO;
	typedef ArrayListV<double> Arr;
	std::cout << "This test show how to read sparse matrix" << std::endl;
	MatSCO mat;
	mm_read_mtx_sparse("./test/input_files/mm/cavity01.mtx", mat);

	std::cout << "matrix size = " << mat.size_i() << " x " << mat.size_j() << std::endl;
	std::cout << "       size = " << mat.size() << std::endl;
	std::cout << " non-zeros  = " << mat.non_zeros() << std::endl;

	Arr arr;
	mm_read_array("./test/input_files/mm/cavity01_rhs1.mtx", arr);
	std::cout << "array size  = " << arr.size() << std::endl;

	Arr x(arr.size());

	Jacobi_<double> solver;
	solver.solve(mat, x, arr);

}


}




#endif
