#pragma once

#include "../../win.h"

struct gaming {
public:
	void run();
private:
	void steam(std::string path);
	void vimeworld(std::string path);
	void osu(std::string user_name, std::string path);
	void minecraft(std::string path);
};

extern gaming g_gaming;