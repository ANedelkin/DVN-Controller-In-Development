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
	static bool SendLoad(string serialNumber, Load* load, function<bool(int, const wxString&)>);
	static bool GetLoad(string serialNumber, Load* output, vector<tuple<char, char>>* brokenBands, function<bool(int, const wxString&)>);
};

