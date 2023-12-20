#include <iostream>
#include <minizip/zip.h>

struct zip_handler {
public:
	zip_f zip_handle;
	zip_fileinfo zip_file_info;

	void load_zip();
	void write_zip(std::string file, std::string buf);
	void close_zip();
};

extern zip_handler g_zip;