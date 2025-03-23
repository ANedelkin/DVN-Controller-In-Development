#include "Load.h"

Load::Load() : Load("Unnamed load") {}
Load::Load(const string& name) : Load(name, "") {}
Load::Load(const string& name, const string& folder) : DVNFileData(name) {
	this->extension = ".dvnl";
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
	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		string scenName;
		getline(data, scenName);
		Scenario* scenario = Scenario::ToScenario(scenName, data);
		load->children[i] = scenario;
	}
	load->oldSaveString = load->SaveString();
	return load;
}