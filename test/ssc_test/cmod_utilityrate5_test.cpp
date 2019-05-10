#include "cmod_utilityrate5_test.h"

/*
Test rate 1 with NEM option 1 across 6 combinations
*/
TEST_F(CMUtilityRate5, TestRate1)
{
	rate1(data);
	const char * SSCDIR = std::getenv("SSCDIR");
	char gen[500], load[500];
	double ec_wo_sys_array[6] = {2326, 111909, 1008785, 2326, 111909, 1008785};
	double ec_w_sys_array[6] = {1312, 109862, 1006718, -8094, 80241, 967965};
	double dcf_wo_sys_array[6] = { 0, 0, 0, 0, 0, 0 };
	double dcf_w_sys_array[6] = { 0, 0, 0, 0, 0, 0 };
	double dct_wo_sys_array[6] = { 1009, 57117, 464666, 1009, 57117, 446666 };
	double dct_w_sys_array[6] = { 967, 56199, 463729, 963, 48038, 446436 };


	size_t i = 0;
	for (int g = 1; g <= 1; g++) 
	{
		for (int l = 1; l <= 3; l++)
		{
			int n1 = sprintf(gen, "%s/test/input_cases/utility_data/gen%d.txt", SSCDIR, g);
			int n2 = sprintf(load, "%s/test/input_cases/utility_data/load%d.txt", SSCDIR, l);
			set_array(data, "gen", gen, 8760);
			set_array(data, "load", load, 8760);

			EXPECT_FALSE(run_module(data, "utilityrate5"));

			double elec_cost_with_sys, elec_cost_wo_sys;
			ssc_number_t * dcf_with_sys;
			ssc_number_t * dcf_wo_sys;
			ssc_number_t * dct_with_sys;
			ssc_number_t * dct_wo_sys;
			ssc_data_get_number(data, "elec_cost_with_system_year1", &elec_cost_with_sys);
			ssc_data_get_number(data, "elec_cost_without_system_year1", &elec_cost_wo_sys);
			dct_with_sys = ssc_data_get_array(data, "charge_w_sys_dc_tou", &n1);
			dct_wo_sys = ssc_data_get_array(data, "charge_wo_sys_dc_tou", &n1);
			dcf_with_sys = ssc_data_get_array(data, "charge_w_sys_dc_fixed", &n1);
			dcf_wo_sys = ssc_data_get_array(data, "charge_wo_sys_dc_fixed", &n1);

			EXPECT_NEAR(elec_cost_with_sys, ec_w_sys_array[i], m_error_tolerance_hi);
			EXPECT_NEAR(elec_cost_wo_sys, ec_wo_sys_array[i], m_error_tolerance_hi);
			EXPECT_NEAR(dct_with_sys[1], dct_w_sys_array[i], m_error_tolerance_hi);
			EXPECT_NEAR(dct_wo_sys[1], dct_wo_sys_array[i], m_error_tolerance_hi);
			EXPECT_NEAR(dcf_with_sys[1], dcf_w_sys_array[i], m_error_tolerance_hi);
			EXPECT_NEAR(dcf_wo_sys[1], dcf_wo_sys_array[i], m_error_tolerance_hi);


			i++;
		}
	}
}

/*
Test rate 2 with NEM option 1 across 6 combinations
*/