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
		scenarios[i] = Scenario(string("Scenario ") + to_string(i + 1));
	}
	//oldSaveString = SaveString();
}

string Load::GetName() { return name; }

array<Scenario, SCENARIOS_COUNT>& Load::GetScenarios()
{
	return scenarios;
}

string Load::SaveString() const {
	ostringstream stream;
	for (char i = 0; i < scenarios.size(); i++)
	{
		stream << scenarios[i].DVNFileData::GetName() << endl << scenarios[i].SaveString();
		if (i != scenarios.size() - 1) stream << endl;
	}
	return stream.str();
}

bool Load::AlteredFromOutside()
{
	return folder != "" && !(exists(folder) && is_directory(folder));
}

Load* Load::ToLoad(const string& name, const string& folder, stringstream& data) {
	Load* load = new Load(name, folder);
	if (!Load::ValidateName(name).empty())
		goto NotOkay;
	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		string scenName;
		if (getline(data, scenName)) {
			load->scenarios[i] = *Scenario::ToScenario(scenName, data);
			if (!load->scenarios[i].ok)
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

DVNFileData* Load::Copy() const
{
	Load* load = new Load(*this);
	load->oldSaveString = "";
	load->folder = "";
	return load;
}
