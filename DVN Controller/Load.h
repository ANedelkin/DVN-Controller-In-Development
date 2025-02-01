#pragma once
#include "Global.h"
#include "Scenario.h"
#include "ScenarioPreset.h"
#include "ModelConstants.h"

class Load {
public:
	string name;
	vector<Scenario> scenarios;

	Load(string name) {
		this->name = name;
	}

	string SaveString() {
		ostringstream stream;
		stream << name;
		for (const Scenario& scenario : scenarios) stream << endl << scenario.SaveString();
		return stream.str();
	}

	static Status ValidateName(string& name) {
		string invalidChars = "\\/:*?\"<>|";
		for (char ch : name) {
			if (invalidChars.find(ch) != string::npos) {
				return InvalidSymbols;
			}
		}
		if (all_of(name.begin(), name.end(), [](unsigned char c) { return std::isspace(c); })) return NameWhitespace;
		return Success;
	}

	static Load ToLoad(string params) {
		stringstream stream(params);
		Load load("");
		getline(stream, load.name);
		string scenario;
		for (char i = 0; i < bandsCount && getline(stream, scenario); i++) {
			ScenarioPreset preset = ScenarioPreset::ToScenarioPreset(scenario);
			for (char j = 0; j < bandsCount; j++)
			{
				load.scenarios.push_back(Scenario(preset.name));
				load.scenarios[i].SetBandValues(j, preset.bands[j].startValue, preset.bands[j].endValue);
			}
		}
	}
	void Save(string path) {
		ofstream stream(path);
		stream << this->SaveString();
		stream.close();
	}
};