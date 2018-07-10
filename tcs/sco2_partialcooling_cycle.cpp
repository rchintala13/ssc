/*******************************************************************************************************
*  Copyright 2017 Alliance for Sustainable Energy, LLC
*
*  NOTICE: This software was developed at least in part by Alliance for Sustainable Energy, LLC
*  (�Alliance�) under Contract No. DE-AC36-08GO28308 with the U.S. Department of Energy and the U.S.
*  The Government retains for itself and others acting on its behalf a nonexclusive, paid-up,
*  irrevocable worldwide license in the software to reproduce, prepare derivative works, distribute
*  copies to the public, perform publicly and display publicly, and to permit others to do so.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted
*  provided that the following conditions are met:
*
*  1. Redistributions of source code must retain the above copyright notice, the above government
*  rights notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright notice, the above government
*  rights notice, this list of conditions and the following disclaimer in the documentation and/or
*  other materials provided with the distribution.
*
*  3. The entire corresponding source code of any redistribution, with or without modification, by a
*  research entity, including but not limited to any contracting manager/operator of a United States
*  National Laboratory, any institution of higher learning, and any non-profit organization, must be
*  made publicly available under this license for as long as the redistribution is made available by
*  the research entity.
*
*  4. Redistribution of this software, without modification, must refer to the software by the same
*  designation. Redistribution of a modified version of this software (i) may not refer to the modified
*  version by the same designation, or by any confusingly similar designation, and (ii) must refer to
*  the underlying software originally provided by Alliance as �System Advisor Model� or �SAM�. Except
*  to comply with the foregoing, the terms �System Advisor Model�, �SAM�, or any confusingly similar
*  designation may not be used to refer to any modified version of this software or any modified
*  version of the underlying software originally provided by Alliance without the prior written consent
*  of Alliance.
*
*  5. The name of the copyright holder, contributors, the United States Government, the United States
*  Department of Energy, or any of their employees may not be used to endorse or promote products
*  derived from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER,
*  CONTRIBUTORS, UNITED STATES GOVERNMENT OR UNITED STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR
*  EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
*  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************************************/

#include "sco2_partialcooling_cycle.h"

#include <algorithm>
#include <string>

#include "nlopt.hpp"
#include "fmin.h"

int C_PartialCooling_Cycle::design(S_des_params & des_par_in)
{
	ms_des_par = des_par_in;

	return design_core();
}

int C_PartialCooling_Cycle::design_core()
{
	// Check that the recompression fraction is not too close to 0
	if (ms_des_par.m_recomp_frac < 0.01)
	{
		ms_des_par.m_recomp_frac = 0.0;
		double UA_tot = ms_des_par.m_UA_LTR + ms_des_par.m_UA_HTR;		//[kW/K]
		ms_des_par.m_UA_LTR = UA_tot;		//[kW/K]
		ms_des_par.m_UA_HTR = 0.0;			//[kW/K]
	}

	// Initialize Recuperators
	mc_LTR.initialize(ms_des_par.m_N_sub_hxrs);
	mc_HTR.initialize(ms_des_par.m_N_sub_hxrs);

	// Initialize known temps and pressures from design parameters
	m_temp_last[MC_IN] = ms_des_par.m_T_mc_in;	//[K]
	m_pres_last[MC_IN] = ms_des_par.m_P_mc_in;	//[kPa]
	m_temp_last[PC_IN] = ms_des_par.m_T_pc_in;	//[K]
	m_pres_last[PC_IN] = ms_des_par.m_P_pc_in;	//[kPa]
	m_temp_last[TURB_IN] = ms_des_par.m_T_t_in;	//[K]
	m_pres_last[MC_OUT] = ms_des_par.m_P_mc_out;//[kPa]

	// Apply design pressure drops to heat exchangers to fully define pressures at all states
	if (ms_des_par.m_DP_LTR[0] < 0.0)
		m_pres_last[LTR_HP_OUT] = m_pres_last[MC_OUT] * (1.0 - fabs(ms_des_par.m_DP_LTR[0]));	//[kPa]
	else
		m_pres_last[LTR_HP_OUT] = m_pres_last[MC_OUT] - ms_des_par.m_DP_LTR[0];		//[kPa]

	if (ms_des_par.m_UA_LTR < 1.0E-12)
		m_pres_last[LTR_HP_OUT] = m_pres_last[MC_OUT];	//[kPa] If no LTR then no pressure drop

	m_pres_last[MIXER_OUT] = m_pres_last[LTR_HP_OUT];	//[kPa] assume no pressure drop in mixer
	m_pres_last[RC_OUT] = m_pres_last[LTR_HP_OUT];		//[kPa] assume no pressure drop in mixer

	if (ms_des_par.m_DP_HTR[0] < 0.0)
		m_pres_last[HTR_HP_OUT] = m_pres_last[MIXER_OUT] * (1.0 - fabs(ms_des_par.m_DP_HTR[0]));	//[kPa]
	else
		m_pres_last[HTR_HP_OUT] = m_pres_last[MIXER_OUT] - ms_des_par.m_DP_HTR[0];	//[kPa]

	if (ms_des_par.m_UA_HTR < 1.0E-12)
		m_pres_last[HTR_HP_OUT] = m_pres_last[MIXER_OUT];	//[kPa] If no HTR then no pressure drop

	if (ms_des_par.m_DP_PHX[0] < 0.0)
		m_pres_last[TURB_IN] = m_pres_last[HTR_HP_OUT] * (1.0 - fabs(ms_des_par.m_DP_PHX[0]));	//[kPa]
	else
		m_pres_last[TURB_IN] = m_pres_last[HTR_HP_OUT] - ms_des_par.m_DP_PHX[0];	//[kPa]

	if (ms_des_par.m_DP_PC_partial[1] < 0.0)
		m_pres_last[PC_OUT] = m_pres_last[MC_IN] / (1.0 - fabs(ms_des_par.m_DP_PC_partial[1]));	//[kPa]
	else
		m_pres_last[PC_OUT] = m_pres_last[MC_IN] + ms_des_par.m_DP_PC_partial[1];	//[kPa]

	if (ms_des_par.m_DP_PC_full[1] < 0.0)
		m_pres_last[LTR_LP_OUT] = m_pres_last[PC_IN] / (1.0 - fabs(ms_des_par.m_DP_PC_partial[1]));	//[kPa]
	else
		m_pres_last[LTR_LP_OUT] = m_pres_last[PC_IN] + ms_des_par.m_DP_PC_partial[1];	//[kPa]

	if (ms_des_par.m_DP_LTR[1] < 0.0)
		m_pres_last[HTR_LP_OUT] = m_pres_last[LTR_LP_OUT] / (1.0 - fabs(ms_des_par.m_DP_LTR[1]));	//[kPa]
	else
		m_pres_last[HTR_LP_OUT] = m_pres_last[LTR_LP_OUT] + ms_des_par.m_DP_LTR[1];		//[kPa]

	if (ms_des_par.m_UA_LTR < 1.0E-12)
		m_pres_last[HTR_LP_OUT] = m_pres_last[LTR_LP_OUT];	//[kPa] if no LTR then no pressure drop

	if (ms_des_par.m_DP_HTR[1] < 0.0)
		m_pres_last[TURB_OUT] = m_pres_last[HTR_LP_OUT] / (1.0 - fabs(ms_des_par.m_DP_HTR[1]));	//[kPa]
	else
		m_pres_last[TURB_OUT] = m_pres_last[HTR_LP_OUT] + ms_des_par.m_DP_HTR[1];	//[kPa]

	if (ms_des_par.m_UA_HTR < 1.0E-12)
		m_pres_last[TURB_OUT] = m_pres_last[HTR_LP_OUT];

	// Calculate equivalent isentropic efficiencies for turbomachinery, if necessary
	double eta_mc_isen = ms_des_par.m_eta_mc;		//[-]
	if (ms_des_par.m_eta_mc < 0.0)
	{
		int poly_error_code = 0;

		isen_eta_from_poly_eta(m_temp_last[MC_IN], m_pres_last[MC_IN], m_pres_last[MC_OUT], fabs(ms_des_par.m_eta_mc),
			true, poly_error_code, eta_mc_isen);

		if (poly_error_code != 0)
			return poly_error_code;
	}

	double eta_rc_isen = ms_des_par.m_eta_rc;		//[-]
	if (ms_des_par.m_eta_rc < 0.0)
	{
		int poly_error_code = 0;

		isen_eta_from_poly_eta(m_temp_last[PC_OUT], m_pres_last[PC_OUT], m_pres_last[RC_OUT], fabs(ms_des_par.m_eta_rc),
			true, poly_error_code, eta_rc_isen);

		if (poly_error_code != 0)
			return poly_error_code;
	}

	double eta_pc_isen = ms_des_par.m_eta_pc;		//[-]
	if (ms_des_par.m_eta_pc < 0.0)
	{
		int poly_error_code = 0;

		isen_eta_from_poly_eta(m_temp_last[PC_IN], m_pres_last[PC_IN], m_pres_last[PC_OUT], fabs(ms_des_par.m_eta_pc),
			true, poly_error_code, eta_pc_isen);

		if (poly_error_code != 0)
			return poly_error_code;
	}

	double eta_t_isen = ms_des_par.m_eta_t;		//[-]
	if (ms_des_par.m_eta_t < 0.0)
	{
		int poly_error_code = 0;

		isen_eta_from_poly_eta(m_temp_last[TURB_IN], m_pres_last[TURB_IN], m_pres_last[TURB_OUT], fabs(ms_des_par.m_eta_t),
			false, poly_error_code, eta_t_isen);

		if (poly_error_code != 0)
			return poly_error_code;
	}

	// Determine the outlet state and specific work for the turbomachinery
	int comp_error_code = 0;
	double w_mc = std::numeric_limits<double>::quiet_NaN();
	calculate_turbomachinery_outlet_1(m_temp_last[MC_IN], m_pres_last[MC_IN], m_pres_last[MC_OUT], eta_mc_isen, true,
		comp_error_code, m_enth_last[MC_IN], m_entr_last[MC_IN], m_dens_last[MC_IN], m_temp_last[MC_OUT],
		m_enth_last[MC_OUT], m_entr_last[MC_OUT], m_dens_last[MC_OUT], w_mc);

	if (comp_error_code != 0)
		return comp_error_code;

	double w_pc = std::numeric_limits<double>::quiet_NaN();
	calculate_turbomachinery_outlet_1(m_temp_last[PC_IN], m_pres_last[PC_IN], m_pres_last[PC_OUT], eta_pc_isen, true,
		comp_error_code, m_enth_last[PC_IN], m_entr_last[PC_IN], m_dens_last[PC_IN], m_temp_last[PC_OUT],
		m_enth_last[PC_OUT], m_entr_last[PC_OUT], m_dens_last[PC_OUT], w_pc);

	if (comp_error_code != 0)
		return comp_error_code;

	double w_rc = 0.0;
	if (ms_des_par.m_recomp_frac >= 1.E-12)
	{
		calculate_turbomachinery_outlet_1(m_temp_last[PC_OUT], m_pres_last[PC_OUT], m_pres_last[RC_OUT], eta_rc_isen, true,
			comp_error_code, m_enth_last[PC_OUT], m_entr_last[PC_OUT], m_dens_last[PC_OUT], m_temp_last[RC_OUT],
			m_enth_last[RC_OUT], m_entr_last[RC_OUT], m_dens_last[RC_OUT], w_rc);

		if (comp_error_code != 0)
			return comp_error_code;
	}

	double w_t = std::numeric_limits<double>::quiet_NaN();
	calculate_turbomachinery_outlet_1(m_temp_last[TURB_IN], m_pres_last[TURB_IN], m_pres_last[TURB_OUT], eta_t_isen, false,
		comp_error_code, m_enth_last[TURB_IN], m_entr_last[TURB_IN], m_dens_last[TURB_IN], m_temp_last[TURB_OUT],
		m_enth_last[TURB_OUT], m_entr_last[TURB_OUT], m_dens_last[TURB_OUT], w_t);

	if (comp_error_code != 0)
		return comp_error_code;

	// know all turbomachinery specific work, so can calculate mass flow rate required to hit target power
	m_m_dot_t = ms_des_par.m_W_dot_net / (w_t + w_pc + ms_des_par.m_recomp_frac*w_rc + (1.0 - ms_des_par.m_recomp_frac)*w_mc);	//[kg/s]
	
	if (m_m_dot_t <= 0.0 || !std::isfinite(m_m_dot_t))	// positive net power is impossible; return an error
		return 25;

	m_m_dot_pc = m_m_dot_t;	//[kg/s]
	m_m_dot_rc = m_m_dot_t * ms_des_par.m_recomp_frac;	//[kg/s]
	m_m_dot_mc = m_m_dot_t *(1.0 - ms_des_par.m_recomp_frac);	//[kg/s]

	// Solve the recuperator performance
	double T_HTR_LP_out_lower = m_temp_last[MC_OUT];		//[K] Coldest possible temperature
	double T_HTR_LP_out_upper = m_temp_last[TURB_OUT];		//[K] Hottest possible temperature

	double T_HTR_LP_out_guess_lower = std::min(T_HTR_LP_out_upper - 2.0, std::max(T_HTR_LP_out_lower+15.0, 220.0 + 273.15));	//[K] There is nothing magic about 15
	double T_HTR_LP_out_guess_upper = std::min(T_HTR_LP_out_guess_lower + 20.0, T_HTR_LP_out_upper - 1.0);		//[K] There is nothing magic about 20

	C_MEQ_HTR_des HTR_des_eq(this);
	C_monotonic_eq_solver HTR_des_solver(HTR_des_eq);

	HTR_des_solver.settings(ms_des_par.m_tol*m_temp_last[MC_IN], 1000, T_HTR_LP_out_lower, T_HTR_LP_out_upper, false);

	double T_HTR_LP_out_solved, tol_T_HTR_LP_out_solved;
	T_HTR_LP_out_solved = tol_T_HTR_LP_out_solved = std::numeric_limits<double>::quiet_NaN();
	int iter_T_HTR_LP_out = -1;

	int T_HTR_LP_out_code = HTR_des_solver.solve(T_HTR_LP_out_guess_lower, T_HTR_LP_out_guess_upper, 0,
								T_HTR_LP_out_solved, tol_T_HTR_LP_out_solved, iter_T_HTR_LP_out);

	if (T_HTR_LP_out_code != C_monotonic_eq_solver::CONVERGED)
	{
		return 35;
	}

	double Q_dot_HTR = HTR_des_eq.m_Q_dot_HTR;		//[kWt]

	// Can now define HTR HP outlet state
	m_enth_last[HTR_HP_OUT] = m_enth_last[MIXER_OUT] + Q_dot_HTR / m_m_dot_t;		//[kJ/kg]
	int prop_error_code = CO2_PH(m_pres_last[HTR_HP_OUT], m_enth_last[HTR_HP_OUT], &mc_co2_props);
	if (prop_error_code != 0)
	{
		return prop_error_code;
	}
	m_temp_last[HTR_HP_OUT] = mc_co2_props.temp;	//[K]
	m_entr_last[HTR_HP_OUT] = mc_co2_props.entr;	//[kJ/kg-K]
	m_dens_last[HTR_HP_OUT] = mc_co2_props.dens;	//[kg/m^3]

	// Set design values for PHX and coolers
	C_HeatExchanger::S_design_parameters PHX_des_par;
	PHX_des_par.m_DP_design[0] = m_pres_last[HTR_HP_OUT] - m_pres_last[TURB_IN];		//[kPa]
	PHX_des_par.m_DP_design[1] = 0.0;
	PHX_des_par.m_m_dot_design[0] = m_m_dot_t;		//[kg/s]
	PHX_des_par.m_m_dot_design[1] = 0.0;			//[kg/s]
	PHX_des_par.m_Q_dot_design = m_m_dot_t*(m_enth_last[TURB_IN] - m_enth_last[HTR_HP_OUT]);	//[kWt]
	mc_PHX.initialize(PHX_des_par);

	C_HeatExchanger::S_design_parameters PC_full_des_par;
	PC_full_des_par.m_DP_design[0] = 0.0;
	PC_full_des_par.m_DP_design[1] = m_pres_last[LTR_LP_OUT] - m_pres_last[PC_IN];	//[kPa]
	PC_full_des_par.m_m_dot_design[0] = 0.0;		//[kg/s]
	PC_full_des_par.m_m_dot_design[1] = m_m_dot_pc;	//[kg/s]
	PC_full_des_par.m_Q_dot_design = m_m_dot_pc*(m_enth_last[LTR_LP_OUT] - m_enth_last[PC_IN]);	//[kWt]
	mc_cooler_pc.initialize(PC_full_des_par);

	C_HeatExchanger::S_design_parameters PC_partial_des_par;
	PC_partial_des_par.m_DP_design[0] = 0.0;
	PC_partial_des_par.m_DP_design[1] = m_pres_last[PC_OUT] - m_pres_last[MC_IN];	//[kPa]
	PC_partial_des_par.m_m_dot_design[0] = 0.0;
	PC_partial_des_par.m_m_dot_design[1] = m_m_dot_mc;	//[kg/s]
	PC_partial_des_par.m_Q_dot_design = m_m_dot_mc*(m_enth_last[PC_OUT] - m_enth_last[MC_IN]);	//[kWt]
	mc_cooler_mc.initialize(PC_partial_des_par);

	// Calculate and set cycle performance metrics
	m_W_dot_t = m_m_dot_t*w_t;		//[kWe]
	m_W_dot_pc = m_m_dot_pc*w_pc;	//[kWe]
	m_W_dot_rc = m_m_dot_rc*w_rc;	//[kWe]
	m_W_dot_mc = m_m_dot_mc*w_mc;	//[kWe]
	m_W_dot_net_last = m_m_dot_t*w_t + m_m_dot_pc*w_pc + m_m_dot_rc*w_rc + m_m_dot_mc*w_mc;		//[kWe]
	m_eta_thermal_calc_last = m_W_dot_net_last / PHX_des_par.m_Q_dot_design;	//[-]
	m_energy_bal_last = (PHX_des_par.m_Q_dot_design - m_W_dot_net_last - PC_partial_des_par.m_Q_dot_design - PC_full_des_par.m_Q_dot_design) / PHX_des_par.m_Q_dot_design;	//[-]

	if (ms_des_par.m_des_objective_type == 2)
	{
		double phx_deltaT = m_temp_last[TURB_IN] - m_temp_last[HTR_HP_OUT];
		double under_min_deltaT = std::max(0.0, ms_des_par.m_min_phx_deltaT - phx_deltaT);
		double eta_deltaT_scale = std::exp(-under_min_deltaT);
		m_objective_metric_last = m_eta_thermal_calc_last * eta_deltaT_scale;
	}
	else
	{
		m_objective_metric_last = m_eta_thermal_calc_last;
	}

	return 0;
}

int C_PartialCooling_Cycle::C_MEQ_HTR_des::operator()(double T_HTR_LP_out /*K*/, double *diff_T_HTR_LP_out /*K*/)
{
	m_Q_dot_LTR = m_Q_dot_HTR = std::numeric_limits<double>::quiet_NaN();

	mpc_pc_cycle->m_temp_last[HTR_LP_OUT] = T_HTR_LP_out;	//[K]

	int prop_error_code = CO2_TP(mpc_pc_cycle->m_temp_last[HTR_LP_OUT], mpc_pc_cycle->m_pres_last[HTR_LP_OUT], &mpc_pc_cycle->mc_co2_props);
	if (prop_error_code != 0)
	{
		*diff_T_HTR_LP_out = std::numeric_limits<double>::quiet_NaN();
		return prop_error_code;
	}
	mpc_pc_cycle->m_enth_last[HTR_LP_OUT] = mpc_pc_cycle->mc_co2_props.enth;	//[kJ/kg]
	mpc_pc_cycle->m_entr_last[HTR_LP_OUT] = mpc_pc_cycle->mc_co2_props.entr;	//[kJ/kg-K]
	mpc_pc_cycle->m_dens_last[HTR_LP_OUT] = mpc_pc_cycle->mc_co2_props.dens;	//[kg/m^3]

	try
	{
		mpc_pc_cycle->mc_LTR.design_fix_UA_calc_outlet(mpc_pc_cycle->ms_des_par.m_UA_LTR, mpc_pc_cycle->ms_des_par.m_LTR_eff_max,
			mpc_pc_cycle->m_temp_last[MC_OUT], mpc_pc_cycle->m_pres_last[MC_OUT], mpc_pc_cycle->m_m_dot_mc, mpc_pc_cycle->m_pres_last[LTR_HP_OUT],
			mpc_pc_cycle->m_temp_last[HTR_LP_OUT], mpc_pc_cycle->m_pres_last[HTR_LP_OUT], mpc_pc_cycle->m_m_dot_t, mpc_pc_cycle->m_pres_last[LTR_LP_OUT],
			m_Q_dot_LTR, mpc_pc_cycle->m_temp_last[LTR_HP_OUT], mpc_pc_cycle->m_temp_last[LTR_LP_OUT]);
	}
	catch (C_csp_exception &)
	{
		*diff_T_HTR_LP_out = std::numeric_limits<double>::quiet_NaN();
		return -2;
	}

	prop_error_code = CO2_TP(mpc_pc_cycle->m_temp_last[LTR_LP_OUT], mpc_pc_cycle->m_pres_last[LTR_LP_OUT], &mpc_pc_cycle->mc_co2_props);
	if (prop_error_code)
	{
		*diff_T_HTR_LP_out = std::numeric_limits<double>::quiet_NaN();
		return prop_error_code;
	}
	mpc_pc_cycle->m_enth_last[LTR_LP_OUT] = mpc_pc_cycle->mc_co2_props.enth;	//[kJ/kg]
	mpc_pc_cycle->m_entr_last[LTR_LP_OUT] = mpc_pc_cycle->mc_co2_props.entr;	//[kJ/kg-K]
	mpc_pc_cycle->m_dens_last[LTR_LP_OUT] = mpc_pc_cycle->mc_co2_props.dens;	//[kg/m^3]

	// *****************************************************************************
		// Energy balance on the LTR HP stream
	mpc_pc_cycle->m_enth_last[LTR_HP_OUT] = mpc_pc_cycle->m_enth_last[MC_OUT] + m_Q_dot_LTR / mpc_pc_cycle->m_m_dot_mc;	//[kJ/kg]
	prop_error_code = CO2_PH(mpc_pc_cycle->m_pres_last[LTR_HP_OUT], mpc_pc_cycle->m_enth_last[LTR_HP_OUT], &mpc_pc_cycle->mc_co2_props);
	if (prop_error_code != 0)
	{
		*diff_T_HTR_LP_out = std::numeric_limits<double>::quiet_NaN();
		return prop_error_code;
	}
	mpc_pc_cycle->m_temp_last[LTR_HP_OUT] = mpc_pc_cycle->mc_co2_props.temp;	//[K]
	mpc_pc_cycle->m_entr_last[LTR_HP_OUT] = mpc_pc_cycle->mc_co2_props.entr;	//[kJ/kg-K]
	mpc_pc_cycle->m_dens_last[LTR_HP_OUT] = mpc_pc_cycle->mc_co2_props.dens;	//[kg/m^3]

	// Solve enthalpy balance for the mixer
	if (mpc_pc_cycle->ms_des_par.m_recomp_frac >= 1.E-12)
	{
		mpc_pc_cycle->m_enth_last[MIXER_OUT] = (1.0 - mpc_pc_cycle->ms_des_par.m_recomp_frac)*mpc_pc_cycle->m_enth_last[LTR_HP_OUT] + mpc_pc_cycle->ms_des_par.m_recomp_frac*mpc_pc_cycle->m_enth_last[RC_OUT];	//[kJ/kg]
		prop_error_code = CO2_PH(mpc_pc_cycle->m_pres_last[MIXER_OUT], mpc_pc_cycle->m_enth_last[MIXER_OUT], &mpc_pc_cycle->mc_co2_props);
		if (prop_error_code != 0)
		{
			*diff_T_HTR_LP_out = std::numeric_limits<double>::quiet_NaN();
			return prop_error_code;
		}
		mpc_pc_cycle->m_temp_last[MIXER_OUT] = mpc_pc_cycle->mc_co2_props.temp;		//[K]
		mpc_pc_cycle->m_entr_last[MIXER_OUT] = mpc_pc_cycle->mc_co2_props.entr;		//[kJ/kg-K]
		mpc_pc_cycle->m_dens_last[MIXER_OUT] = mpc_pc_cycle->mc_co2_props.dens;		//[kg/m^3]
	}
	else
	{
		mpc_pc_cycle->m_temp_last[MIXER_OUT] = mpc_pc_cycle->m_temp_last[LTR_HP_OUT];	//[K]
		mpc_pc_cycle->m_enth_last[MIXER_OUT] = mpc_pc_cycle->m_enth_last[LTR_HP_OUT];	//[kJ/kg]
		mpc_pc_cycle->m_entr_last[MIXER_OUT] = mpc_pc_cycle->m_entr_last[LTR_HP_OUT];	//[kJ/kg-K]
		mpc_pc_cycle->m_dens_last[MIXER_OUT] = mpc_pc_cycle->m_dens_last[LTR_HP_OUT];	//[kg/m^3]
	}

	// Calculate the HTR design performance
	double T_HTR_LP_out_calc = std::numeric_limits<double>::quiet_NaN();

	try
	{
		mpc_pc_cycle->mc_HTR.design_fix_UA_calc_outlet(mpc_pc_cycle->ms_des_par.m_UA_HTR, mpc_pc_cycle->ms_des_par.m_HTR_eff_max,
			mpc_pc_cycle->m_temp_last[MIXER_OUT], mpc_pc_cycle->m_pres_last[MIXER_OUT], mpc_pc_cycle->m_m_dot_t, mpc_pc_cycle->m_pres_last[HTR_HP_OUT],
			mpc_pc_cycle->m_temp_last[TURB_OUT], mpc_pc_cycle->m_pres_last[TURB_OUT], mpc_pc_cycle->m_m_dot_t, mpc_pc_cycle->m_pres_last[HTR_LP_OUT],
			m_Q_dot_HTR, mpc_pc_cycle->m_temp_last[HTR_HP_OUT], T_HTR_LP_out_calc);
	}
	catch (C_csp_exception &)
	{
		*diff_T_HTR_LP_out = std::numeric_limits<double>::quiet_NaN();
		return -1;
	}

	*diff_T_HTR_LP_out = T_HTR_LP_out_calc - mpc_pc_cycle->m_temp_last[HTR_LP_OUT];		//[K]

	return 0;

}

int C_PartialCooling_Cycle::C_MEQ_LTR_des::operator()(double T_LTR_LP_out /*K*/, double *diff_T_LTR_LP_out /*K*/)
{
	m_Q_dot_LTR = std::numeric_limits<double>::quiet_NaN();		//[kWt]
	
	mpc_pc_cycle->m_temp_last[LTR_LP_OUT] = T_LTR_LP_out;		//[K]

	int prop_error_code = CO2_TP(mpc_pc_cycle->m_temp_last[LTR_LP_OUT], mpc_pc_cycle->m_pres_last[LTR_LP_OUT], &mpc_pc_cycle->mc_co2_props);
	if (prop_error_code)
	{
		*diff_T_LTR_LP_out = std::numeric_limits<double>::quiet_NaN();
		return prop_error_code;
	}
	mpc_pc_cycle->m_enth_last[LTR_LP_OUT] = mpc_pc_cycle->mc_co2_props.enth;	//[kJ/kg]
	mpc_pc_cycle->m_entr_last[LTR_LP_OUT] = mpc_pc_cycle->mc_co2_props.entr;	//[kJ/kg-K]
	mpc_pc_cycle->m_dens_last[LTR_LP_OUT] = mpc_pc_cycle->mc_co2_props.dens;	//[kg/m^3]

	double T_LTR_LP_out_calc = std::numeric_limits<double>::quiet_NaN();

	try
	{
		mpc_pc_cycle->mc_LTR.design_fix_UA_calc_outlet(mpc_pc_cycle->ms_des_par.m_UA_LTR, mpc_pc_cycle->ms_des_par.m_LTR_eff_max,
			mpc_pc_cycle->m_temp_last[MC_OUT], mpc_pc_cycle->m_pres_last[MC_OUT], mpc_pc_cycle->m_m_dot_mc, mpc_pc_cycle->m_pres_last[LTR_HP_OUT],
			mpc_pc_cycle->m_temp_last[HTR_LP_OUT], mpc_pc_cycle->m_pres_last[HTR_LP_OUT], mpc_pc_cycle->m_m_dot_t, mpc_pc_cycle->m_pres_last[LTR_LP_OUT],
			m_Q_dot_LTR, mpc_pc_cycle->m_temp_last[LTR_HP_OUT], T_LTR_LP_out_calc);
	}
	catch (C_csp_exception &)
	{
		*diff_T_LTR_LP_out = std::numeric_limits<double>::quiet_NaN();

		return -1;
	}

	*diff_T_LTR_LP_out = T_LTR_LP_out_calc - mpc_pc_cycle->m_temp_last[LTR_LP_OUT];

	return 0;
}

double C_PartialCooling_Cycle::opt_eta_fixed_P_high(double P_high_opt /*kPa*/)
{
	// Complete 'ms_opt_des_par'
	ms_opt_des_par.m_P_mc_out_guess = P_high_opt;	//[kPa]
	ms_opt_des_par.m_fixed_P_mc_out = true;

	ms_opt_des_par.m_fixed_PR_total = false;
	ms_opt_des_par.m_PR_total_guess = 25. / 6.5;	//[-] Guess could be improved...

	ms_opt_des_par.m_fixed_f_PR_mc = false;
	ms_opt_des_par.m_f_PR_mc_guess = (25. - 8.5) / (25. - 6.5);		//[-] Guess could be improved...

	ms_opt_des_par.m_recomp_frac_guess = 0.25;	//[-]
	ms_opt_des_par.m_fixed_recomp_frac = false;

	ms_opt_des_par.m_LTR_frac_guess = 0.5;		//[-]
	ms_opt_des_par.m_fixed_LTR_frac = false;

	int pc_error_code = opt_design_core();

	double local_objective_metric = 0.0;
	if (pc_error_code == 0)
		local_objective_metric = m_objective_metric_opt;

	if (pc_error_code == 0 && m_objective_metric_opt > m_objective_metric_auto_opt)
	{
		ms_des_par_auto_opt = ms_des_par_optimal;
		m_objective_metric_auto_opt = m_objective_metric_opt;
	}

	return -local_objective_metric;
}

int C_PartialCooling_Cycle::finalize_design()
{
	int mc_des_err = mc_mc.design_given_outlet_state(m_temp_last[MC_IN],
										m_pres_last[MC_IN],
										m_m_dot_mc,
										m_temp_last[MC_OUT],
										m_pres_last[MC_OUT]);

	if (mc_des_err != 0)
	{
		return 71;
	}

	int pc_des_err = mc_pc.design_given_outlet_state(m_temp_last[PC_IN],
										m_pres_last[PC_IN],
										m_m_dot_pc,
										m_temp_last[PC_OUT],
										m_pres_last[PC_OUT]);

	if (pc_des_err != 0)
	{
		return 72;
	}

	if (ms_des_par.m_recomp_frac > 0.01)
	{
		int rc_des_err = mc_rc.design_given_outlet_state(m_temp_last[PC_OUT],
										m_pres_last[PC_OUT],
										m_m_dot_rc,
										m_temp_last[RC_OUT],
										m_pres_last[RC_OUT]);

		if (rc_des_err != 0)
		{
			return 73;
		}

		ms_des_solved.m_is_rc = true;
	}
	else
		ms_des_solved.m_is_rc = false;

	C_turbine::S_design_parameters t_des_par;
		// Set turbine shaft speed
	t_des_par.m_N_design = ms_des_par.m_N_turbine;		//[rpm]
	t_des_par.m_N_comp_design_if_linked = mc_mc.get_design_solved()->m_N_design;	//[rpm]
		// Turbine inlet state
	t_des_par.m_P_in = m_pres_last[TURB_IN];	//[kPa]
	t_des_par.m_T_in = m_temp_last[TURB_IN];	//[K]
	t_des_par.m_D_in = m_dens_last[TURB_IN];	//[kg/m^3]
	t_des_par.m_h_in = m_enth_last[TURB_IN];	//[kJ/kg]
	t_des_par.m_s_in = m_entr_last[TURB_IN];	//[kJ/kg-K]
		// Turbine outlet state
	t_des_par.m_P_out = m_pres_last[TURB_OUT];	//[kPa]
	t_des_par.m_h_out = m_enth_last[TURB_OUT];	//[kJ/kg]
		// Mass flow
	t_des_par.m_m_dot = m_m_dot_t;		//[kg/s]

	int turb_size_err = 0;
	mc_t.turbine_sizing(t_des_par, turb_size_err);
	if (turb_size_err != 0)
	{
		return 74;
	}

	// Get 'design_solved' structures from component classes
	ms_des_solved.ms_mc_ms_des_solved = *mc_mc.get_design_solved();
	ms_des_solved.ms_rc_ms_des_solved = *mc_rc.get_design_solved();
	ms_des_solved.ms_pc_ms_des_solved = *mc_pc.get_design_solved();
	ms_des_solved.ms_t_des_solved = *mc_t.get_design_solved();
	ms_des_solved.ms_LTR_des_solved = mc_LTR.ms_des_solved;
	ms_des_solved.ms_HTR_des_solved = mc_HTR.ms_des_solved;

	// Set solved design point metrics
	ms_des_solved.m_temp = m_temp_last;
	ms_des_solved.m_pres = m_pres_last;
	ms_des_solved.m_enth = m_enth_last;
	ms_des_solved.m_entr = m_entr_last;
	ms_des_solved.m_dens = m_dens_last;

	ms_des_solved.m_eta_thermal = m_eta_thermal_calc_last;
	ms_des_solved.m_W_dot_net = m_W_dot_net_last;
	ms_des_solved.m_m_dot_mc = m_m_dot_mc;
	ms_des_solved.m_m_dot_rc = m_m_dot_rc;
	ms_des_solved.m_m_dot_pc = m_m_dot_pc;
	ms_des_solved.m_m_dot_t = m_m_dot_t;
	ms_des_solved.m_recomp_frac = m_m_dot_rc / m_m_dot_t;

	ms_des_solved.m_UA_LTR = ms_des_par.m_UA_LTR;
	ms_des_solved.m_UA_HTR = ms_des_par.m_UA_HTR;

	ms_des_solved.m_W_dot_t = m_W_dot_t;     //[kWe]
	ms_des_solved.m_W_dot_mc = m_W_dot_mc;	 //[kWe]
	ms_des_solved.m_W_dot_rc = m_W_dot_rc;	 //[kWe]
	ms_des_solved.m_W_dot_pc = m_W_dot_pc;	 //[kWe]

	return 0;
}

double C_PartialCooling_Cycle::design_cycle_return_objective_metric(const std::vector<double> &x)
{
	int index = 0;

	// Main compressor outlet pressure
	if (!ms_opt_des_par.m_fixed_P_mc_out)
	{
		ms_des_par.m_P_mc_out = x[index];
		if (ms_des_par.m_P_mc_out > ms_opt_des_par.m_P_high_limit)
			return 0.0;
		index++;
	}
	else
		ms_des_par.m_P_mc_out = ms_opt_des_par.m_P_mc_out_guess;	//[kPa]

	// Total pressure ratio
	double PR_total_local = -999.9;
	double P_pc_in = -999.9;
	if (!ms_opt_des_par.m_fixed_PR_total)
	{
		PR_total_local = x[index];
		if (PR_total_local > 50.0)
			return 0.0;
		index++;
		P_pc_in = ms_des_par.m_P_mc_out / PR_total_local;		//[kPa]
	}
	else
	{
		if (ms_opt_des_par.m_PR_total_guess >= 0.0)
		{
			PR_total_local = ms_opt_des_par.m_PR_total_guess;
			P_pc_in = ms_des_par.m_P_mc_in / PR_total_local;	//[kPa]
		}
		else
		{
			P_pc_in = fabs(ms_opt_des_par.m_PR_total_guess);	//[kPa]
		}
	}

	if (P_pc_in >= ms_des_par.m_P_mc_out)
		return 0.0;
	if (P_pc_in <= 100.0)
		return 0.0;
	ms_des_par.m_P_pc_in = P_pc_in;		//[kPa]

	// Main compressor inlet pressure
	double P_mc_in = -999.9;
	if (!ms_opt_des_par.m_fixed_f_PR_mc)
	{
		P_mc_in = ms_des_par.m_P_mc_out - x[index] * (ms_des_par.m_P_mc_out - ms_des_par.m_P_pc_in);	//[kPa]
		index++;
	}
	else
	{
		P_mc_in = ms_des_par.m_P_mc_out - ms_opt_des_par.m_fixed_f_PR_mc*(ms_des_par.m_P_mc_out - ms_des_par.m_P_pc_in);	//[kPa]
	}
	ms_des_par.m_P_mc_in = P_mc_in;		//[kPa]

	// Recompression fraction
	if (!ms_opt_des_par.m_fixed_recomp_frac)
	{
		ms_des_par.m_recomp_frac = x[index];		//[-]
		if (ms_des_par.m_recomp_frac < 0.0)
			return 0.0;
		index++;
	}
	else
		ms_des_par.m_recomp_frac = ms_opt_des_par.m_recomp_frac_guess;	//[-]

	// Recuperator split fraction
	double LTR_frac_local = -999.9;
	if (!ms_opt_des_par.m_fixed_LTR_frac)
	{
		LTR_frac_local = x[index];								//[-]
		if (LTR_frac_local > 1.0 || LTR_frac_local < 0.0)
			return 0.0;
		index++;
	}
	else
		LTR_frac_local = ms_opt_des_par.m_LTR_frac_guess;		//[-]

	ms_des_par.m_UA_LTR = ms_opt_des_par.m_UA_rec_total*LTR_frac_local;			//[kW/K]
	ms_des_par.m_UA_HTR = ms_opt_des_par.m_UA_rec_total*(1.0 - LTR_frac_local);	//[kW/K]

	int des_err_code = design_core();

	double objective_metric = 0.0;
	if (des_err_code == 0)
	{
		objective_metric = m_objective_metric_last;

		if (m_objective_metric_last > m_objective_metric_opt)
		{
			ms_des_par_optimal = ms_des_par;
			m_objective_metric_opt = m_objective_metric_last;
		}
	}

	return objective_metric;
}

int C_PartialCooling_Cycle::opt_design_core()
{
	// Map ms_opt_des_par to ms_des_par
	ms_des_par.m_W_dot_net = ms_opt_des_par.m_W_dot_net;	//[kWe]
	ms_des_par.m_T_mc_in = ms_opt_des_par.m_T_mc_in;		//[K]
	ms_des_par.m_T_pc_in = ms_opt_des_par.m_T_pc_in;		//[K]
	ms_des_par.m_T_t_in = ms_opt_des_par.m_T_t_in;			//[K]
	ms_des_par.m_DP_LTR = ms_opt_des_par.m_DP_LTR;			//
	ms_des_par.m_DP_HTR = ms_opt_des_par.m_DP_HTR;			//
	ms_des_par.m_DP_PC_full = ms_opt_des_par.m_DP_PC_full;	//
	ms_des_par.m_DP_PC_partial = ms_opt_des_par.m_DP_PC_partial;	//
	ms_des_par.m_DP_PHX = ms_opt_des_par.m_DP_PHX;			//
	ms_des_par.m_LTR_eff_max = ms_opt_des_par.m_LTR_eff_max;	//[-]
	ms_des_par.m_HTR_eff_max = ms_opt_des_par.m_HTR_eff_max;	//[-]
	ms_des_par.m_eta_mc = ms_opt_des_par.m_eta_mc;			//[-]
	ms_des_par.m_eta_rc = ms_opt_des_par.m_eta_rc;			//[-]
	ms_des_par.m_eta_pc = ms_opt_des_par.m_eta_pc;			//[-]
	ms_des_par.m_eta_t = ms_opt_des_par.m_eta_t;			//[-]
	ms_des_par.m_N_sub_hxrs = ms_opt_des_par.m_N_sub_hxrs;	//[-]
	ms_des_par.m_P_high_limit = ms_opt_des_par.m_P_high_limit;	//[kPa]
	ms_des_par.m_tol = ms_opt_des_par.m_tol;				//[-]
	ms_des_par.m_N_turbine = ms_opt_des_par.m_N_turbine;	//[rpm]
	ms_des_par.m_des_objective_type = ms_opt_des_par.m_des_objective_type;	//[-]
	ms_des_par.m_min_phx_deltaT = ms_opt_des_par.m_min_phx_deltaT;			//[K]

	int index = 0;

	std::vector<double> x(0);
	std::vector<double> lb(0);
	std::vector<double> ub(0);
	std::vector<double> scale(0);

	if (!ms_opt_des_par.m_fixed_P_mc_out)
	{
		x.push_back(ms_opt_des_par.m_P_mc_out_guess);	//[kPa]
		lb.push_back(1.E3);								//[kPa]
		ub.push_back(ms_opt_des_par.m_P_high_limit);	//[kPa]
		scale.push_back(500.0);							//[kPa]

		index++;
	}

	if (!ms_opt_des_par.m_fixed_PR_total)
	{
		x.push_back(ms_opt_des_par.m_PR_total_guess);	//[-]
		lb.push_back(1.E-3);			//[-]
		ub.push_back(50);				//[-]
		scale.push_back(0.45);			//[-]

		index++;
	}

	if (!ms_opt_des_par.m_fixed_f_PR_mc)
	{
		x.push_back(ms_opt_des_par.m_f_PR_mc_guess);	//[-]
		lb.push_back(1.E-3);			//[-]
		ub.push_back(0.999);			//[-]
		scale.push_back(0.2);			//[-]

		index++;
	}

	if (!ms_opt_des_par.m_fixed_recomp_frac)
	{
		x.push_back(ms_opt_des_par.m_recomp_frac_guess);	//[-]
		lb.push_back(0.0);			//[-]
		ub.push_back(1.0);			//[-]
		scale.push_back(0.05);		//[-]

		index++;
	}

	if (!ms_opt_des_par.m_fixed_LTR_frac)
	{
		x.push_back(ms_opt_des_par.m_LTR_frac_guess);		//[-]
		lb.push_back(0.0);			//[-]
		ub.push_back(1.0);			//[-]
		scale.push_back(0.1);		//[-]

		index++;
	}

	int no_opt_err_code = 0;
	if (index > 0)
	{
		m_objective_metric_opt = 0.0;

		// Set up instance of nlopt class and set optimization parameters
		nlopt::opt     opt_des_cycle(nlopt::LN_SBPLX, index);
		opt_des_cycle.set_lower_bounds(lb);
		opt_des_cycle.set_upper_bounds(ub);
		opt_des_cycle.set_initial_step(scale);
		opt_des_cycle.set_xtol_rel(ms_opt_des_par.m_opt_tol);

		// set max objective function
		opt_des_cycle.set_max_objective(nlopt_cb_opt_partialcooling_des, this);
		double max_f = std::numeric_limits<double>::quiet_NaN();
		nlopt::result  result_des_cycle = opt_des_cycle.optimize(x, max_f);

		ms_des_par = ms_des_par_optimal;

		no_opt_err_code = design_core();

	}
	else
	{
		ms_des_par.m_P_mc_out = ms_opt_des_par.m_P_mc_out_guess;							//[kPa]
		ms_des_par.m_P_pc_in = ms_des_par.m_P_mc_out / ms_opt_des_par.m_PR_total_guess;		//[kPa]
		ms_des_par.m_P_mc_in = ms_des_par.m_P_mc_out - ms_opt_des_par.m_f_PR_mc_guess*(ms_des_par.m_P_mc_out - ms_des_par.m_P_pc_in);	//[kPa]
		ms_des_par.m_recomp_frac = ms_opt_des_par.m_recomp_frac_guess;	//[-]
		ms_des_par.m_UA_LTR = ms_opt_des_par.m_UA_rec_total*ms_opt_des_par.m_LTR_frac_guess;	//[kW/K]
		ms_des_par.m_UA_HTR = ms_opt_des_par.m_UA_rec_total*ms_opt_des_par.m_LTR_frac_guess;	//[kW/K]

		no_opt_err_code = design_core();

		ms_des_par_optimal = ms_des_par;
	}

	return no_opt_err_code;
}

int C_PartialCooling_Cycle::opt_design(S_opt_des_params & opt_des_par_in)
{
	ms_opt_des_par = opt_des_par_in;

	int opt_des_err_code = opt_design_core();

	if (opt_des_err_code != 0)
		return opt_des_err_code;

	return finalize_design();
}

int C_PartialCooling_Cycle::auto_opt_design(S_auto_opt_design_parameters & auto_opt_des_par_in)
{
	ms_auto_opt_des_par = auto_opt_des_par_in;

	return auto_opt_design_core();
}

int C_PartialCooling_Cycle::auto_opt_design_core()
{
	// map 'auto_opt_des_par_in' to 'ms_auto_opt_des_par'
	ms_opt_des_par.m_W_dot_net = ms_auto_opt_des_par.m_W_dot_net;	//[kWe]
	ms_opt_des_par.m_T_mc_in = ms_auto_opt_des_par.m_T_mc_in;		//[K]
	ms_opt_des_par.m_T_pc_in = ms_auto_opt_des_par.m_T_pc_in;		//[K]
	ms_opt_des_par.m_T_t_in = ms_auto_opt_des_par.m_T_t_in;			//[K]
	ms_opt_des_par.m_DP_LTR = ms_auto_opt_des_par.m_DP_LTR;			        //(cold, hot) positive values are absolute [kPa], negative values are relative (-)
	ms_opt_des_par.m_DP_HTR = ms_auto_opt_des_par.m_DP_HTR;				    //(cold, hot) positive values are absolute [kPa], negative values are relative (-)
	ms_opt_des_par.m_DP_PC_full = ms_auto_opt_des_par.m_DP_PC_pre;		    //(cold, hot) positive values are absolute [kPa], negative values are relative (-)
	ms_opt_des_par.m_DP_PC_partial = ms_auto_opt_des_par.m_DP_PC_main;   //(cold, hot) positive values are absolute [kPa], negative values are relative (-)
	ms_opt_des_par.m_DP_PHX = ms_auto_opt_des_par.m_DP_PHX;				    //(cold, hot) positive values are absolute [kPa], negative values are relative (-)
	ms_opt_des_par.m_UA_rec_total = ms_auto_opt_des_par.m_UA_rec_total;		//[kW/K]
	ms_opt_des_par.m_LTR_eff_max = ms_auto_opt_des_par.m_LTR_eff_max;		//[-]
	ms_opt_des_par.m_HTR_eff_max = ms_auto_opt_des_par.m_HTR_eff_max;		//[-]
	ms_opt_des_par.m_eta_mc = ms_auto_opt_des_par.m_eta_mc;					//[-]
	ms_opt_des_par.m_eta_rc = ms_auto_opt_des_par.m_eta_rc;					//[-]
	ms_opt_des_par.m_eta_pc = ms_auto_opt_des_par.m_eta_pc;					//[-]
	ms_opt_des_par.m_eta_t = ms_auto_opt_des_par.m_eta_t;					//[-]
	ms_opt_des_par.m_N_sub_hxrs = ms_auto_opt_des_par.m_N_sub_hxrs;			//[-]
	ms_opt_des_par.m_P_high_limit = ms_auto_opt_des_par.m_P_high_limit;		//[kPa]
	ms_opt_des_par.m_tol = ms_auto_opt_des_par.m_tol;						//[-]
	ms_opt_des_par.m_opt_tol = ms_auto_opt_des_par.m_opt_tol;				//[-]
	ms_opt_des_par.m_N_turbine = ms_auto_opt_des_par.m_N_turbine;			//[rpm] Turbine shaft speed (negative values link turbine to compressor)

	ms_opt_des_par.m_des_objective_type = ms_auto_opt_des_par.m_des_objective_type;	//[-]
	ms_opt_des_par.m_min_phx_deltaT = ms_auto_opt_des_par.m_min_phx_deltaT;			//[C]

	// Outer optimization loop
	m_objective_metric_auto_opt = 0.0;

	double P_low_limit = std::min(ms_auto_opt_des_par.m_P_high_limit, std::max(10.E3, ms_auto_opt_des_par.m_P_high_limit*0.2));		//[kPa]
	double best_P_high = fminbr(
		P_low_limit, ms_auto_opt_des_par.m_P_high_limit, &fmin_cb_opt_partialcooling_des_fixed_P_high, this, 1.0);

	// fminb_cb_opt_partialcooling_des_fixed_P_high should calculate:
		// ms_des_par_optimal;
		// m_eta_thermal_opt;

		// Complete 'ms_opt_des_par'
	ms_opt_des_par.m_P_mc_out_guess = ms_auto_opt_des_par.m_P_high_limit;	//[kPa]
	ms_opt_des_par.m_fixed_P_mc_out = true;

	ms_opt_des_par.m_fixed_PR_total = false;
	ms_opt_des_par.m_PR_total_guess = 25. / 6.5;	//[-] Guess could be improved...

	ms_opt_des_par.m_fixed_f_PR_mc = false;
	ms_opt_des_par.m_f_PR_mc_guess = (25. - 8.5) / (25. - 6.5);		//[-] Guess could be improved...

	ms_opt_des_par.m_recomp_frac_guess = 0.25;	//[-]
	ms_opt_des_par.m_fixed_recomp_frac = false;

	ms_opt_des_par.m_LTR_frac_guess = 0.5;		//[-]
	ms_opt_des_par.m_fixed_LTR_frac = false;

	int pc_error_code = opt_design_core();

	if (pc_error_code == 0 && m_objective_metric_opt > m_objective_metric_auto_opt)
	{
		ms_des_par_auto_opt = ms_des_par_optimal;
		m_objective_metric_auto_opt = m_objective_metric_opt;
	}

	ms_des_par = ms_des_par_auto_opt;

	int pc_opt_des_error_code = design_core();

	if (pc_opt_des_error_code != 0)
	{
		return pc_opt_des_error_code;
	}

	pc_opt_des_error_code = finalize_design();

	return pc_opt_des_error_code;
}

int C_PartialCooling_Cycle::auto_opt_design_hit_eta(S_auto_opt_design_hit_eta_parameters & auto_opt_des_hit_eta_in, std::string & error_msg)
{
	throw(C_csp_exception("C_PartialCooling_Cycle::auto_opt_design_hit_eta does not yet exist"));

	return -1;
}

int C_PartialCooling_Cycle::off_design_fix_shaft_speeds_core()
{
	// Need to reset 'ms_od_solved' here
	clear_ms_od_solved();

	// Initialize known cycle state point properties
	mv_temp_od[MC_IN] = ms_od_par.m_T_mc_in;		//[K]
	
	mv_temp_od[PC_IN] = ms_od_par.m_T_pc_in;		//[K]
	mv_pres_od[PC_IN] = ms_od_par.m_P_LP_comp_in;	//[kPa]

	mv_temp_od[TURB_IN] = ms_od_par.m_T_t_in;		//[K]

	// Solve for recompression fraction that results in all turbomachinery operating at design/target speeds
	C_MEQ__f_recomp__y_N_rc c_rc_shaft_speed(this, ms_od_par.m_T_pc_in,
													ms_od_par.m_P_LP_comp_in,
													ms_od_par.m_T_mc_in,
													ms_od_par.m_T_t_in);

	C_monotonic_eq_solver c_rc_shaft_speed_solver(c_rc_shaft_speed);

	if (ms_des_solved.m_is_rc)
	{
		// Define bounds on the recompression fraction
		double f_recomp_lower = 0.0;
		double f_recomp_upper = 1.0;

		c_rc_shaft_speed_solver.settings(1.E-3, 50, f_recomp_lower, f_recomp_upper, false);

		C_monotonic_eq_solver::S_xy_pair f_recomp_pair_1st;
		C_monotonic_eq_solver::S_xy_pair f_recomp_pair_2nd;

		double f_recomp_guess = ms_des_solved.m_recomp_frac;
		double y_f_recomp_guess = std::numeric_limits<double>::quiet_NaN();

		// Send the guessed recompression fraction to method; see if it returns a calculated N_rc or fails
		int rc_shaft_speed_err_code = c_rc_shaft_speed_solver.call_mono_eq(f_recomp_guess, &y_f_recomp_guess);

		// If guessed recompression fraction fails, then try to find a recompression fraction that works
		if (rc_shaft_speed_err_code != 0)
		{
			double delta = 0.02;
			bool is_iter = true;
			for (int i = 1; is_iter; i++)
			{
				for (int j = -1; j <= 1; j += 2)
				{
					f_recomp_guess = std::min(1.0, std::max(0.0, ms_des_solved.m_recomp_frac + j * i*delta));
					rc_shaft_speed_err_code = c_rc_shaft_speed_solver.call_mono_eq(f_recomp_guess, &y_f_recomp_guess);
					if (rc_shaft_speed_err_code == 0)
					{
						is_iter = false;
						break;
					}
					if (f_recomp_guess == 0.0)
					{
						// Have tried a fairly fine grid of recompression fraction values; have not found one that works
						return -40;
					}
				}
			}
		}

		f_recomp_pair_1st.x = f_recomp_guess;
		f_recomp_pair_1st.y = y_f_recomp_guess;

		f_recomp_guess = 1.02*f_recomp_pair_1st.x;
		rc_shaft_speed_err_code = c_rc_shaft_speed_solver.call_mono_eq(f_recomp_guess, &y_f_recomp_guess);

		if (rc_shaft_speed_err_code == 0)
		{
			f_recomp_pair_2nd.x = f_recomp_guess;
			f_recomp_pair_2nd.y = y_f_recomp_guess;
		}
		else
		{
			f_recomp_guess = 0.98*f_recomp_pair_1st.x;
			rc_shaft_speed_err_code = c_rc_shaft_speed_solver.call_mono_eq(f_recomp_guess, &y_f_recomp_guess);

			if (rc_shaft_speed_err_code == 0)
			{
				f_recomp_pair_2nd.x = f_recomp_guess;
				f_recomp_pair_2nd.y = y_f_recomp_guess;
			}
			else
			{
				return -41;
			}
		}

		// Now, using the two solved guess values, solve for the recompression fraction that results in:
		// ... balanced turbomachinery at their design/target shaft speed
		double f_recomp_solved, tol_solved;
		f_recomp_solved = tol_solved = std::numeric_limits<double>::quiet_NaN();
		int iter_solved = -1;

		int f_recomp_code = 0;
		try
		{
			f_recomp_code = c_rc_shaft_speed_solver.solve(f_recomp_pair_1st, f_recomp_pair_2nd, 0.0,
													f_recomp_solved, tol_solved, iter_solved);
		}
		catch (C_csp_exception)
		{
			return -42;
		}

		if (f_recomp_code != C_monotonic_eq_solver::CONVERGED)
		{
			int n_call_history = (int)c_rc_shaft_speed_solver.get_solver_call_history()->size();

			int error_code = 0;
			if(n_call_history > 0)
				error_code = -(*(c_rc_shaft_speed_solver.get_solver_call_history()))[n_call_history - 1].err_code;

			if (error_code == 0)
				error_code = f_recomp_code;

			return error_code;
		}

	}
	else
	{
		double y_eq = std::numeric_limits<double>::quiet_NaN();
		int rc_shaft_speed_err_code = c_rc_shaft_speed_solver.call_mono_eq(0.0, &y_eq);
		if (rc_shaft_speed_err_code != 0)
		{
			throw(C_csp_exception("C_PartialCooling::off_design_fix_shaft_speeds_core does not yet have ability to solve for cycles with recompression"));
		}
	}

	// Fully define known states
	std::vector<int> known_states;
	known_states.push_back(PC_IN);
	known_states.push_back(PC_OUT);
	known_states.push_back(RC_OUT);
	known_states.push_back(MC_IN);
	known_states.push_back(MC_OUT);
	known_states.push_back(TURB_IN);
	known_states.push_back(TURB_OUT);	

	int n_states = known_states.size();
	int prop_error_code = 0;
	for (int i = 0; i < n_states; i++)
	{
		int j = known_states[i];
		prop_error_code = CO2_TP(mv_temp_od[j], mv_pres_od[j], &mc_co2_props);
		if (prop_error_code != 0)
		{
			return prop_error_code;
		}
		mv_enth_od[j] = mc_co2_props.enth;
		mv_entr_od[j] = mc_co2_props.entr;
		mv_dens_od[j] = mc_co2_props.dens;
	}

	// Solve recuperators here...
	double T_HTR_LP_out_lower = mv_temp_od[MC_OUT];		//[K] Coldest possible temperature
	double T_HTR_LP_out_upper = mv_temp_od[TURB_OUT];	//[K] Hottest possible temperature

	double T_HTR_LP_out_guess_lower = std::min(T_HTR_LP_out_upper - 2.0, std::max(T_HTR_LP_out_lower + 15.0, mv_temp_od[RC_OUT]));
	double T_HTR_LP_out_guess_upper = std::min(T_HTR_LP_out_guess_lower + 20.0, T_HTR_LP_out_upper - 1.0);

	C_MEQ_recup_od recup_od_eq(this);
	C_monotonic_eq_solver recup_od_solver(recup_od_eq);

	recup_od_solver.settings(ms_des_par.m_tol*mv_temp_od[MC_IN], 1000, T_HTR_LP_out_lower, T_HTR_LP_out_upper, false);

	double T_HTR_LP_out_solved, tol_T_HTR_LP_out_solved;
	T_HTR_LP_out_solved = tol_T_HTR_LP_out_solved = std::numeric_limits<double>::quiet_NaN();
	int iter_T_HTR_LP_out = -1;

	int T_HTR_LP_out_code = recup_od_solver.solve(T_HTR_LP_out_guess_lower, T_HTR_LP_out_guess_upper, 0,
		T_HTR_LP_out_solved, tol_T_HTR_LP_out_solved, iter_T_HTR_LP_out);

	if (T_HTR_LP_out_code != C_monotonic_eq_solver::CONVERGED)
	{
		int n_call_history = (int)recup_od_solver.get_solver_call_history()->size();
		int error_code = 0;
		if (n_call_history > 0)
			error_code = (*(recup_od_solver.get_solver_call_history()))[n_call_history - 1].err_code;
		if (error_code == 0)
		{
			error_code = T_HTR_LP_out_code;
		}
		return error_code;
	}

	// Fully define remaining states
	std::vector<int> remaining_states;
	remaining_states.push_back(HTR_LP_OUT);
	remaining_states.push_back(LTR_LP_OUT);
	remaining_states.push_back(HTR_HP_OUT);

	n_states = remaining_states.size();
	prop_error_code = 0;
	for (int i = 0; i < n_states; i++)
	{
		int j = remaining_states[i];
		prop_error_code = CO2_TP(mv_temp_od[j], mv_pres_od[j], &mc_co2_props);
		if (prop_error_code != 0)
		{
			return prop_error_code;
		}
		mv_enth_od[j] = mc_co2_props.enth;
		mv_entr_od[j] = mc_co2_props.entr;
		mv_dens_od[j] = mc_co2_props.dens;
	}

	// Get mass flow rates from solver
	double m_dot_mc = c_rc_shaft_speed.m_m_dot_mc;	//[kg/s]
	double m_dot_rc = c_rc_shaft_speed.m_m_dot_rc;	//[kg/s]
	double m_dot_pc = c_rc_shaft_speed.m_m_dot_pc;	//[kg/s]
	double m_dot_t = c_rc_shaft_speed.m_m_dot_t;	//[kg/s]

	// Calculate cycle performance metrics
	double w_pc = mv_enth_od[PC_IN] - mv_enth_od[PC_OUT];		//[kJ/kg] (negative) specific work of pre-compressor
	double w_mc = mv_enth_od[MC_IN] - mv_enth_od[MC_OUT];		//[kJ/kg] (negative) specific work of main compressor
	double w_t = mv_enth_od[TURB_IN] - mv_enth_od[TURB_OUT];	//[kJ/kg] (positive) specific work of turbine
	
	double w_rc = 0.0;
	if (m_dot_rc > 0.0)
		w_rc = mv_enth_od[PC_OUT] - mv_enth_od[RC_OUT];		//[kJ/kg] (negative) specific work of recompressor

	m_Q_dot_PHX_od = m_dot_t * (mv_enth_od[TURB_IN] - mv_enth_od[HTR_HP_OUT]);
	m_W_dot_net_od = w_pc*m_dot_pc + w_mc*m_dot_mc + w_rc*m_dot_rc + w_t*m_dot_t;
	m_eta_thermal_od = m_W_dot_net_od / m_Q_dot_PHX_od;

	// Get 'od_solved' structures from component classes
	//ms_od_solved.ms_mc_od_solved = *m_mc.get_od_solved();
	ms_od_solved.ms_mc_ms_od_solved = *mc_mc.get_od_solved();
	ms_od_solved.ms_rc_ms_od_solved = *mc_rc.get_od_solved();
	ms_od_solved.ms_t_od_solved = *mc_t.get_od_solved();
	ms_od_solved.ms_LT_recup_od_solved = mc_LTR.ms_od_solved;
	ms_od_solved.ms_HT_recup_od_solved = mc_HTR.ms_od_solved;

	// Set ms_od_solved
	ms_od_solved.m_eta_thermal = m_eta_thermal_od;
	ms_od_solved.m_W_dot_net = m_W_dot_net_od;
	ms_od_solved.m_Q_dot = m_Q_dot_PHX_od;
	ms_od_solved.m_m_dot_mc = m_dot_mc;
	ms_od_solved.m_m_dot_rc = m_dot_rc;
	ms_od_solved.m_m_dot_t = m_dot_t;
	ms_od_solved.m_recomp_frac = m_dot_rc / m_dot_t;

	ms_od_solved.m_temp = mv_temp_od;
	ms_od_solved.m_pres = mv_pres_od;
	ms_od_solved.m_enth = mv_enth_od;
	ms_od_solved.m_entr = mv_entr_od;
	ms_od_solved.m_dens = mv_dens_od;

	return 0;
}

int C_PartialCooling_Cycle::C_MEQ_recup_od::operator()(double T_HTR_LP_out_guess /*K*/, double *diff_T_HTR_LP_out /*K*/)
{
	mpc_pc_cycle->mv_temp_od[HTR_LP_OUT] = T_HTR_LP_out_guess;		//[K]

		// Low temperature recuperator
	double Q_dot_LTR = std::numeric_limits<double>::quiet_NaN();
	try
	{
		mpc_pc_cycle->mc_LTR.off_design_solution(mpc_pc_cycle->mv_temp_od[MC_OUT], mpc_pc_cycle->mv_pres_od[MC_OUT], mpc_pc_cycle->m_m_dot_mc, mpc_pc_cycle->mv_pres_od[LTR_HP_OUT],
			mpc_pc_cycle->mv_temp_od[HTR_LP_OUT], mpc_pc_cycle->mv_pres_od[HTR_LP_OUT], mpc_pc_cycle->m_m_dot_t, mpc_pc_cycle->mv_pres_od[LTR_LP_OUT],
			Q_dot_LTR, mpc_pc_cycle->mv_temp_od[LTR_HP_OUT], mpc_pc_cycle->mv_temp_od[LTR_LP_OUT]);
	}
	catch (C_csp_exception &)
	{
		*diff_T_HTR_LP_out = std::numeric_limits<double>::quiet_NaN();

		return -1;
	}

		// Mixer
			// Get LTR HP outlet state
	int prop_error_code = CO2_TP(mpc_pc_cycle->mv_temp_od[LTR_HP_OUT], mpc_pc_cycle->mv_pres_od[LTR_HP_OUT], &mpc_pc_cycle->mc_co2_props);
	if (prop_error_code != 0)
	{
		*diff_T_HTR_LP_out = std::numeric_limits<double>::quiet_NaN();
		return prop_error_code;
	}
	mpc_pc_cycle->mv_enth_od[LTR_HP_OUT] = mpc_pc_cycle->mc_co2_props.enth;	//[kJ/kg]
	mpc_pc_cycle->mv_entr_od[LTR_HP_OUT] = mpc_pc_cycle->mc_co2_props.entr;	//[kJ/kg-K]
	mpc_pc_cycle->mv_dens_od[LTR_HP_OUT] = mpc_pc_cycle->mc_co2_props.dens;	//[kg/m^3]

			// Enthalpy balance at mixer
	if (mpc_pc_cycle->m_m_dot_rc > 1.E-12)
	{
		double f_recomp = mpc_pc_cycle->m_m_dot_rc / mpc_pc_cycle->m_m_dot_t;	//[-]
			// Conservation of energy
		mpc_pc_cycle->mv_enth_od[MIXER_OUT] = (1.0 - f_recomp)*mpc_pc_cycle->mv_enth_od[LTR_HP_OUT] +
												f_recomp*mpc_pc_cycle->mv_enth_od[RC_OUT];
		prop_error_code = CO2_PH(mpc_pc_cycle->mv_pres_od[MIXER_OUT], mpc_pc_cycle->mv_enth_od[MIXER_OUT], &mpc_pc_cycle->mc_co2_props);
		if (prop_error_code != 0)
		{
			*diff_T_HTR_LP_out = std::numeric_limits<double>::quiet_NaN();
			return prop_error_code;
		}
		mpc_pc_cycle->mv_temp_od[MIXER_OUT] = mpc_pc_cycle->mc_co2_props.temp;	//[K]
		mpc_pc_cycle->mv_entr_od[MIXER_OUT] = mpc_pc_cycle->mc_co2_props.entr;	//[kJ/kg-K]
		mpc_pc_cycle->mv_dens_od[MIXER_OUT] = mpc_pc_cycle->mc_co2_props.dens;	//[kg/m^3]
	}
	else
	{
		mpc_pc_cycle->mv_enth_od[MIXER_OUT] = mpc_pc_cycle->mv_enth_od[LTR_HP_OUT];		//[kJ/kg]
		mpc_pc_cycle->mv_temp_od[MIXER_OUT] = mpc_pc_cycle->mv_temp_od[LTR_HP_OUT];		//[K]
		mpc_pc_cycle->mv_entr_od[MIXER_OUT] = mpc_pc_cycle->mv_entr_od[LTR_HP_OUT];		//[kJ/kg-K]
		mpc_pc_cycle->mv_dens_od[MIXER_OUT] = mpc_pc_cycle->mv_dens_od[LTR_HP_OUT];		//[kg/m^3]
	}

		// High temperature recuperator
	double T_HTR_LP_out_calc = std::numeric_limits<double>::quiet_NaN();
	double Q_dot_HTR = std::numeric_limits<double>::quiet_NaN();
	try
	{
		mpc_pc_cycle->mc_HTR.off_design_solution(mpc_pc_cycle->mv_temp_od[MIXER_OUT], mpc_pc_cycle->mv_pres_od[MIXER_OUT], 
			mpc_pc_cycle->m_m_dot_t, mpc_pc_cycle->mv_pres_od[HTR_HP_OUT],
			mpc_pc_cycle->mv_temp_od[TURB_OUT], mpc_pc_cycle->mv_pres_od[TURB_OUT], 
			mpc_pc_cycle->m_m_dot_t, mpc_pc_cycle->mv_pres_od[HTR_LP_OUT],
			Q_dot_HTR, mpc_pc_cycle->mv_temp_od[HTR_HP_OUT], T_HTR_LP_out_calc);
	}
	catch (C_csp_exception & csp_except)
	{
		*diff_T_HTR_LP_out = std::numeric_limits<double>::quiet_NaN();
		if (csp_except.m_error_code == 0)
			return -1;
		else
			return csp_except.m_error_code;
	}

	*diff_T_HTR_LP_out = T_HTR_LP_out_calc - mpc_pc_cycle->mv_temp_od[HTR_LP_OUT];	//[K]

	return 0;
}

int C_PartialCooling_Cycle::C_MEQ__f_recomp__y_N_rc::operator()(double f_recomp /*-*/, double *diff_N_rc /*-*/)
{
	// Solve for turbine mass flow rate
	C_MEQ__t_m_dot__bal_turbomachinery c_turbo_bal(mpc_pc_cycle, m_T_pc_in, 
													m_P_pc_in, 
													m_T_mc_in,
													f_recomp,
													m_T_t_in);

	C_monotonic_eq_solver c_turbo_bal_solver(c_turbo_bal);

	// Set lower bound on mass flow rate
	double m_dot_lower = mpc_pc_cycle->ms_des_solved.m_m_dot_t * 1.E-3;		//[kg/s]
	double m_dot_upper = std::numeric_limits<double>::quiet_NaN();

	// Set solver settings
	// Because this application of the solver is trying to get the calculated mass flow rate to match the guess
	//  ... then need to calculate the error in the operator() method
	// So the error is already relative, and the solver is comparing against absolute value of 0
	c_turbo_bal_solver.settings(1.E-3, 100, m_dot_lower, m_dot_upper, false);

	// Generate two guess values
	double m_dot_guess_upper = mpc_pc_cycle->ms_des_solved.m_m_dot_t;		//[kg/s]
	double y_m_dot_guess_upper = std::numeric_limits<double>::quiet_NaN();	//[-]

	int m_dot_guess_iter = 0;
	int m_dot_err_code = 1;
	while (m_dot_err_code != 0)
	{
		if (m_dot_guess_iter > 0)
			m_dot_guess_upper *= 0.9;
		if (m_dot_guess_iter > 10)
		{
			*diff_N_rc = std::numeric_limits<double>::quiet_NaN();
			return -71;
		}
		m_dot_guess_iter++;
		m_dot_err_code = c_turbo_bal_solver.call_mono_eq(m_dot_guess_upper, &y_m_dot_guess_upper);
	}

	double m_dot_guess_lower = 0.7*m_dot_guess_upper;		//[kg/s]

															// Now, solve for the turbine mass flow rate
	double m_dot_t_solved, tol_solved;
	m_dot_t_solved = tol_solved = std::numeric_limits<double>::quiet_NaN();
	int iter_solved = -1;

	int m_dot_t_code = c_turbo_bal_solver.solve(m_dot_guess_lower, m_dot_guess_upper, 0.0,
		m_dot_t_solved, tol_solved, iter_solved);

	if (m_dot_t_code != C_monotonic_eq_solver::CONVERGED)
	{
		*diff_N_rc = std::numeric_limits<double>::quiet_NaN();
		return m_dot_t_code;
	}

	m_m_dot_t = m_dot_t_solved;	//[kg/s]
	m_m_dot_pc = m_m_dot_t;		//[kg/s]
	m_m_dot_rc = m_m_dot_t * f_recomp;	//[kg/s]
	m_m_dot_mc = m_m_dot_t - m_m_dot_rc;

	// Now we know the required recompressor performance, so we can solve the recompressor
	//     model for shaft speed and report design/target
	if (m_m_dot_rc > 1.E-12)
	{
		int rc_error_code = 0;
		mpc_pc_cycle->mc_rc.off_design_given_P_out(mpc_pc_cycle->mv_temp_od[PC_OUT], mpc_pc_cycle->mv_pres_od[PC_OUT], m_m_dot_rc,
			mpc_pc_cycle->mv_pres_od[RC_OUT], rc_error_code, mpc_pc_cycle->mv_temp_od[RC_OUT]);

		if (rc_error_code != 0)
		{
			*diff_N_rc = std::numeric_limits<double>::quiet_NaN();
			return rc_error_code;
		}

		// Get recompressor shaft speed
		double N_rc = mpc_pc_cycle->mc_rc.get_od_solved()->m_N;
		double N_rc_des = mpc_pc_cycle->mc_rc.get_design_solved()->m_N_design;

		// Get difference between solved and design shaft speed
		*diff_N_rc = (N_rc - N_rc_des) / N_rc_des;
	}
	else
	{
		*diff_N_rc = 0.0;
	}

	return 0;
}

int C_PartialCooling_Cycle::C_MEQ__t_m_dot__bal_turbomachinery::operator()(double m_dot_t_in /*kg/s*/, double *diff_m_dot_t /*-*/)
{
	// Calculate the main compressor mass flow rate
	double m_dot_mc = (1.0 - m_f_recomp)*m_dot_t_in;		//[kg/s]

	// Calculate Pre-compressor performance
	int pc_error_code = 0;
	double T_pc_out, P_pc_out;
	T_pc_out = P_pc_out = std::numeric_limits<double>::quiet_NaN();

	mpc_pc_cycle->mc_pc.off_design_at_N_des(m_T_pc_in, m_P_pc_in, m_dot_t_in, pc_error_code,
								T_pc_out, P_pc_out);

	// Check that pre-compressor performance solved
	if (pc_error_code != 0)
	{
		*diff_m_dot_t = std::numeric_limits<double>::quiet_NaN();
		return pc_error_code;
	}

	mpc_pc_cycle->mv_pres_od[PC_OUT] = P_pc_out;	//[kPa]
	mpc_pc_cycle->mv_temp_od[PC_OUT] = T_pc_out;	//[K]

	// Calculate scaled pressure drop through main compressor cooler
	std::vector<double> DP_cooler_main;
	std::vector<double> m_dot_cooler_main;
	m_dot_cooler_main.push_back(0.0);
	m_dot_cooler_main.push_back(m_dot_mc);		//[kg/s]
	mpc_pc_cycle->mc_cooler_mc.hxr_pressure_drops(m_dot_cooler_main, DP_cooler_main);

	mpc_pc_cycle->mv_pres_od[MC_IN] = mpc_pc_cycle->mv_pres_od[PC_OUT] - DP_cooler_main[1];	//[kPa]

	// Calculate main compressor performance
	int mc_error_code = 0;
	double T_mc_out, P_mc_out;
	T_mc_out = P_mc_out = std::numeric_limits<double>::quiet_NaN();

	// Solve main compressor performance at design/target shaft speed
	mpc_pc_cycle->mc_mc.off_design_at_N_des(m_T_mc_in, mpc_pc_cycle->mv_pres_od[MC_IN], m_dot_mc,
				mc_error_code, T_mc_out, P_mc_out);

	// Check that main compressor performance solved
	if (mc_error_code != 0)
	{
		*diff_m_dot_t = std::numeric_limits<double>::quiet_NaN();
		return mc_error_code;
	}

	mpc_pc_cycle->mv_pres_od[MC_OUT] = P_mc_out;	//[kPa]
	mpc_pc_cycle->mv_temp_od[MC_OUT] = T_mc_out;	//[K]

	// Calculate scaled pressure drops through remaining heat exchangers
		// Low temp recuperator
	std::vector<double> DP_LTR;
	DP_LTR.resize(2);
	DP_LTR[0] = mpc_pc_cycle->mc_LTR.od_delta_p_cold(m_dot_mc);
	DP_LTR[1] = mpc_pc_cycle->mc_LTR.od_delta_p_hot(m_dot_t_in);
		// High temp recuperator
	std::vector<double> DP_HTR;
	DP_HTR.resize(2);
	DP_HTR[0] = mpc_pc_cycle->mc_HTR.od_delta_p_cold(m_dot_t_in);
	DP_HTR[1] = mpc_pc_cycle->mc_HTR.od_delta_p_hot(m_dot_t_in);
		// Primary heat exchanger
	std::vector<double> DP_PHX;
	std::vector<double> m_dot_PHX;
	m_dot_PHX.push_back(m_dot_t_in);
	m_dot_PHX.push_back(0.0);
	mpc_pc_cycle->mc_PHX.hxr_pressure_drops(m_dot_PHX, DP_PHX);
		// Pre-compressor cooler
	std::vector<double> DP_cooler_pre;
	std::vector<double> m_dot_cooler_pre;
	m_dot_cooler_pre.push_back(0.0);
	m_dot_cooler_pre.push_back(m_dot_t_in);
	mpc_pc_cycle->mc_cooler_pc.hxr_pressure_drops(m_dot_cooler_pre, DP_cooler_pre);
		
	// Apply pressure drops to heat exchangers, fully defining the pressure at all states
	mpc_pc_cycle->mv_pres_od[LTR_HP_OUT] = mpc_pc_cycle->mv_pres_od[MC_OUT] - DP_LTR[0];
	mpc_pc_cycle->mv_pres_od[MIXER_OUT] = mpc_pc_cycle->mv_pres_od[LTR_HP_OUT];
	mpc_pc_cycle->mv_pres_od[RC_OUT] = mpc_pc_cycle->mv_pres_od[LTR_HP_OUT];
	mpc_pc_cycle->mv_pres_od[HTR_HP_OUT] = mpc_pc_cycle->mv_pres_od[MIXER_OUT] - DP_HTR[0];
	double P_t_in = mpc_pc_cycle->mv_pres_od[TURB_IN] = mpc_pc_cycle->mv_pres_od[HTR_HP_OUT] - DP_PHX[0];
	mpc_pc_cycle->mv_pres_od[LTR_LP_OUT] = mpc_pc_cycle->mv_pres_od[PC_IN] + DP_cooler_pre[1];
	mpc_pc_cycle->mv_pres_od[HTR_LP_OUT] = mpc_pc_cycle->mv_pres_od[LTR_LP_OUT] + DP_LTR[1];
	double P_t_out = mpc_pc_cycle->mv_pres_od[TURB_OUT] = mpc_pc_cycle->mv_pres_od[HTR_LP_OUT] + DP_HTR[1];

	// Calculate turbine performance
	int t_err_code = 0;
	double m_dot_t_calc, T_t_out;
	m_dot_t_calc = T_t_out = std::numeric_limits<double>::quiet_NaN();
	mpc_pc_cycle->mc_t.od_turbine_at_N_des(m_T_t_in, P_t_in, P_t_out, t_err_code, m_dot_t_calc, T_t_out);

	// Check that turbine performance solved
	if (t_err_code != 0)
	{
		*diff_m_dot_t = std::numeric_limits<double>::quiet_NaN();
		return t_err_code;
	}

	mpc_pc_cycle->mv_temp_od[TURB_OUT] = T_t_out;	//[K]

	// Calculate difference between calculated and guessed mass flow rates
	*diff_m_dot_t = (m_dot_t_calc - m_dot_t_in) / m_dot_t_in;

	return 0;
}

int C_PartialCooling_Cycle::off_design_fix_shaft_speeds(S_od_par & od_phi_par_in)
{
	ms_od_par = od_phi_par_in;

	return off_design_fix_shaft_speeds_core();
}

double nlopt_cb_opt_partialcooling_des(const std::vector<double> &x, std::vector<double> &grad, void *data)
{
	C_PartialCooling_Cycle *frame = static_cast<C_PartialCooling_Cycle*>(data);
	if (frame != NULL)
		return frame->design_cycle_return_objective_metric(x);
	else
		return 0.0;
}

double fmin_cb_opt_partialcooling_des_fixed_P_high(double P_high /*kPa*/, void *data)
{
	C_PartialCooling_Cycle *frame = static_cast<C_PartialCooling_Cycle*>(data);

	return frame->opt_eta_fixed_P_high(P_high);
}
