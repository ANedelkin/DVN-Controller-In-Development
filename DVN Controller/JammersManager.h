#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>

#include "Load.h"

#include "ftd2xx.h";

using namespace std;

class JammersManager
{
private:
	static FT_HANDLE OpenConnection(string serialNumber);
public:
	static vector<string> GetJammers();
	static bool SendLoad(string serialNumber, Load* load);
	static Load* GetLoad(string serialNumber);
};

