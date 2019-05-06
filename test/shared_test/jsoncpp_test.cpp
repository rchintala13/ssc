#include "jsoncpp_test.h"
#include "json/json.h"
#include <gtest/gtest.h>
#include <fstream>

TEST_F(JSONCPPTest, TestRead)
{
	std::ifstream json(jsonfile, std::ifstream::binary);
	Json::Value urdb;
	json >> urdb;

	const Json::Value flatdemandstructure = urdb["flatdemandstructure"];
	if (flatdemandstructure) {
		for (size_t p = 0; p < flatdemandstructure.size(); p++) {
			const Json::Value period = flatdemandstructure[(int)p];
			for (size_t t = 0; t < period.size(); t++) {
				const Json::Value tier = period[(int)t];
				const double rate = tier["rate"].asDouble();
				EXPECT_EQ(rate, 10.0);
			}
		}
	}

}