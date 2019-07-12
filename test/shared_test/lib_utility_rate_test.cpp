#include <gtest/gtest.h>
#include "lib_utility_rate.h"
#include "lib_utility_rate_test.h"


TEST_F(libUtilityRateTests, TestJsonStringConstruct)
{
	UtilityRate ur(m_urdb);

	// https://openei.org/apps/IURDB/rate/view/5ae3aef6682bea1e47abd1c5#3__Energy
	util::matrix_t<double> energyRatesMatrix = ur.getEnergyRatesMatrix();
	// Column 0: Period
	ASSERT_EQ(energyRatesMatrix.at(0, 0), 1);
	ASSERT_EQ(energyRatesMatrix.at(1, 0), 1);
	ASSERT_EQ(energyRatesMatrix.at(2, 0), 1);
	ASSERT_EQ(energyRatesMatrix.at(8, 0), 2);
	// Column 1: Tier
	ASSERT_EQ(energyRatesMatrix.at(0, 1), 1);
	ASSERT_EQ(energyRatesMatrix.at(1, 1), 2);
	ASSERT_EQ(energyRatesMatrix.at(2, 1), 3);
	ASSERT_EQ(energyRatesMatrix.at(8, 1), 1);
	// Column 2: Max usage
	ASSERT_EQ(energyRatesMatrix.at(0, 2), 200);
	ASSERT_EQ(energyRatesMatrix.at(1, 2), 3000);
	ASSERT_EQ(energyRatesMatrix.at(2, 2), 10000);
	ASSERT_EQ(energyRatesMatrix.at(4, 2), 1e38);
	// Column 3: Max usage unit
	ASSERT_EQ(energyRatesMatrix.at(0, 3), 1);
	ASSERT_EQ(energyRatesMatrix.at(1, 3), 0);
	ASSERT_EQ(energyRatesMatrix.at(2, 3), 0);
	ASSERT_EQ(energyRatesMatrix.at(8, 3), 1);
	// Column 4: Rate
	ASSERT_NEAR(energyRatesMatrix.at(0, 4), 0.031718, 0.001);
	ASSERT_NEAR(energyRatesMatrix.at(1, 4), 0.194683, 0.001);
	ASSERT_NEAR(energyRatesMatrix.at(2, 4), 0.179509, 0.001);
	ASSERT_NEAR(energyRatesMatrix.at(3, 4), 0.157777, 0.001);

	util::matrix_t<size_t> energyWeekdaySchedule = ur.getEnergyWeekdaySchedule();
	size_t nr, nc;
	energyWeekdaySchedule.size(nr, nc);
	ASSERT_EQ(nr, 12);
	ASSERT_EQ(nc, 24);
	ASSERT_EQ(energyWeekdaySchedule.at(0, 0), 1);
	ASSERT_EQ(energyWeekdaySchedule.at(5, 0), 0);

	util::matrix_t<size_t> energyWeekendSchedule = ur.getEnergyWeekendSchedule();
	energyWeekendSchedule.size(nr, nc);
	ASSERT_EQ(nr, 12);
	ASSERT_EQ(nc, 24);
	ASSERT_EQ(energyWeekendSchedule.at(0, 0), 1);
	ASSERT_EQ(energyWeekendSchedule.at(5, 0), 0);

}