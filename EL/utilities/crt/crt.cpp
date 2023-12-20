#include "crt.h"

crt g_crt;

char* crt::strstr(const char *str, const char *sub_string)
{
	const char *a, *b = sub_string;

	if (!*b) return (char *)str;
	for (; *str; str++) {
		if (*str != *b) continue;
		a = str;
		for (;;) {
			if (!*b) return (char *)str;
			if (*a++ != *b++) break;
		}
		b = sub_string;
	}
	return 0;
}

int crt::strcmp(const char *str, const char *sub_string)
{
	while (*str && *str == *sub_string)
	{
		++str;
		++sub_string;
	}

	return (unsigned char)*str - (unsigned char)*sub_string;
}

char* crt::strcpy(char *str, const char *sub_string)
{
	// Use unsigned char as all str functions behave as if char was unsigned
	// Also need to save s1 for return

	unsigned char *us1 = (unsigned char*)str;
	const unsigned char *us2 = (const unsigned char*)sub_string;

	while (*us2) {
		*us1++ = *us2++;
	}

	return str;
}

int crt::floor(double x) {
	if (x > 0.0)
		return (int)x;
	else
		return (int)(x - 0.5);
}

int crt::atoi(const char* str)
{
	// Initialize result
	int res = 0;

	// Iterate through all characters
	// of input string and update result
	// take ASCII character of corresponding digit and
	// subtract the code from '0' to get numerical
	// value and multiply res by 10 to shuffle
	// digits left to update running total

	for (int i = 0; str[i] != '\0'; ++i)
		res = res * 10 + str[i] - '0';

	// return result.
	return res;
}