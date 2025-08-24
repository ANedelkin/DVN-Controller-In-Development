#pragma once

#include "Scenario.h"

const string Scenario::folder = "./scenarios";
const string Scenario::extension = ".dvns";
Scenario* Scenario::placeHolder = new Scenario();

Scenario::Scenario() : Scenario("Unnamed scenario") {}
Scenario::Scenario(string name) : DVNFileData(name) {
	DVNFileData::folder = folder;
	DVNFileData::extension = extension;
	int k = 0;
	for (int i = 0; i < BAND_RANGES_COUNT; i++) {
		BandInfo band = BandInfo(i, BAND_RANGES[i][0], BAND_RANGES[i][1]);
		for (int j = 0; j < BAND_RANGES[i][3]; j++)
		{
			bands[k] = band;
			bands[k].name = "Band ";
			bands[k].name += to_string(k + 1);
			k++;
		}
	}
	//oldSaveString = SaveString();
}

void Scenario::SetBandData(char i, string name, int startValue, int endValue, bool working)
{
	Rename(name, i);
	if (IsBandValid(i)) SetFreq(i, 0, startValue);
	if (IsBandValid(i)) SetFreq(i, 1, endValue);
	if (working) TurnOn(i);
	else TurnOff(i);
}

void Scenario::SetFreq(char bandIndex, char freqIndex, int value)
{
	string stat = "";
	
	if (value < GetStartValueBorder(bandIndex)) stat = ToString(StartValueOutOfBounds, GetStartValueBorder(bandIndex));
	else if (value > GetEndValueBorder(bandIndex)) stat = ToString(EndValueOutOfBounds, GetEndValueBorder(bandIndex));

	if (freqIndex) { //End frequency
		bands[bandIndex].endValue = value;

		if (stat.empty() && value < GetFreq(bandIndex, 0)) stat = ToString(EndValueLowerThanStartValue);
	}
	else { //Start frequency
		bands[bandIndex].startValue = value;

		if (stat.empty() && value > GetFreq(bandIndex, 1)) stat = ToString(StartValueHigherThanEndvalue);
	}

	SetBandStatus(bandIndex, (BandInfo::BandProperty)(freqIndex + 1), stat);
}

void Scenario::SetBandStatus(char i, BandInfo::BandProperty property, const string error)
{
	bool wasValid = IsBandValid(i);

	bands[i].errors[property] = error;

	if (!wasValid && IsBandValid(i))
		invalidBands--;
	else if (wasValid && !IsBandValid(i))
		invalidBands++;
}

string Scenario::GetBandStatus(char i, BandInfo::BandProperty property) {
	return bands[i].errors[property];
}

bool Scenario::IsBandValid(char i)
{
	return GetBandStatus(i, Name).empty() && GetBandStatus(i, Start).empty() && GetBandStatus(i, End).empty();
}

//void Scenario::CheckIfFull(char i)
//{
//	if (GetFreq(i, 0) == GetStartValueBorder(i) && GetFreq(i, 1) == GetEndValueBorder(i)) {
//		for (char j = 0; j < BANDS_COUNT; j++) {
//			if (GetRangeIndex(j) == GetRangeIndex(i) && j != i) Disable(j);
//		}
//	}
//}

void Scenario::TurnOn(char i) {
		bands[i].working = true;
}
void Scenario::TurnOff(char i) {
	bands[i].working = false;
}
string Scenario::GetName(char i) { return bands[i].name; }
void Scenario::Rename(const string& name, char i) {
	SetBandStatus(i, BandInfo::Name, BandInfo::ValidateName(name));
	bands[i].name = name;
}

int Scenario::GetFreq(char bandIndex, char freqIndex) const { return freqIndex ? bands[bandIndex].endValue : bands[bandIndex].startValue; }
int Scenario::GetRangeIndex(char i) const { return bands[i].rangeIndex; }
int Scenario::GetStartValueBorder(char i) const { return BAND_RANGES[bands[i].rangeIndex][0]; }
int Scenario::GetEndValueBorder(char i) const { return BAND_RANGES[bands[i].rangeIndex][1]; }
bool Scenario::IsEnabled(char i) const { return bands[i].enabled; }
bool Scenario::IsActive(char i) const { return bands[i].working; }
void Scenario::Enable(char i) {
	bands[i].enabled = true;
}
void Scenario::Disable(char i) {
	bands[i].enabled = false;
	bands[i].working = false;
}

string Scenario::ValidateNameUnique(const string& name)
{
	string stat = DVNFileData::ValidateName(name);
	if (!stat.empty()) return stat;

	if (ifstream(folder + "\\" + name + extension)) return ToString(ScenarioAlreadyExists, name.c_str());
	return ToString(Success);
}

Scenario* Scenario::ToScenario(const string& name, stringstream& stream, bool unique)
{
	Scenario* scenario = new Scenario(name);
	string bandString;
	if (!( unique ? Scenario::ValidateNameUnique(name) : Scenario::ValidateName(name) ).empty())
		goto NotOkay;
	for (int i = 0; i < GetBandsCount(); i++) {
		if (getline(stream, bandString)) {
			vector<string> values = Split(bandString, '|');
			if (values.size() == 4) {
				bool on;
				if (values[3] == "ON")
					on = true;
				else  if (values[3] == "OFF")
					on = false;
				else
					goto NotOkay;
				int start;
				int end;
				if (!Validation::TryParse(values[1], &start) || !Validation::TryParse(values[2], &end))
					goto NotOkay;
				else
					scenario->SetBandData(i, values[0], stoi(values[1]), stoi(values[2]), on);
			}
			else
				goto NotOkay;
		}
		else
			goto NotOkay;
	}
	scenario->oldSaveString = scenario->SaveString();
	return scenario;
NotOkay:
	scenario->ok = false;
	return scenario;
}
vector<Scenario*> Scenario::LoadScenarios()
{
	vector<Scenario*> output;
	if (exists(folder)) {
		directory_iterator dirItr(folder);
		for (const auto& dir : dirItr) {
			path path = dir.path();
			ifstream stream(path);
			stringstream data;
			data << stream.rdbuf();
			const string name = path.stem().string();
			Scenario* scenario = ToScenario(name, data);
			output.push_back(scenario);
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

DVNFileData* Scenario::Copy() const
{
	Scenario* scen = new Scenario(*this);
	scen->oldSaveString = "";
	return scen;
}
