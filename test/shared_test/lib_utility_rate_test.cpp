#include <gtest/gtest.h>
#include "lib_utility_rate.h"
#include "lib_utility_rate_test.h"


TEST_F(libUtilityRateTests, TestJsonStringConstruct)
{
	UtilityRate ur(m_urdb);

	// https://openei.org/apps/IURDB/rate/view/5ae3aef6682bea1e47abd1c5#3__Energy
	util::matrix_t<double> energyRatesMatrix = ur.getEnergyRatesMatrix();
	ASSERT_EQ(energyRatesMatrix.at(0, 0), 1);
	ASSERT_EQ(energyRatesMatrix.at(1, 0), 1);
	ASSERT_EQ(energyRatesMatrix.at(2, 0), 1);
	ASSERT_EQ(energyRatesMatrix.at(8, 0), 2);
	ASSERT_EQ(energyRatesMatrix.at(0, 1), 1);
	ASSERT_EQ(energyRatesMatrix.at(1, 1), 2);
	ASSERT_EQ(energyRatesMatrix.at(2, 1), 3);
	ASSERT_EQ(energyRatesMatrix.at(8, 1), 1);
	ASSERT_EQ(energyRatesMatrix.at(0, 2), 200);
	ASSERT_EQ(energyRatesMatrix.at(1, 2), 3000);
	ASSERT_EQ(energyRatesMatrix.at(2, 2), 10000);
	ASSERT_EQ(energyRatesMatrix.at(4, 2), 1e38);
	ASSERT_EQ(energyRatesMatrix.at(0, 3), 1);
	ASSERT_EQ(energyRatesMatrix.at(1, 3), 0);
	ASSERT_EQ(energyRatesMatrix.at(2, 3), 0);
	ASSERT_EQ(energyRatesMatrix.at(8, 3), 1);
	ASSERT_NEAR(energyRatesMatrix.at(0, 4), 0.031718, 0.001);
	ASSERT_NEAR(energyRatesMatrix.at(1, 4), 0.194683, 0.001);
	ASSERT_NEAR(energyRatesMatrix.at(2, 4), 0.179509, 0.001);
	ASSERT_NEAR(energyRatesMatrix.at(3, 4), 0.157777, 0.001);

	


}