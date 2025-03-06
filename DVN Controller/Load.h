#pragma once
#include "Global.h"
#include "Scenario.h"

#include "ScenarioPreset.h"
#include "ModelConstants.h"

class Load : public DVNFileData {
public:
	Load();
	Load(const string& name);
	Load(const string& name, const string& folder);

	string GetName();

	string SaveString() const override;

	static Load* ToLoad(const string& name, const string& folder, stringstream& data);
};