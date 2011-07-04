#include "core.h"


static var_info vtab_utility_rate[] = {

/*   VARTYPE           DATATYPE         NAME                         LABEL                                           UNITS     META                      GROUP          REQUIRED_IF                 CONSTRAINTS                      UI_HINTS*/
	{ SSC_INPUT,        SSC_NUMBER,     "analysis_years",           "Number of years in analysis",                   "years",  "",                      "",             "*",                         "INTEGER,POSITIVE",              "" },
	{ SSC_INPUT,        SSC_ARRAY,      "e_with_system",            "Net energy at grid with system",                "kWh",    "",                      "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_INPUT,        SSC_ARRAY,      "p_with_system",            "Max power at grid with system",                 "kW",     "",                      "",             "?",                         "LENGTH=8760",                   "" },
	{ SSC_INPUT,        SSC_ARRAY,      "e_without_sys",            "Net energy at grid without system (load only)", "kWh",    "",                      "",             "?",                         "LENGTH=8760",                   "" },
	{ SSC_INPUT,        SSC_ARRAY,      "p_without_system",         "Max power at grid without system (load only)",  "kW",     "",                      "",             "?",                         "LENGTH=8760",                   "" },

	{ SSC_INPUT,        SSC_ARRAY,      "system_degradation",       "Annual degradation of system",    "%/year", "",                      "",             "?=0",                       "",                              "" },
	{ SSC_INPUT,        SSC_ARRAY,      "load_escalation",          "Annual load escalation",          "%/year", "",                      "",             "?=0",                       "",                              "" },
	{ SSC_INPUT,        SSC_ARRAY,      "rate_escalation",          "Annual utility rate escalation",  "%/year", "",                      "",             "?=0",                       "",                              "" },
	
	{ SSC_INPUT,        SSC_NUMBER,     "ur_sell_eq_buy",           "Force sell rate equal to buy",    "0/1",    "Enforce net metering",  "",             "?=1",                       "BOOLEAN",                       "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_monthly_fixed_charge",  "Monthly fixed charge",            "$",      "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_flat_buy_rate",         "Flat rate (buy)",                 "$/kWh",  "",                      "",             "*",                         "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_flat_sell_rate",        "Flat rate (sell)",                "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_flat_fuel_rate",        "Flat rate (fuel adj)",            "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_enable",            "Enable time-of-use rates",        "0/1",    "",                      "",             "?=0",                       "BOOLEAN",                       "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p1_buy_rate",       "TOU Period 1 Rate (buy)",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p1_sell_rate",      "TOU Period 1 Rate (sell)",        "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p1_fuel_rate",      "TOU Period 1 Rate (fuel adj)",    "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p2_buy_rate",       "TOU Period 2 Rate (buy)",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p2_sell_rate",      "TOU Period 2 Rate (sell)",        "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p2_fuel_rate",      "TOU Period 2 Rate (fuel adj)",    "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p3_buy_rate",       "TOU Period 3 Rate (buy)",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p3_sell_rate",      "TOU Period 3 Rate (sell)",        "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p3_fuel_rate",      "TOU Period 3 Rate (fuel adj)",    "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p4_buy_rate",       "TOU Period 4 Rate (buy)",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p4_sell_rate",      "TOU Period 4 Rate (sell)",        "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p4_fuel_rate",      "TOU Period 4 Rate (fuel adj)",    "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p5_buy_rate",       "TOU Period 5 Rate (buy)",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p5_sell_rate",      "TOU Period 5 Rate (sell)",        "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p5_fuel_rate",      "TOU Period 5 Rate (fuel adj)",    "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p6_buy_rate",       "TOU Period 6 Rate (buy)",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p6_sell_rate",      "TOU Period 6 Rate (sell)",        "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p6_fuel_rate",      "TOU Period 6 Rate (fuel adj)",    "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p7_buy_rate",       "TOU Period 7 Rate (buy)",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p7_sell_rate",      "TOU Period 7 Rate (sell)",        "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p7_fuel_rate",      "TOU Period 7 Rate (fuel adj)",    "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p8_buy_rate",       "TOU Period 8 Rate (buy)",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p8_sell_rate",      "TOU Period 8 Rate (sell)",        "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p8_fuel_rate",      "TOU Period 8 Rate (fuel adj)",    "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p9_buy_rate",       "TOU Period 9 Rate (buy)",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p9_sell_rate",      "TOU Period 9 Rate (sell)",        "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tou_p9_fuel_rate",      "TOU Period 9 Rate (fuel adj)",    "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_STRING,     "ur_tou_sched_weekday",     "TOU Weekday Schedule",            "",       "288 digits 0-9, 24x12", "",             "ur_tou_enable=1",           "TOUSCHED",                      "" },
	{ SSC_INPUT,        SSC_STRING,     "ur_tou_sched_weekend",     "TOU Weekend Schedule",            "",       "288 digits 0-9, 24x12", "",             "ur_tou_enable=1",           "TOUSCHED",                      "" },

	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_enable",             "Enable demand charges",           "0/1",    "",                      "",             "?=0",                       "BOOLEAN",                       "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m1",           "DC Fixed Rate January",           "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m2",           "DC Fixed Rate February",          "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m3",           "DC Fixed Rate March",             "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m4",           "DC Fixed Rate April",             "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m5",           "DC Fixed Rate May",               "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m6",           "DC Fixed Rate June",              "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m7",           "DC Fixed Rate July",              "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m8",           "DC Fixed Rate August",            "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m9",           "DC Fixed Rate September",         "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m10",          "DC Fixed Rate October",           "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m11",          "DC Fixed Rate November",          "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_fixed_m12",          "DC Fixed Rate December",          "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_p1",                 "DC TOU Rate Period 1",            "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_p2",                 "DC TOU Rate Period 2",            "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_p3",                 "DC TOU Rate Period 3",            "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_p4",                 "DC TOU Rate Period 4",            "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_p5",                 "DC TOU Rate Period 5",            "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_p6",                 "DC TOU Rate Period 6",            "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_p7",                 "DC TOU Rate Period 7",            "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_p8",                 "DC TOU Rate Period 8",            "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_dc_p9",                 "DC TOU Rate Period 9",            "$/kW,pk","",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_STRING,     "ur_dc_sched_weekday",      "DC TOU Weekday Schedule",         "",       "288 digits 0-9, 24x12", "",             "ur_dc_enable=1",            "TOUSCHED",                      "" },
	{ SSC_INPUT,        SSC_STRING,     "ur_dc_sched_weekend",      "DC TOU Weekend Schedule",         "",       "288 digits 0-9, 24x12", "",             "ur_dc_enable=1",            "TOUSCHED",                      "" },

	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_enable",             "Enable tiered rates",             "0/1",    "",                      "",             "?=0",                       "BOOLEAN",                       "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sell_mode",          "Tiered rate sell mode",           "0,1,2",  "0=specified,1=tier1,2=lowest", "",      "?=1",                       "INTEGER,MIN=0,MAX=2",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sell_rate",          "Specified tiered sell rate",      "$/kW",   "",                      "",             "ur_tr_sell_mode=0",         "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_energy_ub1",      "Tiered Struct. 1 Energy UB 1",    "kWh",    "",                      "",             "?=1e99",                    "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_energy_ub2",      "Tiered Struct. 1 Energy UB 2",    "kWh",    "",                      "",             "?=1e99",                    "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_energy_ub3",      "Tiered Struct. 1 Energy UB 3",    "kWh",    "",                      "",             "?=1e99",                    "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_energy_ub4",      "Tiered Struct. 1 Energy UB 4",    "kWh",    "",                      "",             "?=1e99",                    "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_energy_ub5",      "Tiered Struct. 1 Energy UB 5",    "kWh",    "",                      "",             "?=1e99",                    "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_energy_ub6",      "Tiered Struct. 1 Energy UB 6",    "kWh",    "",                      "",             "?=1e99",                    "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_rate1",           "Tiered Struct. 1 Rate 1",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_rate2",           "Tiered Struct. 1 Rate 2",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_rate3",           "Tiered Struct. 1 Rate 3",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_rate4",           "Tiered Struct. 1 Rate 4",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_rate5",           "Tiered Struct. 1 Rate 5",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s1_rate6",           "Tiered Struct. 1 Rate 6",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_energy_ub1",      "Tiered Struct. 2 Energy UB 1",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_energy_ub2",      "Tiered Struct. 2 Energy UB 2",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_energy_ub3",      "Tiered Struct. 2 Energy UB 3",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_energy_ub4",      "Tiered Struct. 2 Energy UB 4",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_energy_ub5",      "Tiered Struct. 2 Energy UB 5",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_energy_ub6",      "Tiered Struct. 2 Energy UB 6",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_rate1",           "Tiered Struct. 2 Rate 1",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_rate2",           "Tiered Struct. 2 Rate 2",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_rate3",           "Tiered Struct. 2 Rate 3",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_rate4",           "Tiered Struct. 2 Rate 4",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_rate5",           "Tiered Struct. 2 Rate 5",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s2_rate6",           "Tiered Struct. 2 Rate 6",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_energy_ub1",      "Tiered Struct. 3 Energy UB 1",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_energy_ub2",      "Tiered Struct. 3 Energy UB 2",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_energy_ub3",      "Tiered Struct. 3 Energy UB 3",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_energy_ub4",      "Tiered Struct. 3 Energy UB 4",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_energy_ub5",      "Tiered Struct. 3 Energy UB 5",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_energy_ub6",      "Tiered Struct. 3 Energy UB 6",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_rate1",           "Tiered Struct. 3 Rate 1",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_rate2",           "Tiered Struct. 3 Rate 2",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_rate3",           "Tiered Struct. 3 Rate 3",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_rate4",           "Tiered Struct. 3 Rate 4",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_rate5",           "Tiered Struct. 3 Rate 5",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s3_rate6",           "Tiered Struct. 3 Rate 6",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_energy_ub1",      "Tiered Struct. 4 Energy UB 1",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_energy_ub2",      "Tiered Struct. 4 Energy UB 2",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_energy_ub3",      "Tiered Struct. 4 Energy UB 3",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_energy_ub4",      "Tiered Struct. 4 Energy UB 4",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_energy_ub5",      "Tiered Struct. 4 Energy UB 5",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_energy_ub6",      "Tiered Struct. 4 Energy UB 6",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_rate1",           "Tiered Struct. 4 Rate 1",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_rate2",           "Tiered Struct. 4 Rate 2",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_rate3",           "Tiered Struct. 4 Rate 3",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_rate4",           "Tiered Struct. 4 Rate 4",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_rate5",           "Tiered Struct. 4 Rate 5",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s4_rate6",           "Tiered Struct. 4 Rate 6",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_energy_ub1",      "Tiered Struct. 5 Energy UB 1",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_energy_ub2",      "Tiered Struct. 5 Energy UB 2",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_energy_ub3",      "Tiered Struct. 5 Energy UB 3",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_energy_ub4",      "Tiered Struct. 5 Energy UB 4",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_energy_ub5",      "Tiered Struct. 5 Energy UB 5",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_energy_ub6",      "Tiered Struct. 5 Energy UB 6",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_rate1",           "Tiered Struct. 5 Rate 1",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_rate2",           "Tiered Struct. 5 Rate 2",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_rate3",           "Tiered Struct. 5 Rate 3",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_rate4",           "Tiered Struct. 5 Rate 4",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_rate5",           "Tiered Struct. 5 Rate 5",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s5_rate6",           "Tiered Struct. 5 Rate 6",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_energy_ub1",      "Tiered Struct. 6 Energy UB 1",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_energy_ub2",      "Tiered Struct. 6 Energy UB 2",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_energy_ub3",      "Tiered Struct. 6 Energy UB 3",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_energy_ub4",      "Tiered Struct. 6 Energy UB 4",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_energy_ub5",      "Tiered Struct. 6 Energy UB 5",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_energy_ub6",      "Tiered Struct. 6 Energy UB 6",    "kWh",    "",                      "",             "?=1e99",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_rate1",           "Tiered Struct. 6 Rate 1",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_rate2",           "Tiered Struct. 6 Rate 2",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_rate3",           "Tiered Struct. 6 Rate 3",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_rate4",           "Tiered Struct. 6 Rate 4",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_rate5",           "Tiered Struct. 6 Rate 5",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_s6_rate6",           "Tiered Struct. 6 Rate 6",         "$/kWh",  "",                      "",             "?=0.0",                     "",                              "" },
/*   VARTYPE           DATATYPE         NAME                         LABEL                              UNITS     META                      GROUP          REQUIRED_IF                 CONSTRAINTS                      UI_HINTS*/

	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m1",           "Tiered Structure for January",    "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m2",           "Tiered Structure for February",   "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m3",           "Tiered Structure for March",      "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m4",           "Tiered Structure for April",      "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m5",           "Tiered Structure for May",        "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m6",           "Tiered Structure for June",       "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m7",           "Tiered Structure for July",       "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m8",           "Tiered Structure for August",     "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m9",           "Tiered Structure for September",  "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m10",          "Tiered Structure for October",    "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m11",          "Tiered Structure for November",   "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ur_tr_sched_m12",          "Tiered Structure for December",   "0-5",    "tiered structure #",    "",             "?=0",                       "INTEGER,MIN=0,MAX=5",           "" },
	
	{ SSC_OUTPUT,       SSC_ARRAY,      "energy_value",             "Net energy value in each year",     "$",    "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "energy_net",               "Net energy in each year",           "kW",   "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "revenue_with_system",      "Total revenue with system",         "$",    "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "revenue_without_system",   "Total revenue without system",      "$",    "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_e_grid",         "Year 1 hourly grid energy",       "kWh", "",                      "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_p_grid",         "Year 1 hourly grid peak power",   "kW",  "",                      "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_system_output",  "Year 1 hourly system output",     "kWh", "",                      "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_e_demand",       "Year 1 hourly energy demand",     "kWh", "",                      "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_p_demand",       "Year 1 hourly peak power demand", "kW",  "",                      "",             "*",                         "LENGTH=8760",                   "" },
	

	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_revenue_with_system",     "Year 1 Hourly revenue with system",    "$", "",          "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_payment_with_system",     "Year 1 Hourly payment with system",    "$", "",          "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_income_with_system",      "Year 1 Hourly income with system",     "$", "",          "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_price_with_system",       "Year 1 Hourly price with system",      "$", "",          "",             "*",                         "LENGTH=8760",                   "" },
	
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_revenue_without_system",  "Year 1 Hourly revenue without system", "$", "",          "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_payment_without_system",  "Year 1 Hourly payment without system", "$", "",          "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_income_without_system",   "Year 1 Hourly income without system",  "$", "",          "",             "*",                         "LENGTH=8760",                   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_hourly_price_without_system",    "Year 1 Hourly price without system",   "$", "",          "",             "*",                         "LENGTH=8760",                   "" },
	
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_monthly_dc_fixed_with_system",      "Year 1 monthly demand charge (Fixed) with system",    "$", "", "",          "*",                         "LENGTH=12",                     "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_monthly_dc_tou_with_system",        "Year 1 monthly demand charge (TOU) with system",      "$", "", "",          "*",                         "LENGTH=12",                     "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_monthly_tr_charge_with_system",     "Year 1 monthly tiered charge with system",            "$", "", "",          "*",                         "LENGTH=12",                     "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_monthly_tr_rate_with_system",       "Year 1 monthly tiered rate with system",              "$", "", "",          "*",                         "LENGTH=12",                     "" },
	
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_monthly_dc_fixed_without_system",   "Year 1 monthly demand charge (Fixed) without system", "$", "", "",          "*",                         "LENGTH=12",                     "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_monthly_dc_tou_without_system",     "Year 1 monthly demand charge (TOU) without system",   "$", "", "",          "*",                         "LENGTH=12",                     "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_monthly_tr_charge_without_system",  "Year 1 monthly tiered charge without system",         "$", "", "",          "*",                         "LENGTH=12",                     "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "year1_monthly_tr_rate_without_system",    "Year 1 monthly tiered rate without system",           "$", "", "",          "*",                         "LENGTH=12",                     "" },
	
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_jan",      "Demand Charge (Fixed) in Jan",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_feb",      "Demand Charge (Fixed) in Feb",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_mar",      "Demand Charge (Fixed) in Mar",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_apr",      "Demand Charge (Fixed) in Apr",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_may",      "Demand Charge (Fixed) in May",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_jun",      "Demand Charge (Fixed) in Jun",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_jul",      "Demand Charge (Fixed) in Jul",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_aug",      "Demand Charge (Fixed) in Aug",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_sep",      "Demand Charge (Fixed) in Sep",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_oct",      "Demand Charge (Fixed) in Oct",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_nov",      "Demand Charge (Fixed) in Nov",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_fixed_dec",      "Demand Charge (Fixed) in Dec",    "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },

		
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_jan",        "Demand Charge (TOU) in Jan",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_feb",        "Demand Charge (TOU) in Feb",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_mar",        "Demand Charge (TOU) in Mar",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_apr",        "Demand Charge (TOU) in Apr",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_may",        "Demand Charge (TOU) in May",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_jun",        "Demand Charge (TOU) in Jun",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_jul",        "Demand Charge (TOU) in Jul",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_aug",        "Demand Charge (TOU) in Aug",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_sep",        "Demand Charge (TOU) in Sep",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_oct",        "Demand Charge (TOU) in Oct",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_nov",        "Demand Charge (TOU) in Nov",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_dc_tou_dec",        "Demand Charge (TOU) in Dec",      "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },

	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_jan",            "Tiered Rate Charge in Jan",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_feb",            "Tiered Rate Charge in Feb",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_mar",            "Tiered Rate Charge in Mar",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_apr",            "Tiered Rate Charge in Apr",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_may",            "Tiered Rate Charge in May",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_jun",            "Tiered Rate Charge in Jun",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_jul",            "Tiered Rate Charge in Jul",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_aug",            "Tiered Rate Charge in Aug",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_sep",            "Tiered Rate Charge in Sep",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_oct",            "Tiered Rate Charge in Oct",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_nov",            "Tiered Rate Charge in Nov",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	{ SSC_OUTPUT,       SSC_ARRAY,      "charge_tr_dec",            "Tiered Rate Charge in Dec",       "$",      "",                      "",             "*",                         "LENGTH_EQUAL=analysis_years",   "" },
	
var_info_invalid };

class cm_utilityrate : public compute_module
{
private:
public:
	cm_utilityrate()
	{
		add_var_info( vtab_utility_rate );
	}

	void exec( ) throw( general_error )
	{
		ssc_number_t *parr = 0;
		size_t count, i, j;

		size_t nyears = (size_t)as_integer("analysis_years");

		// compute annual system output degradation multipliers
		std::vector<ssc_number_t> sys_scale(nyears);
		parr = as_array("system_degradation", &count);
		if (count == 1)
		{
			for (i=0;i<nyears;i++)
				sys_scale[i] = (ssc_number_t) pow( (double)(1-parr[0]*0.01), (double)i );
		}
		else
		{
			for (i=0;i<nyears && i<count;i++)
				sys_scale[i] = (ssc_number_t)(1.0 - parr[i]*0.01);
		}

		// compute load (electric demand) annual escalation multipliers
		std::vector<ssc_number_t> load_scale(nyears);
		parr = as_array("load_escalation", &count);
		if (count == 1)
		{
			// TODO: add in inflation rate
			for (i=0;i<nyears;i++)
				load_scale[i] = (ssc_number_t)pow( (double)(/*inflation_rate+*/1+parr[0]*0.01), (double)i );
		}
		else
		{
			for (i=0;i<nyears;i++)
				load_scale[i] = (ssc_number_t)(1 + parr[i]*0.01);
		}

		// compute utility rate out-years escalation multipliers
		std::vector<ssc_number_t> rate_scale(nyears);
		parr = as_array("rate_escalation", &count);
		if (count == 1)
		{
			for (i=0;i<nyears;i++)
				rate_scale[i] = (ssc_number_t)pow( (double)(/*inflation_rate+*/1+parr[0]*0.01), (double)i );
		}
		else
		{
			for (i=0;i<nyears;i++)
				rate_scale[i] = (ssc_number_t)(1 + parr[i]*0.01);
		}


		// prepare 8760 arrays for load and grid values
		std::vector<ssc_number_t> e_sys(8760), p_sys(8760), 
			e_load(8760), p_load(8760),
			e_grid(8760), p_grid(8760),
			e_load_cy(8760), p_load_cy(8760); // current year load (accounts for escal)
		
		parr = as_array("e_with_system", &count);
		for (i=0;i<8760;i++)
		{
			e_sys[i] = p_sys[i] = parr[i]; // by default p_sys = e_sys (since it's hourly)
			// others are 0.0
			e_grid[i] = p_grid[i] = e_load[i] = p_load[i] = e_load_cy[i] = p_load_cy[i] = 0.0;
		}

		if (is_assigned("p_with_system"))
		{
			parr = as_array("p_with_system", &count);
			if (count != 8760) throw general_error("p_with_system must have 8760 values");
			for (i=0;i<8760;i++)
				p_sys[i] = parr[i];
		}

		if (is_assigned("e_without_system"))
		{
			parr = as_array("e_without_system", &count);
			if (count != 8760) throw general_error("e_without_system must have 8760 values");
			for (i=0;i<8760;i++)
			{
				e_load[i] = parr[i];
				p_load[i] = parr[i]; // by default p_load = e_load
			}
		}

		if (is_assigned("p_without_system"))
		{
			parr = as_array("p_without_system", &count);
			if (count != 8760) throw general_error("p_without_system must have 8760 values");
			for (i=0;i<8760;i++)
				p_load[i] = parr[i];
		}

		/* allocate intermediate data arrays */
		std::vector<ssc_number_t> revenue_w_sys(8760), revenue_wo_sys(8760),
			payment(8760), income(8760), price(8760);
		std::vector<ssc_number_t> monthly_revenue_w_sys(12), monthly_revenue_wo_sys(12),
			monthly_fixed_charges(12),
			monthly_dc_fixed(12), monthly_dc_tou(12),
			monthly_tr_charges(12), monthly_tr_rates(12);

		/* allocate outputs */		
		ssc_number_t *annual_net_revenue = allocate("energy_value", nyears);
		ssc_number_t *energy_net = allocate("energy_net", nyears);
		ssc_number_t *annual_revenue_w_sys = allocate("revenue_with_system", nyears);
		ssc_number_t *annual_revenue_wo_sys = allocate("revenue_without_system", nyears);

		ssc_number_t *ch_dc_fixed_jan = allocate("charge_dc_fixed_jan", nyears );
		ssc_number_t *ch_dc_fixed_feb = allocate("charge_dc_fixed_feb", nyears );
		ssc_number_t *ch_dc_fixed_mar = allocate("charge_dc_fixed_mar", nyears );
		ssc_number_t *ch_dc_fixed_apr = allocate("charge_dc_fixed_apr", nyears );
		ssc_number_t *ch_dc_fixed_may = allocate("charge_dc_fixed_may", nyears );
		ssc_number_t *ch_dc_fixed_jun = allocate("charge_dc_fixed_jun", nyears );
		ssc_number_t *ch_dc_fixed_jul = allocate("charge_dc_fixed_jul", nyears );
		ssc_number_t *ch_dc_fixed_aug = allocate("charge_dc_fixed_aug", nyears );
		ssc_number_t *ch_dc_fixed_sep = allocate("charge_dc_fixed_sep", nyears );
		ssc_number_t *ch_dc_fixed_oct = allocate("charge_dc_fixed_oct", nyears );
		ssc_number_t *ch_dc_fixed_nov = allocate("charge_dc_fixed_nov", nyears );
		ssc_number_t *ch_dc_fixed_dec = allocate("charge_dc_fixed_dec", nyears );
		
		ssc_number_t *ch_dc_tou_jan = allocate("charge_dc_tou_jan", nyears );
		ssc_number_t *ch_dc_tou_feb = allocate("charge_dc_tou_feb", nyears );
		ssc_number_t *ch_dc_tou_mar = allocate("charge_dc_tou_mar", nyears );
		ssc_number_t *ch_dc_tou_apr = allocate("charge_dc_tou_apr", nyears );
		ssc_number_t *ch_dc_tou_may = allocate("charge_dc_tou_may", nyears );
		ssc_number_t *ch_dc_tou_jun = allocate("charge_dc_tou_jun", nyears );
		ssc_number_t *ch_dc_tou_jul = allocate("charge_dc_tou_jul", nyears );
		ssc_number_t *ch_dc_tou_aug = allocate("charge_dc_tou_aug", nyears );
		ssc_number_t *ch_dc_tou_sep = allocate("charge_dc_tou_sep", nyears );
		ssc_number_t *ch_dc_tou_oct = allocate("charge_dc_tou_oct", nyears );
		ssc_number_t *ch_dc_tou_nov = allocate("charge_dc_tou_nov", nyears );
		ssc_number_t *ch_dc_tou_dec = allocate("charge_dc_tou_dec", nyears );
		
		ssc_number_t *ch_tr_jan = allocate("charge_tr_jan", nyears );
		ssc_number_t *ch_tr_feb = allocate("charge_tr_feb", nyears );
		ssc_number_t *ch_tr_mar = allocate("charge_tr_mar", nyears );
		ssc_number_t *ch_tr_apr = allocate("charge_tr_apr", nyears );
		ssc_number_t *ch_tr_may = allocate("charge_tr_may", nyears );
		ssc_number_t *ch_tr_jun = allocate("charge_tr_jun", nyears );
		ssc_number_t *ch_tr_jul = allocate("charge_tr_jul", nyears );
		ssc_number_t *ch_tr_aug = allocate("charge_tr_aug", nyears );
		ssc_number_t *ch_tr_sep = allocate("charge_tr_sep", nyears );
		ssc_number_t *ch_tr_oct = allocate("charge_tr_oct", nyears );
		ssc_number_t *ch_tr_nov = allocate("charge_tr_nov", nyears );
		ssc_number_t *ch_tr_dec = allocate("charge_tr_dec", nyears );

		for (i=0;i<nyears;i++)
		{
			for (j=0;j<8760;j++)
			{
				// apply load escalation appropriate for current year
				e_load_cy[j] = e_load[j] * load_scale[i];
				p_load_cy[j] = p_load[j] * load_scale[i];

				// calculate e_grid value (e_sys + e_load)
				// note: load is assumed to have negative sign

				e_grid[j] = e_sys[j]*sys_scale[i] + e_load_cy[j];
				p_grid[j] = p_sys[j]*sys_scale[i] + p_load_cy[j];
			}

			// calculate revenue with solar system (using net grid energy & maxpower)
			ur_calc( &e_grid[0], &p_grid[0],
				&revenue_w_sys[0], &payment[0], &income[0], &price[0],
				&monthly_fixed_charges[0],
				&monthly_dc_fixed[0], &monthly_dc_tou[0],
				&monthly_tr_charges[0], &monthly_tr_rates[0] );

			if (i == 0)
			{
				assign( "year1_hourly_revenue_with_system", var_data( &revenue_w_sys[0], 8760 ) );
				assign( "year1_hourly_payment_with_system", var_data( &payment[0], 8760 ) );
				assign( "year1_hourly_income_with_system", var_data( &income[0], 8760 ) );
				assign( "year1_hourly_price_with_system", var_data( &price[0], 8760 ) );
				assign( "year1_hourly_e_grid", var_data( &e_grid[0], 8760 ) );
				assign( "year1_hourly_p_grid", var_data( &p_grid[0], 8760 ) );
				
				// output and demand per Paul's email 9/10/10
				// positive demand indicates system does not produce enough electricity to meet load
				// zero if the system produces more than the demand
				std::vector<ssc_number_t> output(8760), edemand(8760), pdemand(8760);
				for (j=0;j<8760;j++)
				{
					output[j] = e_sys[j] * sys_scale[i];
					edemand[j] = e_grid[j] < 0.0 ? -e_grid[j] : (ssc_number_t)0.0;
					pdemand[j] = p_grid[j] < 0.0 ? -p_grid[j] : (ssc_number_t)0.0;
				}

				assign( "year1_hourly_system_output", var_data(&output[0], 8760) );
				assign( "year1_hourly_e_demand", var_data(&edemand[0], 8760) );
				assign( "year1_hourly_p_demand", var_data(&pdemand[0], 8760) );
				
				assign( "year1_monthly_dc_fixed_with_system", var_data(&monthly_dc_fixed[0], 12) );
				assign( "year1_monthly_dc_tou_with_system", var_data(&monthly_dc_tou[0], 12) );
				assign( "year1_monthly_tr_charge_with_system", var_data(&monthly_tr_charges[0], 12) );
				assign( "year1_monthly_tr_rate_with_system", var_data(&monthly_tr_rates[0], 12) );
			}

			// now recalculate revenue without solar system (using load only)
			ur_calc( &e_load_cy[0], &p_load_cy[0],
				&revenue_wo_sys[0], &payment[0], &income[0], &price[0],
				&monthly_fixed_charges[0],
				&monthly_dc_fixed[0], &monthly_dc_tou[0],
				&monthly_tr_charges[0], &monthly_tr_rates[0] );

			if (i == 0)
			{
				assign( "year1_hourly_revenue_without_system", var_data( &revenue_wo_sys[0], 8760 ) );
				assign( "year1_hourly_payment_without_system", var_data( &payment[0], 8760 ) );
				assign( "year1_hourly_income_without_system", var_data( &income[0], 8760 ) );
				assign( "year1_hourly_price_without_system", var_data( &price[0], 8760 ) );
								
				assign( "year1_monthly_dc_fixed_without_system", var_data(&monthly_dc_fixed[0], 12) );
				assign( "year1_monthly_dc_tou_without_system", var_data(&monthly_dc_tou[0], 12) );
				assign( "year1_monthly_tr_charge_without_system", var_data(&monthly_tr_charges[0], 12) );
				assign( "year1_monthly_tr_rate_without_system", var_data(&monthly_tr_rates[0], 12) );
			}

			// determine net-revenue benefit due to solar for year 'i'
			
			annual_net_revenue[i] = 0.0;
			energy_net[i] = 0.0;
			annual_revenue_w_sys[i] = 0.0;
			annual_revenue_wo_sys[i] = 0.0;

			for(j=0;j<8760;j++)
			{
				energy_net[i] +=  e_sys[j]*sys_scale[i];
				annual_net_revenue[i] += revenue_w_sys[j] - revenue_wo_sys[j];
				annual_revenue_w_sys[i] += revenue_w_sys[j];
				annual_revenue_wo_sys[i] += revenue_wo_sys[j];
			}

			annual_net_revenue[i] *= rate_scale[i];
			annual_revenue_w_sys[i] *= rate_scale[i];
			annual_revenue_wo_sys[i] *= rate_scale[i];

			ch_dc_fixed_jan[i] = monthly_dc_fixed[0] * rate_scale[i];
			ch_dc_fixed_feb[i] = monthly_dc_fixed[1] * rate_scale[i];
			ch_dc_fixed_mar[i] = monthly_dc_fixed[2] * rate_scale[i];
			ch_dc_fixed_apr[i] = monthly_dc_fixed[3] * rate_scale[i];
			ch_dc_fixed_may[i] = monthly_dc_fixed[4] * rate_scale[i];
			ch_dc_fixed_jun[i] = monthly_dc_fixed[5] * rate_scale[i];
			ch_dc_fixed_jul[i] = monthly_dc_fixed[6] * rate_scale[i];
			ch_dc_fixed_aug[i] = monthly_dc_fixed[7] * rate_scale[i];
			ch_dc_fixed_sep[i] = monthly_dc_fixed[8] * rate_scale[i];
			ch_dc_fixed_oct[i] = monthly_dc_fixed[9] * rate_scale[i];
			ch_dc_fixed_nov[i] = monthly_dc_fixed[10] * rate_scale[i];
			ch_dc_fixed_dec[i] = monthly_dc_fixed[11] * rate_scale[i];
		
			ch_dc_tou_jan[i] = monthly_dc_tou[0] * rate_scale[i];
			ch_dc_tou_feb[i] = monthly_dc_tou[1] * rate_scale[i];
			ch_dc_tou_mar[i] = monthly_dc_tou[2] * rate_scale[i];
			ch_dc_tou_apr[i] = monthly_dc_tou[3] * rate_scale[i];
			ch_dc_tou_may[i] = monthly_dc_tou[4] * rate_scale[i];
			ch_dc_tou_jun[i] = monthly_dc_tou[5] * rate_scale[i];
			ch_dc_tou_jul[i] = monthly_dc_tou[6] * rate_scale[i];
			ch_dc_tou_aug[i] = monthly_dc_tou[7] * rate_scale[i];
			ch_dc_tou_sep[i] = monthly_dc_tou[8] * rate_scale[i];
			ch_dc_tou_oct[i] = monthly_dc_tou[9] * rate_scale[i];
			ch_dc_tou_nov[i] = monthly_dc_tou[10] * rate_scale[i];
			ch_dc_tou_dec[i] = monthly_dc_tou[11] * rate_scale[i];
		
			ch_tr_jan[i] = monthly_tr_charges[0] * rate_scale[i];
			ch_tr_feb[i] = monthly_tr_charges[1] * rate_scale[i];
			ch_tr_mar[i] = monthly_tr_charges[2] * rate_scale[i];
			ch_tr_apr[i] = monthly_tr_charges[3] * rate_scale[i];
			ch_tr_may[i] = monthly_tr_charges[4] * rate_scale[i];
			ch_tr_jun[i] = monthly_tr_charges[5] * rate_scale[i];
			ch_tr_jul[i] = monthly_tr_charges[6] * rate_scale[i];
			ch_tr_aug[i] = monthly_tr_charges[7] * rate_scale[i];
			ch_tr_sep[i] = monthly_tr_charges[8] * rate_scale[i];
			ch_tr_oct[i] = monthly_tr_charges[9] * rate_scale[i];
			ch_tr_nov[i] = monthly_tr_charges[10] * rate_scale[i];
			ch_tr_dec[i] = monthly_tr_charges[11] * rate_scale[i];			
		}
	
	}

	void ur_calc( ssc_number_t e_in[8760], ssc_number_t p_in[8760],

		ssc_number_t revenue[8760], ssc_number_t payment[8760], ssc_number_t income[8760], ssc_number_t price[8760],
		ssc_number_t monthly_fixed_charges[12],
		ssc_number_t monthly_dc_fixed[12], ssc_number_t monthly_dc_tou[12],
		ssc_number_t monthly_tr_charges[12], ssc_number_t monthly_tr_rates[12] ) throw(general_error)
	{
		int i;

		for (i=0;i<8760;i++)
			revenue[i] = payment[i] = income[i] = price[i] = 0.0;

		for (i=0;i<12;i++)
		{
			monthly_fixed_charges[i] 
				= monthly_dc_fixed[i] = monthly_dc_tou[i] 
				= monthly_tr_charges[i] = monthly_tr_rates[i] = 0.0;
		}

		// process basic flat rate
		process_flat_rate( e_in, payment, income, price );

		// process monthly fixed charges
		process_monthly_charge( payment, monthly_fixed_charges );

		// process time of use charges
		if (as_boolean("ur_tou_enable"))
			process_tou_rate( e_in, payment, income, price );

		// process demand charges
		if (as_boolean("ur_dc_enable"))
			process_demand_charge( p_in, payment, monthly_dc_fixed, monthly_dc_tou );

		// process tiered rate charges
		if (as_boolean("ur_tr_enable"))
			process_tiered_rate( e_in, payment, income, monthly_tr_charges, monthly_tr_rates );

		// compute revenue ( = income - payment )
		for (i=0;i<8760;i++)
			revenue[i] = income[i] - payment[i];
	}

	void process_flat_rate( ssc_number_t e[8760],
			ssc_number_t payment[8760],
			ssc_number_t income[8760],
			ssc_number_t price[8760] )
	{
		int i;
		ssc_number_t buy = as_number("ur_flat_buy_rate");
		ssc_number_t sell = as_number("ur_flat_sell_rate");
		ssc_number_t fuel = as_number("ur_flat_fuel_rate");

		if (as_boolean("ur_sell_eq_buy")) sell = buy;

		for (i=0;i<8760;i++)
		{
			if (e[i] < 0) // must buy from grid
			{
				payment[i] += -1.0f*e[i]*(buy+fuel);
				price[i] += (buy+fuel);
			}
			else
			{
				income[i] += e[i]*sell;
				price[i] += sell;
			}
		}
	}

	void process_monthly_charge( ssc_number_t payment[8760], ssc_number_t charges[12] )
	{
		int m,d,h,c;

		ssc_number_t fixed = as_number("ur_monthly_fixed_charge");
		c=0;
		for (m=0;m<12;m++)
		{
			for (d=0;d<util::nday[m];d++)
			{
				for (h=0;h<24;h++)
				{
					if ( d==util::nday[m]-1 && h == 23)
					{
						charges[m] = fixed;
						payment[c] += fixed;
					}
					c++;
				}
			}
		}
	}

	void process_tou_rate( ssc_number_t e[8760],
			ssc_number_t payment[8760],
			ssc_number_t income[8760],
			ssc_number_t price[8760] )
	{
		ssc_number_t rates[9][3];

		const char *schedwkday = as_string("ur_tou_sched_weekday");
		const char *schedwkend = as_string("ur_tou_sched_weekend");

		int tod[8760];

		if (!util::translate_schedule( tod, schedwkday, schedwkend, 0, 8))
			throw general_error("could not translate weekday and weekend schedules for time-of-use rate");

		bool sell_eq_buy = as_boolean("ur_sell_eq_buy");

		for (int i=0;i<9;i++)
		{
			std::string nstr = util::to_string( i+1 );
			rates[i][0] = as_number("ur_tou_p" + nstr + "_buy_rate");
			rates[i][1] = sell_eq_buy ? rates[i][0] : as_number("ur_tou_p" + nstr + "_sell_rate");
			rates[i][2] = as_number("ur_tou_p" + nstr + "_fuel_rate");
		}

		for (int i=0;i<8760;i++)
		{
			int tod_p = tod[i];
			ssc_number_t buy = rates[tod_p][0];
			ssc_number_t sell = rates[tod_p][1];
			ssc_number_t fuel = rates[tod_p][2];

			if (e[i] < 0)
			{
				payment[i] += -1.0f*e[i]*(buy+fuel);
				price[i] += (buy+fuel);
			}
			else
			{
				income[i] += e[i]*sell;
				price[i] += sell;
			}
		}
	}

	void process_demand_charge( ssc_number_t p[8760],
			ssc_number_t payment[8760],
			ssc_number_t dc_fixed[12],
			ssc_number_t dc_tou[12] )
	{
		int i,m,d,h,c;


		// compute fixed monthly demand charges
		c=0;
		for (m=0;m<12;m++)
		{
			ssc_number_t charge = 0.0;
			ssc_number_t mpeak = 0.0;
			for (d=0;d<util::nday[m];d++)
			{
				for (h=0;h<24;h++)
				{
					if (p[c] < 0 && p[c] < mpeak)
						mpeak = p[c];

					if (d==util::nday[m]-1 && h==23)
					{
						charge = as_number(util::format("ur_dc_fixed_m%d", m+1));
						dc_fixed[m] = -mpeak*charge;
						payment[c] += dc_fixed[m];
					}

					c++;
				}
			}
		}


		// compute time-of-use based demand charge
		// for each month:
		// 1. find peak demand in each period (1-9)
		// 2. multiply each period's peak demand by period price and add to payment for that month

		// extract schedules
		const char *schedwkday = as_string("ur_dc_sched_weekday");
		const char *schedwkend = as_string("ur_dc_sched_weekend");
		int tod[8760];
		if (!util::translate_schedule( tod, schedwkday, schedwkend, 0, 8))
			throw general_error("could not translate weekday and weekend schedules for demand charge time-of-use rate");

		// extract rate info
		ssc_number_t period_price[9];
		for (i=0;i<9;i++)
			period_price[i] = as_number(util::format("ur_dc_p%d", i+1));

		ssc_number_t ppeaks[9];
		c=0;
		for (m=0;m<12;m++)
		{
			for (i=0;i<9;i++) ppeaks[i] = 0;

			for (d=0;d<util::nday[m];d++)
			{
				for(h=0;h<24;h++)
				{
					int todp = tod[c];
					if (p[c] < 0 && p[c] < ppeaks[todp])
						ppeaks[todp] = p[c];

					if (d==util::nday[m]-1 && h==23)
					{
						// sum up all peak demand charges at end of month
						ssc_number_t charge=0;
						for (i=0;i<9;i++)
							charge += -ppeaks[i]*period_price[i];

						// add to payments
						dc_tou[m] = charge;
						payment[c] += charge;
					}

					c++;
				}
			}
		}

	}

	void process_tiered_rate( ssc_number_t e[8760],
			ssc_number_t payment[8760],
			ssc_number_t income[8760],
			ssc_number_t tr_charge[12],
			ssc_number_t tr_rate[12] )
	{
		int i,m,d,h,c;

		ssc_number_t energy_ub[6];
		ssc_number_t rates[6];
		ssc_number_t sell_rate=0, tier_rate=0;

		c=0;
		for (m=0;m<12;m++)
		{
			// compute total monthly energy use (net)
			ssc_number_t energy_use = 0;
			for (d=0;d<util::nday[m];d++)
			{
				for (h=0;h<24;h++)
				{
					energy_use += -1.0f*e[c]; // add up total energy use
					c++;
				}
			}


			int period = as_integer(util::format("ur_tr_sched_m%d",m+1)); // 0..5
			if (period < 0) period = 0;
			if (period > 5) period = 5;

			if (energy_use > 0)
			{

				for (i=0;i<6;i++)
				{
					energy_ub[i] = as_number(util::format("ur_tr_s%d_energy_ub%d", period+1, i+1));
					rates[i] = as_number(util::format("ur_tr_s%d_rate%d", period+1, i+1));
				}
				
				ssc_number_t charge_amt = 0;
				i=0;
				while (i<6)
				{
					// add up the charge amount for this block
					ssc_number_t e_upper = energy_ub[i];
					ssc_number_t e_lower = i > 0 ? energy_ub[i-1] : (ssc_number_t)0.0;
	
					if (energy_use > e_upper)
						charge_amt += (e_upper-e_lower)*rates[i];
					else
						charge_amt += (energy_use-e_lower)*rates[i];
	
					if ( energy_use < e_upper )
						break;
	
					i++;
				}
	
				tr_rate[m] = energy_use > 0 ? charge_amt/energy_use : (ssc_number_t)0.0;
				tr_charge[m] = charge_amt;
				payment[c-1] += charge_amt;
			}
			else // sell excess
			{
				tr_rate[m] = 0;
				tr_charge[m] = 0;
				sell_rate = 0;
				switch (as_integer("ur_tr_sell_mode"))
				{
				case 0: // flat tiered sell rate
					sell_rate = as_number("ur_tr_sell_rate");
					break;
				case 1: // Tier 1 rate
					sell_rate = as_number(util::format("ur_tr_s%d_rate1", period+1));
					break;
				case 2: // cheapest rate for tier structure
					sell_rate = as_number(util::format("ur_tr_s%d_rate1", period+1));
					for (int j=1; j<6; j++)
					{
						tier_rate = as_number(util::format("ur_tr_s%d_rate%d", period+1, j+1));
						if (tier_rate < sell_rate) sell_rate = tier_rate;
					}
					break;
				default:
					throw general_error("invalid sell rate mode. must be 0, 1, or 2");
					break;
				}

				income[c-1] -= sell_rate * energy_use;
			}
		}
	
	}


};

DEFINE_MODULE_ENTRY( utilityrate, "Complex utility rate structure net revenue calculator", 3 );


