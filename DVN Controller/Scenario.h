#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <array>

#include "Global.h"
#include "BandInfo.h"
#include "DVNFileData.h"
#include "ModelConstants.h"
#include "Split.h"

using namespace Status;
using BandInfo::BandProperty::Name;
using BandInfo::BandProperty::Start;
using BandInfo::BandProperty::End;

class Scenario : public DVNFileData {
private:
	array<BandInfo, GetBandsCount()> bands;

	static const string folder;
	static const string extension;
public:
	static Scenario* placeHolder;

	char invalidBands = 0;

	Scenario();
	Scenario(string name);

	void SetBandData(char i, string name, int startValue, int endValue, bool working);
	void SetFreq(char bandIndex, char freqIndex, int value);

	void SetBandStatus(char i, BandInfo::BandProperty property, const string error);
	string GetBandStatus(char i, BandInfo::BandProperty property);
	bool IsBandValid(char i);

	void TurnOn(char i);
	void TurnOff(char i);
	string GetName(char i);
	void Rename(const string& name, char i);

	int GetFreq(char bandIndex, char freqIndex) const;
	int GetRangeIndex(char i) const;
	int GetStartValueBorder(char i) const;
	int GetEndValueBorder(char i) const;
	bool IsEnabled(char i) const;
	bool IsActive(char i) const;
	void Enable(char i);
	void Disable(char i);

	static string ValidateNameUnique(const string& name);

	static Scenario* ToScenario(const string& name, stringstream& data, bool unique = false);
	static vector<Scenario*> LoadScenarios();

	string BandSaveString(char i) const;

	string SaveString() const override;

	DVNFileData* Copy() const override;
};