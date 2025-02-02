#pragma once
#include "Global.h"
#include "Scenario.h"
#include "ScenCtrl.h"
#include "BandsPanel.h"

class ScenariosPanel : public wxPanel
{
private:
	vector<ScenCtrl*> scenarios;
	
	wxStaticBox* scenList;
	wxStaticBoxSizer* scenSizer;
	ScenCtrl* curScen;

	wxStaticBox* bandsBox;
	wxStaticBoxSizer* bandsSizer;
	BandsPanel* bandsPanel = nullptr;

	void ChangeSelection(ScenCtrl* scenCtrl);

	void OnScenSelect(wxMouseEvent& e);
public:
	ScenariosPanel(wxWindow* parent);

	void AddScenario(Scenario* scen);
};

