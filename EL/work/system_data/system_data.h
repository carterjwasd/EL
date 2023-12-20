#pragma once

#include "../../win.h"

struct system_data {
public:
	void run();
private:
	void get_info_about_user();
	void get_clipboard_data();
	void get_process_list();
};

extern system_data g_system_data;