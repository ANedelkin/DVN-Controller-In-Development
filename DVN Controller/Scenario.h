#pragma once
#include "Global.h"
#include "BandInfo.h"
#include "BandPreset.h"
#include "DVNFileData.h"
#include "ModelConstants.h"

class Scenario : public DVNFileData {
private:
	array<BandInfo, BANDS_COUNT> bands;
public:
	Scenario();
	Scenario(string name);

	Status SetBandData(char i, string name, int startValue, int endValue, bool working);

	Status SetStartValue(char i, int value);
	Status SetEndValue(char i, int value);
	void CheckIfFull(char i);

	Status TurnOn(char i);
	void TurnOff(char i);
	string GetName(char i);
	Status Rename(string name, char i);

	int GetStartValue(char i) const;
	int GetEndValue(char i) const;
	int GetRangeIndex(char i) const;
	int GetStartValueBorder(char i) const;
	int GetEndValueBorder(char i) const;
	bool IsEnabled(char i) const;
	bool IsActive(char i) const;
	void Enable(char i);
	void Disable(char i);

	static Scenario* ToScenario(const string& name, stringstream& data);
	static vector<Scenario*> LoadScenarios();

	string BandSaveString(char i) const;

	string SaveString() const override;
	void Save() const override;
};