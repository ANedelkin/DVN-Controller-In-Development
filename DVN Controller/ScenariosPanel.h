#pragma once

#include "Global.h"
#include "Scenario.h"
#include "Load.h"
#include "BandsPanel.h"
#include "SideNotebook.h"
#include "ScenSelectDialog.h"

class ScenariosPanel : public SideNotebook
{
private:
	char style;

	void OnLoad(wxCommandEvent& e);
	void OnSave(wxCommandEvent& e);
	void OnDelete(wxCommandEvent& e);
public:
	ScenariosPanel(wxWindow* parent, Load* data = nullptr, const char style = 0);
	Status AddPage(Scenario* data);
	void OnRename(wxCommandEvent& e);
};

