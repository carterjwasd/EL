#include "geolocation.h"

#include "../../winapi/winapi.h"

#include "../../utilities/xor/xor.h"
#include "../../utilities/tools/tools.h"

#include "../../depository/json/json.h"

#include <sstream>
#include <fstream>

geolocation g_geolocation;

void geolocation::get_geolocation()
{
	const std::string url(XOR_STRING("http://ipwhois.app/json/"));

	HINTERNET net = g_winapi.InternetOpenA(g_tools.random_string(15, XOR_STRING("abcdefghijklmnopqrstuvwxyz")).c_str(),
		0, 0, 0, 0);
	HINTERNET conn = g_winapi.InternetOpenUrlA(net, url.c_str(),
		0, 0, 0x80000000, 0);

	char buffer[4096];
	DWORD read;

	g_winapi.InternetReadFile(conn, buffer, sizeof(buffer) / sizeof(buffer[0]), &read);
	g_winapi.InternetCloseHandle(net);

	std::string data = std::string(buffer, read);

	Json::Value json_data;
	Json::Reader json_reader;

	if (json_reader.parse(data, json_data))
	{
		if (json_data[XOR_STRING("ip")])
			m_ip = json_data[XOR_STRING("ip")].asString();

		if (json_data[XOR_STRING("country")])
			m_country = json_data[XOR_STRING("country")].asString();

		if (json_data[XOR_STRING("country_flag")])
			m_country_flag = json_data[XOR_STRING("country_flag")].asString();

		if (json_data[XOR_STRING("country_capital")])
			m_country_capital = json_data[XOR_STRING("country_capital")].asString();

		if (json_data[XOR_STRING("country_phone")])
			m_country_phone = json_data[XOR_STRING("country_phone")].asString();

		if (json_data[XOR_STRING("country_code")])
			m_country_code = json_data[XOR_STRING("country_code")].asString();

		if (json_data[XOR_STRING("continent_code")])
			m_continent_code = json_data[XOR_STRING("continent_code")].asString();

		if (json_data[XOR_STRING("region")])
			m_region = json_data[XOR_STRING("region")].asString();

		if (json_data[XOR_STRING("latitude")])
			m_latitude = json_data[XOR_STRING("latitude")].asString();

		if (json_data[XOR_STRING("longitude")])
			m_longitude = json_data[XOR_STRING("longitude")].asString();

		if (json_data[XOR_STRING("org")])
			m_org = json_data[XOR_STRING("org")].asString();

		if (json_data[XOR_STRING("isp")])
			m_isp = json_data[XOR_STRING("isp")].asString();

		if (json_data[XOR_STRING("timezone")])
			m_time_zone = json_data[XOR_STRING("timezone")].asString();

		if (json_data[XOR_STRING("timezone_name")])
			m_time_zone_name = json_data[XOR_STRING("timezone_name")].asString();

		if (json_data[XOR_STRING("timezone_gmt")])
			m_time_zone_gmt = json_data[XOR_STRING("timezone_gmt")].asString();

		if (json_data[XOR_STRING("currency")])
			m_currency = json_data[XOR_STRING("currency")].asString();

		if (json_data[XOR_STRING("currency_code")])
			m_currency_code = json_data[XOR_STRING("currency_code")].asString();
	}
}