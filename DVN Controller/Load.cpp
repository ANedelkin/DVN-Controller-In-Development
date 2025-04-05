#include "Load.h"

const string Load::extension = ".dvnl";

Load::Load() : Load("Unnamed load") {}
Load::Load(const string& name) : Load(name, "") {}
Load::Load(const string& name, const string& folder) : DVNFileData(name) {
	DVNFileData::extension = extension;
	this->folder = folder;
	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		children.push_back(new Scenario(string("Scenario ") + to_string(i + 1)));
	}
	oldSaveString = SaveString();
}

string Load::GetName() { return name; }

string Load::SaveString() const {
	ostringstream stream;
	for (char i = 0; i < children.size(); i++)
	{
		stream << children[i]->GetName() << endl << children[i]->SaveString();
		if (i != children.size() - 1) stream << endl;
	}
	return stream.str();
}

Load* Load::ToLoad(const string& name, const string& folder, stringstream& data) {
	Load* load = new Load(name, folder);
	if (Load::ValidateName(name))
		goto NotOkay;
	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		string scenName;
		if (getline(data, scenName)) {
			Scenario* scenario = Scenario::ToScenario(scenName, data);
			if (scenario->ok)
				load->children[i] = scenario;
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