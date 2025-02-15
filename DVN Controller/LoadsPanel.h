#pragma once

#include "Global.h"
#include "Scenario.h"
#include "Load.h"

#include "BandsPanel.h"
#include "SideMenuCtrl.h"
#include "ScenariosPanel.h"

class LoadsPanel : public wxPanel
{
private:
	vector<SideMenuCtrl*> loads;

	wxStaticBox* loadsList;
	wxStaticBoxSizer* loadsSizer;
	SideMenuCtrl* curLoad;
	Load* placeHolder = new Load("");

	ScenariosPanel* scenPanel;

	void ChangeSelection(SideMenuCtrl* loadCtrl);

	void OnLoadSelect(wxMouseEvent& e);

	void OnLoadDelete(wxCommandEvent& e);
public:
	LoadsPanel(wxWindow* parent);

	void AddLoad(Load* load);
};