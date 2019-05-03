#ifndef __JSON_CPP_TEST_H__
#define __JSON_CPP_TEST_H__

#include <gtest/gtest.h>
#include "json/json.h"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

class JSONCPPTest : public ::testing::Test
{
protected:

	char jsonfile[256];

	void SetUp()
	{
		int n = sprintf(jsonfile, "%s/test/input_cases/utility_data/urdb.json", std::getenv("SSCDIR"));
	}
};

#endif 
