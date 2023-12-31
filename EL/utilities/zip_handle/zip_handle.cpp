#include "zip_handle.h"

#include "../tools/tools.h"
#include "../globals/globals.h"
#include "../directories/directories.h"

#include <fstream>

zip_handler g_zip;

void zip_handler::load_zip()
{
	std::ofstream zip(g_globals.zip_path.c_str());
	zip.close();

	zip_handle = zipOpen(g_globals.zip_path.c_str(), APPEND_STATUS_ADDINZIP);
}

void zip_handler::write_zip(std::string file, std::string buf)
{
	zipOpenNewFileInZip(g_zip.zip_handle, file.c_str(), &g_zip.zip_file_info,
		NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);

	zipWriteInFileInZip(g_zip.zip_handle, buf.c_str(), buf.length());
	zipCloseFileInZip(g_zip.zip_handle);
}

void zip_handler::close_zip()
{
	std::stringstream comment;

	const char banner[] =
	{
		0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
		0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F,
		0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x0D, 0x0A, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x5F, 0x5F,
		0x20, 0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
		0x5F, 0x5F, 0x5F, 0x28, 0x5F, 0x29, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
		0x5F, 0x20, 0x0D, 0x0A, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x5F, 0x2F, 0x20, 0x20, 0x5F,
		0x5F, 0x20, 0x20, 0x2F, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x5F, 0x20, 0x20, 0x5F,
		0x5F, 0x5F, 0x2F, 0x5F, 0x20, 0x20, 0x2F, 0x5F, 0x5F, 0x20, 0x7C, 0x20, 0x2F, 0x20, 0x2F, 0x20,
		0x20, 0x5F, 0x20, 0x5C, 0x0D, 0x0A, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x20,
		0x20, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x2F, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x2F, 0x5F, 0x28, 0x5F,
		0x5F, 0x20, 0x20, 0x29, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x5F, 0x5F, 0x20, 0x7C, 0x2F, 0x20, 0x2F,
		0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x2F, 0x0D, 0x0A, 0x20, 0x20, 0x2F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
		0x2F, 0x20, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x2F,
		0x5F, 0x5F, 0x5F, 0x5F, 0x2F, 0x20, 0x2F, 0x5F, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F,
		0x2F, 0x20, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F
	};

	comment << XOR_STRING("\r\n");
	for (size_t i = 0; i < sizeof(banner) / sizeof(banner[0]); ++i) {
		comment << banner[i];
	}
	comment << XOR_STRING("\r\n");
	comment << XOR_STRING("\r\n");

	comment << std::string(2, ' ') << XOR_STRING("~ To see more info, open ") << g_tools.instruction_file.c_str() << XOR_STRING("\r\n");
	comment << std::string(2, ' ') << XOR_STRING(WATERMARK) << XOR_STRING("\r\n");

	zipClose(zip_handle, comment.str().c_str());
}