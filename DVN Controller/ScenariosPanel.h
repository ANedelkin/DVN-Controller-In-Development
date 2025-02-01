#pragma once
#include "Global.h"
#include "Scenario.h"
#include "ScenCtrl.h"

class ScenariosPanel : public wxPanel
{
private:
	vector<ScenCtrl*> scenarios;
	
	wxStaticBox* scenList;
	wxStaticBoxSizer* scenSizer;
	ScenCtrl* curScen;

	wxStaticBox* bandsList;
	wxStaticBoxSizer* bandsSizer;
	wxPanel* bandsPanel = nullptr;

	void ChangeSelection(ScenCtrl* scenCtrl);

	void OnScenSelect(wxMouseEvent& e);
public:
	ScenariosPanel(wxWindow* parent);

	void AddScenario(Scenario* scen);
};

