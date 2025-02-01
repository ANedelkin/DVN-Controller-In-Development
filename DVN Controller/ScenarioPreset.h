#pragma once
#include "Global.h"
#include "BandPreset.h"

struct ScenarioPreset {
	string name;
	array<BandPreset, bandsCount> bands;

	static ScenarioPreset ToScenarioPreset(string params) {
		ScenarioPreset scenario;
		stringstream stream(params);
		getline(stream, scenario.name);
		string band;
		for (int i = 0; i < bandsCount && getline(stream, band); i++) {
			BandPreset preset = BandPreset::ToBandPreset(band);
			scenario.bands[i].startValue = preset.startValue;
			scenario.bands[i].endValue = preset.endValue;
		}
	}
	static vector<ScenarioPreset> LoadScenarioPresets() {
		vector<ScenarioPreset> output;
		directory_iterator dirItr("./scenarios");
		for (const auto& scenario : dirItr) {
			ifstream stream(scenario.path());
			string info;
			getline(stream, info);
			output.push_back(ToScenarioPreset(info));
		}
		return output;
	}
};