#include "directories.h"

#include "../tools/tools.h"
#include "../xor/xor.h"

dir g_dir;

std::string dir::get_user_dir() {
	std::string system_disk = g_tools.get_system_disk();
	std::string user_name = g_tools.get_user_name();

	return system_disk.c_str() + std::string(XOR_STRING("\\Users\\")) + user_name.c_str() + std::string(XOR_STRING("\\"));
}

std::string dir::get_appdata() {
	return get_user_dir().c_str() + std::string(XOR_STRING("AppData\\"));
}

std::string dir::get_local_appdata() {
	return get_appdata().c_str() + std::string(XOR_STRING("Local\\"));
}

std::string dir::get_roaming_appdata() {
	return get_appdata().c_str() + std::string(XOR_STRING("Roaming\\"));
}

std::string dir::get_temp() {
	return get_local_appdata().c_str() + std::string(XOR_STRING("Temp\\"));
}

std::string dir::get_dekstop() {
	std::string system_disk = g_tools.get_system_disk();
	std::string user_name = g_tools.get_user_name();

	return system_disk.c_str() + std::string(XOR_STRING("\\Users\\")) + user_name.c_str() + std::string(XOR_STRING("\\Desktop\\"));
}

std::string dir::get_windows() {
	std::string system_disk = g_tools.get_system_disk();
	return system_disk.c_str() + std::string(XOR_STRING("\\Windows\\"));
}

std::string dir::get_program_files() {
	std::string system_disk = g_tools.get_system_disk();
	return system_disk.c_str() + std::string(XOR_STRING("\\Program Files\\"));
}

std::string dir::get_program_files86() {
	std::string system_disk = g_tools.get_system_disk();
	return system_disk.c_str() + std::string(XOR_STRING("\\Program Files (x86)\\"));
}

std::string dir::get_program_data() {
	std::string system_disk = g_tools.get_system_disk();
	return system_disk.c_str() + std::string(XOR_STRING("\\ProgramData\\"));
}

std::string dir::get_system32() {
	return get_windows().c_str() + std::string(XOR_STRING("System32\\"));
}

std::string dir::get_documents() {
	return get_user_dir() + std::string(XOR_STRING("Documents\\"));
}

std::string dir::get_downloads() {
	return get_user_dir() + std::string(XOR_STRING("Downloads\\"));
}

std::string dir::get_images() {
	return get_user_dir() + std::string(XOR_STRING("Pictures\\"));
}