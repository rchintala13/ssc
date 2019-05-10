#include "jsoncpp_test.h"
#include "json/json.h"
#include <gtest/gtest.h>
#include <fstream>

TEST_F(JSONCPPTest, TestReadFile)
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

TEST_F(JSONCPPTest, TestStringAssign)
{
	Json::CharReaderBuilder builder;
	Json::Value urdb;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(urdb_string, urdb);

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

TEST_F(JSONCPPTest, TestCharAssign)
{
	Json::CharReaderBuilder builder;
	Json::Value urdb;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(std::string(urdb_chars), urdb);

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