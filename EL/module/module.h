#include "../win.h"

struct module {
public:
	void run();
	void delete_temporary_files();
private:
	void anti_cis();
};

extern module g_module;