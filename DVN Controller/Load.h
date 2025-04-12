#pragma once
#include "Global.h"
#include "Scenario.h"

#include "ModelConstants.h"

class Load : public DVNFileData {
private:
	static const string extension;
	array<Scenario*, SCENARIOS_COUNT> scenarios;
public:
	static Load* placeHolder;

	Load();
	Load(const string& name);
	Load(const string& name, const string& folder);

	string GetName();
	array<Scenario*, SCENARIOS_COUNT>& GetScenarios();

	string SaveString() const override;

	static Load* ToLoad(const string& name, const string& folder, stringstream& data);
};