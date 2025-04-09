#pragma once
#include "Global.h"
#include "BandInfo.h"
#include "DVNFileData.h"
#include "ModelConstants.h"

class Scenario : public DVNFileData {
private:
	array<BandInfo, GetBandsCount()> bands;

	static const string folder;
	static const string extension;
public:
	Scenario();
	Scenario(string name);

	Status SetBandData(char i, string name, int startValue, int endValue, bool working);
	Status SetFreq(char bandIndex, char freqIndex, int value);

	Status TurnOn(char i);
	void TurnOff(char i);
	string GetName(char i);
	Status Rename(const string& name, char i);

	int GetFreq(char bandIndex, char freqIndex) const;
	int GetRangeIndex(char i) const;
	int GetStartValueBorder(char i) const;
	int GetEndValueBorder(char i) const;
	bool IsEnabled(char i) const;
	bool IsActive(char i) const;
	void Enable(char i);
	void Disable(char i);

	static Status ValidateNameUnique(const string& name);

	static Scenario* ToScenario(const string& name, stringstream& data);
	static vector<Scenario*> LoadScenarios();

	string BandSaveString(char i) const;

	string SaveString() const override;
};