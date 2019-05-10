#ifndef __LIB_UTILITY_RATE_TEST_H__
#define __LIB_UTILITY_RATE_TEST_H__

#include <gtest/gtest.h>
#include <fstream>
#include <string>

#include "json/json.h"

class libUtilityRateTests : public ::testing::Test
{
protected:

	const char* m_urdb;

	void SetUp()
	{
		char jsonfile[256];
		int n = sprintf(jsonfile, "%s/test/input_cases/utility_data/urdb.json", std::getenv("SSCDIR"));
		std::ifstream json(jsonfile);
		std::string line, text; 
		while (std::getline(json, line)) {
			text += line;
		}
		m_urdb = text.c_str();
	}
};

#endif 
