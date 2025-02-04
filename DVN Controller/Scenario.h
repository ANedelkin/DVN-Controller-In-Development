#pragma once
#include "Global.h"
#include "BandInfo.h"
#include "BandPreset.h"
#include "ModelConstants.h"

class Scenario {
private:
	array<BandInfo, BANDS_COUNT> bands;
	string name;
public:
	Scenario() : Scenario("Unnamed scenario") {}
	Scenario(string name) {
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

	Status SetBandValues(char i, int startValue, int endValue) {
		if (endValue != -1 && startValue > endValue) return StartValueHigherThanEndvalue;
		if (startValue < GetStartValueBorder(i) || startValue > GetEndValueBorder(i)) return StartValueOutOfBounds;
		if (endValue > GetEndValueBorder(i)) return EndValueOutOfBounds;
		if (endValue != -1 && endValue < startValue) return EndValueLowerThanStartValue;

		int invalidValue = bandRanges[GetRangeIndex(i)][2];

		if (startValue == GetStartValueBorder(i) && endValue == GetEndValueBorder(i)) {
			for (char j = 0; j < BANDS_COUNT; j++) {
				if (GetRangeIndex(j) == GetRangeIndex(i) && j != i) Disable(j);
			}
			goto success;
		}

		//if (GetStartValue(i) < invalidValue && GetEndValue(i) > invalidValue) {
		//	if (startValue > invalidValue || endValue < invalidValue) {
		//		int rangeEnd = BANDS_COUNT - 1;
		//		for (; rangeEnd > i && (GetRangeIndex(rangeEnd) != GetRangeIndex(i)); rangeEnd--);
		//		bool f = true;
		//		for (char j = i + 1; j <= rangeEnd; j++) {
		//			if (IsEnabled(j) && (f && (GetStartValue(j) != -1 || GetEndValue(j) != -1) || !f)) {
		//				Enable(j);
		//				break;
		//			}
		//			if (j == rangeEnd) {
		//				f = false;
		//				j = i + 1;
		//			}
		//		}
		//	}
		//}
		//else if (startValue < invalidValue && endValue > invalidValue) {
		//	int rangeEnd = BANDS_COUNT - 1;
		//	for (; rangeEnd > i && (GetRangeIndex(rangeEnd) != GetRangeIndex(i)); rangeEnd--);
		//	if (rangeEnd == BANDS_COUNT - 1) return BandAtLastPlace;
		//	bool f = true;
		//	for (char j = rangeEnd; j > i; j--) {
		//		if (IsEnabled(j) && (f && (GetStartValue(j) == -1) && (GetEndValue(j) == -1) || !f)) {
		//			Disable(j);
		//			break;
		//		}
		//		if (j == i + 1) {
		//			f = false;
		//			j = rangeEnd;
		//		}
		//	}
		//}
	success:
		bands[i].startValue = startValue;
		bands[i].endValue = endValue;
		return Success;
	}

	void TurnOn(char i) {
		bands[i].working = true;
	}
	void TurnOff(char i) {
		bands[i].working = false;
	}
	string GetName() { return name; }
	string GetName(char i) { return bands[i].name; }
	Status Rename(string name) {
		this->name = name;
		return Success;
	}
	Status Rename(string name, char i) {
		if (name.length() == 0) return NameWhitespace;
		if (all_of(name.begin(), name.end(), [](unsigned char c) { return std::isspace(c); })) return NameWhitespace;

		bands[i].name = name;
		return Success;
	}

	int GetStartValue(char i) const { return bands[i].startValue; }
	int GetEndValue(char i) const { return bands[i].endValue; }
	int GetRangeIndex(char i) const { return bands[i].rangeIndex; }
	int GetStartValueBorder(char i) const { return bandRanges[bands[i].rangeIndex][0]; }
	int GetEndValueBorder(char i) const { return bandRanges[bands[i].rangeIndex][1]; }
	bool IsEnabled(char i) const { return bands[i].enabled; }
	bool IsActive(char i) const { return bands[i].working; }
	void Enable(char i) {
		bands[i].enabled = true;
	}
	void Disable(char i) {
		bands[i].enabled = false;
		bands[i].working = false;
	}

	static Status ValidateName(string& name) {
		string invalidChars = "\\/:*?\"<>|";
		for (char ch : name) {
			if (invalidChars.find(ch) != string::npos) {
				return InvalidSymbols;
			}
		}
		if (name.length() == 0) return NameWhitespace;
		if (all_of(name.begin(), name.end(), [](unsigned char c) { return std::isspace(c); })) return NameWhitespace;
		return Success;
	}

	string BandSaveString(char i) const {
		ostringstream stream;
		stream << name << "|" << bands[i].startValue << "|" << bands[i].endValue << "|";
		return stream.str();
	}
	void SaveBand(char i) const {
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

	string SaveString() const {
		ostringstream stream;
		stream << name;
		for (char i = 0; i < bands.size(); i++)
		{
			stream << endl << BandSaveString(i);
		}
		return stream.str();
	}
	void Save() const  {
		if (!exists("./scenarios")) create_directory("./scenarios");
		time_t now = system_clock::to_time_t(system_clock::now());
		tm now_tm;
		localtime_s(&now_tm, &now);
		int year = now_tm.tm_year % 100;
		ostringstream fileName;
		fileName << "./scenarios/scenario_" << setfill('0') << setw(2) << year << put_time(&now_tm, "%m%d%H%M%S");
		auto ms_part = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) % 1000;
		fileName << setfill('0') << setw(3) << ms_part.count() << ".dvns";
		ofstream stream(fileName.str());
		stream << this->SaveString();
		stream.close();
	}
};