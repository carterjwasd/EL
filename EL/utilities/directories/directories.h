#pragma once

#include <iostream>

struct dir {
public:
	std::string get_user_dir();
	std::string get_appdata();
	std::string get_local_appdata();
	std::string get_roaming_appdata();
	std::string get_temp();
	std::string get_dekstop();
	std::string get_windows();
	std::string get_program_files();
	std::string get_program_files86();
	std::string get_program_data();
	std::string get_system32();
	std::string get_documents();
	std::string get_downloads();
	std::string get_images();
};

extern dir g_dir;