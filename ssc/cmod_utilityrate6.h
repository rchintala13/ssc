#ifndef _CMOD_UTILITYRATE6_H_
#define _CMOD_UTILITYRATE6_H_

#include "core.h"
#include "lib_utility_rate.h"

class cm_utilityrate6 : public compute_module
{
public:

	/// Default constructor
	cm_utilityrate6();

	/// Default destructor
	~cm_utilityrate6();

	/// construct since compute_module framework is fundamentally broken
	void construct();

	/// Main execution
	void exec() throw(general_error);

	/// Generate monthly outputs
	void monthly_outputs(ssc_number_t *e_load, ssc_number_t *e_sys, ssc_number_t *e_grid, ssc_number_t *salespurchases, ssc_number_t monthly_load[12], ssc_number_t monthly_generation[12], ssc_number_t monthly_elec_to_grid[12], ssc_number_t monthly_elec_needed_from_grid[12], ssc_number_t monthly_salespurchases[12]);

	/// Setup rate
	void setup();

	/// Parse URDB JSON into existing utility rate structures
	void parse_urdb_json();

	/// Calculate bill
	void ur_calc(ssc_number_t *e_in, ssc_number_t *p_in,
		ssc_number_t *revenue, ssc_number_t *payment, ssc_number_t *income,
		ssc_number_t *demand_charge, ssc_number_t *energy_charge,
		ssc_number_t monthly_fixed_charges[12], ssc_number_t monthly_minimum_charges[12],
		ssc_number_t monthly_dc_fixed[12], ssc_number_t monthly_dc_tou[12],
		ssc_number_t monthly_ec_charges[12],
		ssc_number_t monthly_ec_charges_gross[12],
		ssc_number_t excess_dollars_earned[12],
		ssc_number_t excess_dollars_applied[12],
		ssc_number_t excess_kwhs_earned[12],
		ssc_number_t excess_kwhs_applied[12],
		ssc_number_t *dc_hourly_peak, ssc_number_t monthly_cumulative_excess_energy[12],
		ssc_number_t monthly_cumulative_excess_dollars[12], ssc_number_t monthly_bill[12],
		ssc_number_t rate_esc, size_t year, bool include_fixed = true, bool include_min = true, bool gen_only = false) throw (general_error);

	/// Calculate timestep
	void ur_calc_timestep(ssc_number_t *e_in, ssc_number_t *p_in,
		ssc_number_t *revenue, ssc_number_t *payment, ssc_number_t *income,
		ssc_number_t *demand_charge, ssc_number_t *energy_charge,
		ssc_number_t monthly_fixed_charges[12], ssc_number_t monthly_minimum_charges[12],
		ssc_number_t monthly_dc_fixed[12], ssc_number_t monthly_dc_tou[12],
		ssc_number_t monthly_ec_charges[12],
		ssc_number_t monthly_ec_charges_gross[12],
		ssc_number_t excess_dollars_earned[12],
		ssc_number_t excess_dollars_applied[12],
		ssc_number_t excess_kwhs_earned[12],
		ssc_number_t excess_kwhs_applied[12],
		ssc_number_t *dc_hourly_peak, ssc_number_t monthly_cumulative_excess_energy[12],
		ssc_number_t monthly_cumulative_excess_dollars[12], ssc_number_t monthly_bill[12],
		ssc_number_t rate_esc, bool include_fixed = true, bool include_min = true, bool gen_only = false)
		throw(general_error);

	/// Update Monthly Energy charge
	void ur_update_ec_monthly(int month, util::matrix_t<double>& charge, util::matrix_t<double>& energy, util::matrix_t<double>& surplus)
		throw(general_error);

	/// Allocate Outputs
	void allocateOutputs();

protected:

	size_t nyears;
	size_t nrec_gen;

	std::unique_ptr<UtilityRate> m_utilityRate;

	// schedule outputs
	std::vector<int> m_ec_tou_sched;
	std::vector<int> m_dc_tou_sched;
	std::vector<ur_month> m_month;
	std::vector<int> m_ec_periods; // period number

	// time step sell rate
	std::vector<ssc_number_t> m_ec_ts_sell_rate;

	// track initial values - may change based on units
	std::vector<std::vector<int> >  m_ec_periods_tiers_init; // tier numbers
	std::vector<int> m_dc_tou_periods; // period number
	std::vector<std::vector<int> >  m_dc_tou_periods_tiers; // tier numbers
	std::vector<std::vector<int> >  m_dc_flat_tiers; // tier numbers for each month of flat demand charge
	size_t m_num_rec_yearly;

	// outputs
	ssc_number_t *annual_net_revenue;
	ssc_number_t *annual_electric_load;
	ssc_number_t *energy_net;
	ssc_number_t *annual_revenue_w_sys;
	ssc_number_t *annual_revenue_wo_sys;
	ssc_number_t *annual_elec_cost_w_sys;
	ssc_number_t *annual_elec_cost_wo_sys;

	ssc_number_t *utility_bill_w_sys_ym;
	ssc_number_t *utility_bill_wo_sys_ym;
	ssc_number_t *ch_w_sys_dc_fixed_ym;
	ssc_number_t *ch_w_sys_dc_tou_ym;
	ssc_number_t *ch_w_sys_ec_ym;

	ssc_number_t *ch_w_sys_ec_gross_ym;
	ssc_number_t *excess_dollars_applied_ym;
	ssc_number_t *excess_dollars_earned_ym;
	ssc_number_t *excess_kwhs_applied_ym;
	ssc_number_t *excess_kwhs_earned_ym;


	ssc_number_t *ch_wo_sys_dc_fixed_ym;
	ssc_number_t *ch_wo_sys_dc_tou_ym;
	ssc_number_t *ch_wo_sys_ec_ym;
	ssc_number_t *ch_w_sys_fixed_ym;
	ssc_number_t *ch_wo_sys_fixed_ym;
	ssc_number_t *ch_w_sys_minimum_ym;
	ssc_number_t *ch_wo_sys_minimum_ym;

	ssc_number_t *utility_bill_w_sys;
	ssc_number_t *utility_bill_wo_sys;
	ssc_number_t *ch_w_sys_dc_fixed;
	ssc_number_t *ch_w_sys_dc_tou;
	ssc_number_t *ch_w_sys_ec;
	ssc_number_t *ch_wo_sys_dc_fixed;
	ssc_number_t *ch_wo_sys_dc_tou;
	ssc_number_t *ch_wo_sys_ec;
	ssc_number_t *ch_w_sys_fixed;
	ssc_number_t *ch_wo_sys_fixed;
	ssc_number_t *ch_w_sys_minimum;
	ssc_number_t *ch_wo_sys_minimum;

	ssc_number_t *year1_hourly_e_togrid;
	ssc_number_t *year1_hourly_e_fromgrid;

	util::matrix_t<ssc_number_t> charge_wo_sys_ec_jan_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_feb_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_mar_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_apr_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_may_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_jun_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_jul_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_aug_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_sep_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_oct_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_nov_tp;
	util::matrix_t<ssc_number_t> charge_wo_sys_ec_dec_tp;
	
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_jan_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_feb_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_mar_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_apr_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_may_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_jun_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_jul_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_aug_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_sep_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_oct_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_nov_tp;
	util::matrix_t<ssc_number_t> energy_wo_sys_ec_dec_tp;

	// no longer output but still updated internally in ur
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_jan_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_feb_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_mar_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_apr_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_may_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_jun_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_jul_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_aug_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_sep_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_oct_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_nov_tp;
	util::matrix_t<ssc_number_t> surplus_wo_sys_ec_dec_tp;

	util::matrix_t<ssc_number_t> charge_w_sys_ec_jan_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_feb_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_mar_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_apr_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_may_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_jun_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_jul_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_aug_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_sep_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_oct_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_nov_tp;
	util::matrix_t<ssc_number_t> charge_w_sys_ec_dec_tp;

	util::matrix_t<ssc_number_t> energy_w_sys_ec_jan_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_feb_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_mar_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_apr_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_may_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_jun_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_jul_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_aug_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_sep_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_oct_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_nov_tp;
	util::matrix_t<ssc_number_t> energy_w_sys_ec_dec_tp;


	util::matrix_t<ssc_number_t> surplus_w_sys_ec_jan_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_feb_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_mar_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_apr_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_may_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_jun_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_jul_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_aug_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_sep_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_oct_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_nov_tp;
	util::matrix_t<ssc_number_t> surplus_w_sys_ec_dec_tp;

	util::matrix_t<ssc_number_t> monthly_tou_demand_peak_w_sys;
	util::matrix_t<ssc_number_t> monthly_tou_demand_peak_wo_sys;
	util::matrix_t<ssc_number_t> monthly_tou_demand_charge_w_sys;
	util::matrix_t<ssc_number_t> monthly_tou_demand_charge_wo_sys;

	ssc_number_t *lifetime_load;
};





#endif
