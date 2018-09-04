#ifndef _TEST_CLOCK_HPP_
#define _TEST_CLOCK_HPP_

#include "gtest/gtest.h"
#include "utility/clock.hpp"

#include <math.h>
#include <string>
#include <memory>

namespace carpio {

TEST(Clock, construct) {
	Clock c;
	c.start();
	c.break_point("a");
	c.break_point("b");
	c.show();

}

}

#endif
