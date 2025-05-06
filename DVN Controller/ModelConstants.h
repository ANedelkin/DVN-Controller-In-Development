#pragma once

#define JAMMER_NAME "BJ130P"
#define VERSION "1.0"

#define SCENARIOS_COUNT 6

#define BAND_RANGES_COUNT 6

constexpr int BAND_RANGES[BAND_RANGES_COUNT][4] = {
	{20, 350, -1, 3},
	{350, 500, -1, 3},
	{500, 2000, -1, 6},
	{2000, 3000, -1, 6},
	{3000, 4000, -1, 3},
	{4000, 6000, -1, 3},
};

consteval char GetBandsCount() {
	char count = 0;
	for (char i = 0; i < BAND_RANGES_COUNT; i++)
	{
		count += BAND_RANGES[i][3];
	}
	return count;
}

consteval char cstrlen(const char* str) {
	char n = 0;
	while (str[n])
		n++;
	return n;
}