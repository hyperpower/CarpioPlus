#ifndef _ALGEBRA_TEST_LINEAR_POLYNOMIAL_HPP_
#define _ALGEBRA_TEST_LINEAR_POLYNOMIAL_HPP_

#include "gtest/gtest.h"
#include "algebra/misc/linear_polynomial.hpp"

namespace carpio{

TEST(linear_polynomial, lp){
	typedef LinearPolynomial_<double, std::string> Poly;
	Poly poly;
	poly["a"] = 1;
	poly["b"] = 2;
	poly["b"] +=3;

	Poly pb;
	pb = poly;

	pb += 3;
	std::cout << "pb += 3\n";
	std::cout << pb << std::endl;

	pb -= 5;
	std::cout << "pb -= 5\n";
	std::cout << pb << std::endl;

	pb *= 2;
	std::cout << "pb *= 2\n";
	std::cout << pb << std::endl;

	pb /= 3;
	std::cout << "pb /= 3\n";
	std::cout << pb << std::endl;

}

}


#endif
