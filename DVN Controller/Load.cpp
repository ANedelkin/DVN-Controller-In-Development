#include "Load.h"

const string Load::extension = ".dvnl";
Load* Load::placeHolder = new Load();

Load::Load() : Load("Unnamed load") {}
Load::Load(const string& name) : Load(name, "") {}
Load::Load(const string& name, const string& folder) : DVNFileData(name) {
	DVNFileData::extension = extension;
	this->folder = folder;
	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		scenarios[i] = new Scenario(string("Scenario ") + to_string(i + 1));
	}
	oldSaveString = SaveString();
}

Load::~Load()
{
	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		delete scenarios[i];
	}
}

string Load::GetName() { return name; }

array<Scenario*, SCENARIOS_COUNT>& Load::GetScenarios()
{
	return scenarios;
}

string Load::SaveString() const {
	ostringstream stream;
	for (char i = 0; i < scenarios.size(); i++)
	{
		stream << scenarios[i]->DVNFileData::GetName() << endl << scenarios[i]->SaveString();
		if (i != scenarios.size() - 1) stream << endl;
	}
	return stream.str();
}

Load* Load::ToLoad(const string& name, const string& folder, stringstream& data) {
	Load* load = new Load(name, folder);
	if (!Load::ValidateName(name).empty())
		goto NotOkay;
	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		string scenName;
		if (getline(data, scenName)) {
			Scenario* scenario = Scenario::ToScenario(scenName, data);
			if (scenario->ok)
				load->scenarios[i] = scenario;
			else
				goto NotOkay;
		}
		else
			goto NotOkay;

	}
	load->oldSaveString = load->SaveString();
	return load;
NotOkay:
	load->ok = false;
	return load;
}