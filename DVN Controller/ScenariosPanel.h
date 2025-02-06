#pragma once
#include "Global.h"
#include "Scenario.h"
#include "SideMenuCtrl.h"
#include "BandsPanel.h"

class ScenariosPanel : public wxPanel
{
private:
	//vector<
	vector<SideMenuCtrl*> scenarios;
	
	wxStaticBox* scenList;
	wxStaticBoxSizer* scenSizer;
	SideMenuCtrl* curScen;
	Scenario* placeHolder = new Scenario();

	wxStaticBox* bandsBox;
	wxStaticBoxSizer* bandsSizer;
	BandsPanel* bandsPanel = nullptr;

	void ChangeSelection(SideMenuCtrl* scenCtrl);

	void OnScenSelect(wxMouseEvent& e);

	void OnScenDelete(wxCommandEvent& e);
public:
	ScenariosPanel(wxWindow* parent);

	void AddScenario(Scenario* scen);
};

