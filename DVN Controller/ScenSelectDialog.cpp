#include "ScenSelectDialog.h"
#include "ScenariosPanel.h"

ScenSelectDialog::ScenSelectDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, "Select scenario")
{
	SetSize(FromDIP(wxSize(905, 710)));

	wxPanel* mainPanel = new wxPanel(this);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	
	wxPanel* contentPanel = new wxPanel(mainPanel);
	contentPanel->SetBackgroundColour(wxColour(255, 255, 255));
	wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);
	scenariosPanel = new ScenariosPanel(contentPanel, nullptr, READ_ONLY);

	contentSizer->Add(scenariosPanel, 1, wxEXPAND | wxBOTTOM | wxLEFT | wxRIGHT, FromDIP(5));
	contentPanel->SetSizerAndFit(contentSizer);

	wxPanel* btnsPanel = new wxPanel(mainPanel);
	wxBoxSizer* btnsSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* select = new wxButton(btnsPanel, wxID_OK, "Select");
	wxButton* cancel = new wxButton(btnsPanel, wxID_CANCEL, "Cancel");

	select->SetDefault();

	btnsSizer->AddStretchSpacer();
	btnsSizer->Add(select, 0, wxALL, FromDIP(5));
	btnsSizer->Add(cancel, 0, wxALL, FromDIP(5));
	btnsPanel->SetSizerAndFit(btnsSizer);
	
	mainSizer->Add(contentPanel, 1, wxEXPAND);
	mainSizer->Add(btnsPanel, 0, wxEXPAND);
	mainPanel->SetSizerAndFit(mainSizer);

	LoadScenarios();
}

void ScenSelectDialog::LoadScenarios()
{
	vector<Scenario*> scenarios = Scenario::LoadScenarios();
	for (char i = 0; i < scenarios.size(); i++)
	{
		scenariosPanel->NewPage(scenarios[i]);
	}
	scenariosPanel->Select(0);
}

Scenario* ScenSelectDialog::GetSelection()
{
	SideMenuCtrl* cur = scenariosPanel->GetCurrent();
	DVNFileData* source = cur->GetSource();
	return (Scenario*)source;
}
