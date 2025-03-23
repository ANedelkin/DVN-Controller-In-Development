#pragma once

#include "Global.h"
#include "Scenario.h"
#include "BandsPanel.h"

class SideNotebook;

class ScenSelectDialog : public wxDialog
{
private:
	SideNotebook* scenariosPanel;
public:
	ScenSelectDialog(wxWindow* parent);
	void LoadScenarios();
	Scenario* GetSelection();
};

