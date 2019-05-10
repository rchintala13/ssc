#include <gtest/gtest.h>
#include "lib_utility_rate.h"
#include "lib_utility_rate_test.h"


TEST_F(libUtilityRateTests, TestJsonStringConstruct)
{
	UtilityRate ur(m_urdb);
}