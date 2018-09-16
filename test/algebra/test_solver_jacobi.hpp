#ifndef ALGEBRA_TEST_SOLVER_JACOBI_HPP_
#define ALGEBRA_TEST_SOLVER_JACOBI_HPP_

#include "algebra/solver/jacobi.hpp"
#include "algebra/solver/direct.hpp"
#include "gtest/gtest.h"

namespace carpio{

TEST(solver, direct1) {
	typedef MatrixV_<Vt> Mat;
	typedef ArrayListV_<Vt> Arr;
	typedef Jacobi_<Vt> Solver;
	double rawA[4 * 4] = {
			10., -1., 2., 0.,
			-1., 11., -1., 3.,
			2., -1., 10., -1.,
			0.0, 3., -1., 8. };
	Mat A(4, 4, 16, rawA);
	Arr b { 6., 25., -11., 15. };

	Solve(A, b);
	b.show();
}


TEST(solver, jacobi_dense1){
	typedef MatrixV_<Vt>    Mat;
	typedef ArrayListV_<Vt> Arr;
	typedef Jacobi_<Vt> Solver;
	double  rawA[4*4] = {10., -1.,  2., 0.,
	                    -1., 11., -1., 3.,
	                     2., -1., 10., -1.,
	                     0.0, 3., -1., 8.};
	Mat A( 4, 4, 16, rawA);
	Arr b{ 6., 25., -11., 15.};
	Arr x( 4);
	Arr expected_x{ 1.0, 2.0, -1.0, 1.0 };
	int n = 4;

	Solver solver(1000, 1e-6);
	solver.solve(A, x, b);
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;

	// checking if all values are correct (up to some error value)
	for (int i = 0; i < n; ++i) {
		EXPECT_NEAR(expected_x[i], x[i], 1e-3) << " element y[" << i
				<< "] is wrong";
	}

}

TEST(solver, jacobi_scr1){
	typedef MatrixV_<Vt>    Mat;
	typedef MatrixSCR_<Vt>  MatSCR;
	typedef ArrayListV_<Vt> Arr;
	typedef Jacobi_<Vt> Solver;
	double  rawA[4*4] = {10., -1.,  2., 0.,
	                    -1., 11., -1., 3.,
	                     2., -1., 10., -1.,
	                     0.0, 3., -1., 8.};
	Mat A( 4, 4, 16, rawA);
	Arr b{ 6., 25., -11., 15.};
	Arr x( 4);
	Arr expected_x{ 1.0, 2.0, -1.0, 1.0 };
	MatSCR Ascr(A);
	int n = 4;

	Solver solver(1000, 1e-6);
	solver.solve(Ascr, x, b);
	std::cout << "iter end = " << solver.num_iter() << std::endl;
	std::cout << "resi end = " << solver.residual() << std::endl;

	// checking if all values are correct (up to some error value)
	for (int i = 0; i < n; ++i) {
		EXPECT_NEAR(expected_x[i], x[i], 1e-3) << " element y[" << i
				<< "] is wrong";
	}
}

}




#endif
