
#include <iostream>
#include "test.hpp"

namespace carpio{

int RunTests(int argc, char **argv) {
	//::testing::GTEST_FLAG(filter) = "Poisson.adpgrid_source";
	//::testing::GTEST_FLAG(filter) = "Grid2D.no_leaf";
	::testing::InitGoogleTest(&argc, argv);
	int res = RUN_ALL_TESTS();
	return res;
}

}

int main(int argc, char **argv) {
	int res = carpio::RunTests(argc, argv);
    std::cout<<" ====  end ====";
    return res;
}