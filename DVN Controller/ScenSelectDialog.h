#pragma once

#include "Global.h"
#include "Scenario.h"
#include "BandsPanel.h"

class ScenariosPanel;

class ScenSelectDialog : public wxDialog
{
private:
	ScenariosPanel* scenariosPanel;
public:
	ScenSelectDialog(wxWindow* parent);
	void LoadScenarios();
	Scenario* GetSelection();
};

