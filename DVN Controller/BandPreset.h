#pragma once
#include "Global.h"

struct BandPreset {
	string name;
	int startValue;
	int endValue;

	BandPreset():BandPreset("", -1, -1){}
	BandPreset(const string& name, int startValue, int endvalue) {
		this->name = name;
		this->startValue = startValue;
		this->endValue = endvalue;
	}

	static BandPreset ToBandPreset(string params) {
		vector<string> values = Split(params, '|');
		return BandPreset(values[0], stoi(values[1]), stoi(values[2]));
	}
	static vector<BandPreset> LoadBandPresets() {
		vector<BandPreset> output;
		directory_iterator dirItr("./bands");
		for (const auto& band : dirItr) {
			ifstream stream(band.path());
			string info;
			getline(stream, info);
			output.push_back(ToBandPreset(info));
		}
		return output;
	}
};