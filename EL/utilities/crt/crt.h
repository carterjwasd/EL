#pragma once

#include "../../win.h"

struct crt {
public:
	char* strstr(const char *str, const char *sub_string);
	int strcmp(const char *str, const char *sub_string);
	char* strcpy(char *str, const char *sub_string);
	int floor(double x);
	int atoi(const char* str);
};

extern crt g_crt;