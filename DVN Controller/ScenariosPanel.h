#pragma once
#include "Global.h"
#include "Scenario.h"
#include "Load.h"

#include "SideMenuCtrl.h"
#include "BandsPanel.h"

class ScenariosPanel : public wxPanel
{
private:
	Load* load;

	vector<SideMenuCtrl*> scenarios;
	
	wxStaticBox* scenList;
	wxStaticBoxSizer* scenSizer;
	SideMenuCtrl* curScen;
	Scenario* placeHolder = new Scenario();

	wxStaticBox* bandsBox;
	wxStaticBoxSizer* bandsSizer;
	BandsPanel* bandsPanel;

	void ChangeSelection(SideMenuCtrl* scenCtrl);

	void OnScenSelect(wxMouseEvent& e);

	void OnScenDelete(wxCommandEvent& e);
public:
	ScenariosPanel(wxWindow* parent);
	ScenariosPanel(wxWindow* parent, Load* load);

	void AddScenario(Scenario* scen);

	void ChangeLoad(Load* load);
};

