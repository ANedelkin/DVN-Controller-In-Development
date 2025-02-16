#pragma once

#include "Scenario.h"

Scenario::Scenario() : Scenario("Unnamed scenario") {}
Scenario::Scenario(string name) : DVNFileData("./scenarios/", ".dvns") {
	this->name = name;
	int k = 0;
	for (int i = 0; i < BAND_RANGES_COUNT; i++) {
		BandInfo band = BandInfo(i, -1, -1);
		for (int j = 0; j < bandRanges[i][3]; j++)
		{
			bands[k] = band;
			k++;
		}
	}
}

Status Scenario::SetStartValue(char i, int value)
{
	if (value > GetEndValue(i) && GetEndValue(i) != -1) return StartValueHigherThanEndvalue;
	if (value > GetEndValueBorder(i) || value < GetStartValueBorder(i)) return StartValueOutOfBounds;
	
	bands[i].startValue = value;
	CheckIfFull(i);
	return Success;
}

Status Scenario::SetEndValue(char i, int value)
{
	if (value < GetStartValue(i)) return StartValueHigherThanEndvalue;
	if (value > GetEndValueBorder(i) || value < GetStartValueBorder(i)) return EndValueOutOfBounds;

	bands[i].endValue = value;
	CheckIfFull(i);
	return Success;
}

void Scenario::CheckIfFull(char i)
{
	if (GetStartValue(i) == GetStartValueBorder(i) && GetEndValue(i) == GetEndValueBorder(i)) {
		for (char j = 0; j < BANDS_COUNT; j++) {
			if (GetRangeIndex(j) == GetRangeIndex(i) && j != i) Disable(j);
		}
	}
}

Status Scenario::TurnOn(char i) {
	if (bands[i].startValue != -1 && bands[i].endValue != -1) {
		bands[i].working = true;
		return Success;
	}
	return BandUninitialized;
}
void Scenario::TurnOff(char i) {
	bands[i].working = false;
}
string Scenario::GetName(char i) { return bands[i].name; }
Status Scenario::Rename(string name, char i) {
	if (name.length() == 0) return NameWhitespace;
	if (all_of(name.begin(), name.end(), [](unsigned char c) { return std::isspace(c); })) return NameWhitespace;

	bands[i].name = name;
	return Success;
}

int Scenario::GetStartValue(char i) const { return bands[i].startValue; }
int Scenario::GetEndValue(char i) const { return bands[i].endValue; }
int Scenario::GetRangeIndex(char i) const { return bands[i].rangeIndex; }
int Scenario::GetStartValueBorder(char i) const { return bandRanges[bands[i].rangeIndex][0]; }
int Scenario::GetEndValueBorder(char i) const { return bandRanges[bands[i].rangeIndex][1]; }
bool Scenario::IsEnabled(char i) const { return bands[i].enabled; }
bool Scenario::IsActive(char i) const { return bands[i].working; }
void Scenario::Enable(char i) {
	bands[i].enabled = true;
}
void Scenario::Disable(char i) {
	bands[i].enabled = false;
	bands[i].working = false;
}

string Scenario::BandSaveString(char i) const {
	ostringstream stream;
	stream << bands[i].name << "|" << bands[i].startValue << "|" << bands[i].endValue << "|" << (bands[i].working ? "ON" : "OFF");
	return stream.str();
}
void Scenario::SaveBand(char i) const {
	if (!exists("./bands")) create_directory("./bands");
	time_t now = system_clock::to_time_t(system_clock::now());
	tm now_tm;
	localtime_s(&now_tm, &now);
	int year = now_tm.tm_year % 100;
	ostringstream fileName;
	fileName << "./bands/band_" << setfill('0') << setw(2) << year << put_time(&now_tm, "%m%d%H%M%S");
	auto ms_part = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) % 1000;
	fileName << setfill('0') << setw(3) << ms_part.count() << ".dvnb";
	ofstream stream(fileName.str());
	stream << BandSaveString(i);
	stream.close();
}

string Scenario::SaveString() const {
	ostringstream stream;
	stream << name;
	for (char i = 0; i < bands.size(); i++)
	{
		stream << endl << BandSaveString(i);
	}
	return stream.str();
}