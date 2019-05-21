#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include "lib_utility_rate.h"
#include "json/json.h"

util::matrix_t<double> UtilityRate::getEnergyRatesMatrix() { return m_ecRatesMatrix; }

UtilityRate::UtilityRate(std::string urdb_json_chars)
{
	parseUrdbRate(urdb_json_chars);
}

bool UtilityRate::parseUrdbRate(std::string urdb_reponse)
{
	Json::CharReaderBuilder builder;
	Json::CharReader * reader = builder.newCharReader();
	bool parseOk = true;
	std::string errors;
	parseOk = reader->parse(urdb_reponse.c_str(), urdb_reponse.c_str() + urdb_reponse.size(), &m_urdb, &errors );
	delete reader;

	if (parseOk)
	{
		m_annual_min_charge_dollar = m_urdb["annualmincharge"].asDouble();
		m_monthly_min_charge_dollar = m_urdb["minmonthlycharge"].asDouble();
		m_monthly_fixed_charge_dollar = m_urdb["fixedmonthlycharge"].asDouble();
		
		size_t energy_rows = 0;
		std::vector<double> ec_rates_vector;
		const Json::Value energyRatePeriods = m_urdb["energyratestructure"];
		if (energyRatePeriods) {
			for (size_t p = 0; p < energyRatePeriods.size(); p++) {
				const Json::Value period = energyRatePeriods[(int)p];
				for (size_t t = 0; t < period.size(); t++) {
					const Json::Value tier = period[(int)t];
					double max = tier["max"].asDouble();
					if (!max) {
						max = 1e38;
					}
					int iunit = 0;
					std::string unit = tier["unit"].asString();
					std::transform(unit.begin(), unit.end(), unit.begin(), ::tolower);
					if (unit == "kwh") {
						iunit = 0;
					}
					else if (unit == "kwh/kw") {
						iunit = 1;
					}
					else if (unit == "kwh daily") {
						iunit = 2;
					}
					else if (unit == "kwh/kw daily") {
						iunit = 3;
					}

					ec_rates_vector.push_back(p + 1);
					ec_rates_vector.push_back(t + 1);
					ec_rates_vector.push_back(max);
					ec_rates_vector.push_back(iunit);
					ec_rates_vector.push_back(tier["rate"].asDouble() + tier["adj"].asDouble());
					ec_rates_vector.push_back(tier["sell"].asDouble());
					energy_rows++;
				}
			}
		}
		m_ecRatesMatrix = util::matrix_t<double>(energy_rows, 6, &ec_rates_vector);



		const Json::Value flatdemandstructure = m_urdb["flatdemandstructure"];
		if (flatdemandstructure) {
			for (size_t p = 0; p < flatdemandstructure.size(); p++) {
				const Json::Value period = flatdemandstructure[(int)p];
				for (size_t t = 0; t < period.size(); t++) {
					const Json::Value tier = period[(int)t];
					const double rate = tier["rate"].asDouble();
				}
			}
		}

	}
	return parseOk;
}




UtilityRate::UtilityRate(util::matrix_t<size_t> ecWeekday, util::matrix_t<size_t> ecWeekend, util::matrix_t<double> ecRatesMatrix)
{
	m_ecWeekday = ecWeekday;
	m_ecWeekend = ecWeekend;
	m_ecRatesMatrix = ecRatesMatrix;
}

UtilityRateCalculator::UtilityRateCalculator(UtilityRate * rate, size_t stepsPerHour) :
	UtilityRate(*rate)
{
	m_stepsPerHour = stepsPerHour;
	initializeRate();
}

UtilityRateCalculator::UtilityRateCalculator(UtilityRate * rate, size_t stepsPerHour, std::vector<double> loadProfile) :
	UtilityRate(*rate)
{
	m_stepsPerHour = stepsPerHour;
	m_loadProfile = loadProfile;
	initializeRate();
}

void UtilityRateCalculator::initializeRate()
{
	
	for (size_t r = 0; r != m_ecRatesMatrix.nrows(); r++)
	{
		size_t period = static_cast<size_t>(m_ecRatesMatrix(r, 0));
		size_t tier = static_cast<size_t>(m_ecRatesMatrix(r, 1));

		// assumers table is in monotonically increasing order
		m_energyTiersPerPeriod[period] = tier;

		if (tier == 1)
			m_energyUsagePerPeriod.push_back(0);
	}
}

void UtilityRateCalculator::updateLoad(double loadPower)
{
	m_loadProfile.push_back(loadPower);
}
void UtilityRateCalculator::calculateEnergyUsagePerPeriod()
{
	for (size_t idx = 0; idx != m_loadProfile.size(); idx++)
	{
		size_t hourOfYear = static_cast<size_t>(std::floor(idx / m_stepsPerHour));
		size_t period = static_cast<size_t>(getEnergyPeriod(hourOfYear));
		m_energyUsagePerPeriod[period] += m_loadProfile[idx];
	}
}
double UtilityRateCalculator::getEnergyRate(size_t hourOfYear)
{
	// period is the human readable value from the table (1-based)
	size_t period = getEnergyPeriod(hourOfYear);

	//size_t idx = m_loadProfile.size() - 1;
	//double energy = m_energyTiersPerPeriod[period];
	// add ability to check for tiered usage, for now assume one tier

	// Reduce period to 0-based index
	return m_ecRatesMatrix(period - 1, 4);

}
size_t UtilityRateCalculator::getEnergyPeriod(size_t hourOfYear)
{
	size_t period, month, hour;
	util::month_hour(hourOfYear, month, hour);

	if (util::weekday(hourOfYear)) {
		if (m_ecWeekday.nrows() == 1 && m_ecWeekday.ncols() == 1) {
			period = m_ecWeekday.at(0, 0);
		}
		else {
			period = m_ecWeekday.at(month - 1, hour - 1);
		}
	}
	else {
		if (m_ecWeekend.nrows() == 1 && m_ecWeekend.ncols() == 1) {
			period = m_ecWeekend.at(0, 0);
		}
		else {
			period = m_ecWeekend.at(month - 1, hour - 1);
		}
	}
	return period;
}
