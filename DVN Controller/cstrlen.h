#pragma once

consteval char cstrlen(const char* str) {
	char n = 0;
	while (str[n])
		n++;
	return n;
}