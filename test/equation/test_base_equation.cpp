#include "gtest/gtest.h"
#include "domain/base/base_operator.hpp"

using namespace carpio;

TEST(base_equation, first){
    std::cout << "base equation" << std::endl;
    typedef BaseGrid_<3>  Grid;
    typedef BaseGhost_<3> Ghost;
    typedef BaseOrder_<3> Order;

    typedef BaseField_<3, Grid, Ghost, Order> Field;

    Field field;
    Laplacian(field, "BI", 0);
    
}