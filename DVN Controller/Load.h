#pragma once
#include "Global.h"
#include "Scenario.h"

#include "ScenarioPreset.h"
#include "ModelConstants.h"

class Load : public DVNFileData {
public:
	Load() : Load("Unnamed load") {}
	Load(string name) : DVNFileData("./loads/", ".dvnl") {
		this->name = name;
		for (char i = 0; i < SCENARIOS_COUNT; i++)
		{
			children.push_back(new Scenario());
		}
	}

	string GetName() { return name; }

	string SaveString() const override {
		ostringstream stream;
		stream << name;
		for (auto& scenario : children) stream << endl << scenario->SaveString();
		return stream.str();
	}

	static Load ToLoad(string params) {
		//stringstream stream(params);
		//Load load("");
		//getline(stream, load.name);
		//string scenario;
		//for (char i = 0; i < BANDS_COUNT && getline(stream, scenario); i++) {
		//	ScenarioPreset preset = ScenarioPreset::ToScenarioPreset(scenario);
		//	for (char j = 0; j < BANDS_COUNT; j++)
		//	{
		//		load.children.push_back(Scenario(preset.name));
		//		load.children[i].SetBandValues(j, preset.bands[j].startValue, preset.bands[j].endValue);
		//	}
		//}
		return Load();
	}
};