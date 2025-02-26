#pragma once
#include "Global.h"
#include "Scenario.h"

#include "ScenarioPreset.h"
#include "ModelConstants.h"

class Load : public DVNFileData {
public:
	Load() : Load("Unnamed load") {}
	Load(const string& name) : Load(name, "") {}
	Load(const string& name, const string& folder) : DVNFileData(name) {
		this->extension = ".dvnl";
		this->folder = folder;
		for (char i = 0; i < SCENARIOS_COUNT; i++)
		{
			children.push_back(new Scenario());
		}
	}

	string GetName() { return name; }

	string SaveString() const override {
		ostringstream stream;
		for (char i = 0; i < children.size(); i++)
		{
			stream << children[i]->GetName() << endl << children[i]->SaveString();
			if (i != children.size() - 1) stream << endl;
		}
		return stream.str();
	}

	void Save() const override {
		if (!exists(folder)) create_directory(folder);
		ostringstream fileName;
		fileName << GetPath();
		ofstream stream(fileName.str());
		stream << this->SaveString();
		stream.close();
	}

	static Load* ToLoad(const string& name, const string& folder, stringstream& data) {
		Load* load = new Load(name, folder);
		for (char i = 0; i < SCENARIOS_COUNT; i++)
		{
			string scenName;
			getline(data, scenName);
			Scenario* scenario = Scenario::ToScenario(scenName, data);
			load->children[i] = scenario;
		}
		return load;
	}
};