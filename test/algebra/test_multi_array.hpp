#ifndef ALGEBRA_TEST_MULTI_ARRAY_HPP_
#define ALGEBRA_TEST_MULTI_ARRAY_HPP_

#include "algebra/array/multi_array.hpp"
#include "gtest/gtest.h"
#include "utility/clock.hpp"

namespace carpio{

TEST(multiarray, multiarray){
	MultiArray_<double, 2> ma;
	ma.reconstruct(3,3);
}

TEST(array, openmp){
	ArrayListV_<double> a(1e7), b(1e7);
	a.assign(2);
	b.assign(1);
	// a.show();
	// b.show();
	tick_t start = Clock::Tick();
//	omp_set_num_threads(1);
//#pragma omp parallel
	b = a + b - a * b + 5.0 * a;
//	b.show();
	tick_t end = Clock::Tick();
 	std::cout << "Time = " << Clock::TimespanToMillisecondsD(start, end) << "ms" << std::endl;
	// a.show();
}

}




#endif
