#ifndef __JSON_CPP_TEST_H__
#define __JSON_CPP_TEST_H__

#include <fstream>
#include <string>

#include <gtest/gtest.h>
#include "json/json.h"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

class JSONCPPTest : public ::testing::Test
{
protected:

	// To test passing file name
	char jsonfile[256];

	// To test pass json deserialized to chars
	const char* urdb_chars;
	std::string urdb_string;

	void SetUp()
	{
		char jsonfile[256];
		int n = sprintf(jsonfile, "%s/test/input_cases/utility_data/urdb.json", std::getenv("SSCDIR"));
		std::ifstream json(jsonfile);
		std::string line;
		while (std::getline(json, line)) {
			urdb_string += line;
		}
		urdb_chars = urdb_string.c_str();
	}
};

#endif 
