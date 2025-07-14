#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>

#include "Load.h"

#include "ftd2xx.h"
#include <wx/stopwatch.h>

using namespace std;

class LoadTransferProgressFrame;

class JammersManager
{
private:
	static FT_HANDLE OpenConnection(string serialNumber);
public:
	static vector<string> GetJammers();
	static bool SendLoad(string serialNumber, Load* load, LoadTransferProgressFrame*);
	static Load* GetLoad(string serialNumber);
};

