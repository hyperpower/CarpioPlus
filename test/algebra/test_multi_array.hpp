#ifndef ALGEBRA_TEST_MULTI_ARRAY_HPP_
#define ALGEBRA_TEST_MULTI_ARRAY_HPP_

#include "algebra/array/multi_array.hpp"
#include "gtest/gtest.h"

namespace carpio{

TEST(multiarray, multiarray){
	MultiArray_<double, 2> ma;
	ma.reconstruct(3,3);
}

}




#endif
