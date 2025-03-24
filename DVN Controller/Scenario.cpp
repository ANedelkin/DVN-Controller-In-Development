#pragma once

#include "Scenario.h"

Scenario::Scenario() : Scenario("Unnamed scenario") {}
Scenario::Scenario(string name) : DVNFileData(name) {
	this->folder = "./scenarios";
	this->extension = ".dvns";
	int k = 0;
	for (int i = 0; i < BAND_RANGES_COUNT; i++) {
		BandInfo band = BandInfo(i, bandRanges[i][0], bandRanges[i][1]);
		for (int j = 0; j < bandRanges[i][3]; j++)
		{
			bands[k] = band;
			bands[k].name = "Band ";
			bands[k].name += to_string(k + 1);
			k++;
		}
	}
	oldSaveString = SaveString();
}

Status Scenario::SetBandData(char i, string name, int startValue, int endValue, bool working)
{
	Status stat = Success;
	stat = Rename(name, i);
	if (!stat) stat = SetFreq(i, 0, startValue);
	if (!stat) stat = SetFreq(i, 1, endValue);
	if (!stat) {
		if (working) stat = TurnOn(i);
		else TurnOff(i);
	}
	return stat;
}

//Status Scenario::SetStartValue(char i, int value)
//{
//	if (value == GetStartValue(i)) return Success;
//	if (value > GetEndValue(i)) return StartValueHigherThanEndvalue;
//	if (value < GetStartValueBorder(i)) return StartValueOutOfBounds;
//	
//	bands[i].startValue = value;
//	//CheckIfFull(i);
//	return Success;
//}
//
//Status Scenario::SetEndValue(char i, int value)
//{
//	if (value == GetEndValue(i)) return Success;
//	if (value < GetStartValue(i)) return StartValueHigherThanEndvalue;
//	if (value > GetEndValueBorder(i)) return EndValueOutOfBounds;
//
//	bands[i].endValue = value;
//	//CheckIfFull(i);
//	return Success;
//}

Status Scenario::SetFreq(char bandIndex, char freqIndex, int value)
{
	if (freqIndex) {
		if (value == GetFreq(bandIndex, 1)) return Success;
		if (value < GetFreq(bandIndex, 0)) return StartValueHigherThanEndvalue;
		if (value > GetEndValueBorder(bandIndex)) return EndValueOutOfBounds;

		bands[bandIndex].endValue = value;
	}
	else {
		if (value == GetFreq(bandIndex, 0)) return Success;
		if (value > GetFreq(bandIndex, 1)) return StartValueHigherThanEndvalue;
		if (value < GetStartValueBorder(bandIndex)) return StartValueOutOfBounds;

		bands[bandIndex].startValue = value;
	}
	return Success;
}

//void Scenario::CheckIfFull(char i)
//{
//	if (GetFreq(i, 0) == GetStartValueBorder(i) && GetFreq(i, 1) == GetEndValueBorder(i)) {
//		for (char j = 0; j < BANDS_COUNT; j++) {
//			if (GetRangeIndex(j) == GetRangeIndex(i) && j != i) Disable(j);
//		}
//	}
//}

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
	if (name.find('|') != string::npos) return InvalidSymbols;

	bands[i].name = name;
	return Success;
}

//int Scenario::GetStartValue(char i) const { return bands[i].startValue; }
//int Scenario::GetEndValue(char i) const { return bands[i].endValue; }
int Scenario::GetFreq(char bandIndex, char freqIndex) const { return freqIndex ? bands[bandIndex].endValue : bands[bandIndex].startValue; }
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

Scenario* Scenario::ToScenario(const string& name, stringstream& stream)
{
	Scenario* scenario = new Scenario(name);
	string bandString;
	for (int i = 0; i < BANDS_COUNT && getline(stream, bandString); i++) {
		vector<string> values = Split(bandString, '|');
		BandInfo band(values[0], scenario->GetRangeIndex(i), stoi(values[1]), stoi(values[2]), values[3] == "ON");
		scenario->SetBandData(i, band.name, band.startValue, band.endValue, band.working);
	}
	scenario->oldSaveString = scenario->SaveString();
	return scenario;
}
vector<Scenario*> Scenario::LoadScenarios()
{
	vector<Scenario*> output;
	if (filesystem::exists("./scenarios")) {
		directory_iterator dirItr("./scenarios");
		for (const auto& scenario : dirItr) {
			ifstream stream(scenario.path());
			stringstream data;
			data << stream.rdbuf();
			const string name = scenario.path().filename().string();
			output.push_back(ToScenario(name.substr(0, name.length() - 5), data));
		}
	}
	return output;
}

string Scenario::BandSaveString(char i) const {
	ostringstream stream;
	stream << bands[i].name << "|" << bands[i].startValue << "|" << bands[i].endValue << "|" << (bands[i].working ? "ON" : "OFF");
	return stream.str();
}

string Scenario::SaveString() const {
	ostringstream stream;
	for (char i = 0; i < bands.size(); i++)
	{
		stream << BandSaveString(i);
		if (i != bands.size() - 1) stream << endl;
	}
	return stream.str();
}