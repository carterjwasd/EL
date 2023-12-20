#include "../../win.h"

struct geolocation {
public:
	void get_geolocation();
public:
	std::string m_ip;
	std::string m_country;
	std::string m_country_flag;
	std::string m_country_capital;
	std::string m_country_phone;
	std::string m_country_code;
	std::string m_continent_code;
	std::string m_region;
	std::string m_latitude;
	std::string m_longitude;
	std::string m_org;
	std::string m_isp;
	std::string m_time_zone;
	std::string m_time_zone_name;
	std::string m_time_zone_gmt;
	std::string m_currency;
	std::string m_currency_code;
};

extern geolocation g_geolocation;