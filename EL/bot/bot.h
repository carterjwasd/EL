#pragma once

#include "../win.h"

struct tgbot {
public:
	void send_details(std::string bot_token, std::string telegram_id);
};

extern tgbot g_tgbot;