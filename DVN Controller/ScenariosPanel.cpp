#include "ScenariosPanel.h"
#include "BandsPanel.h"

ScenariosPanel::ScenariosPanel(wxWindow* parent) : wxPanel(parent)
{
	curScen = nullptr;

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	scenList = new wxStaticBox(this, wxID_ANY, "Scenarios");
	scenSizer = new wxStaticBoxSizer(wxVERTICAL, scenList, "Scenarios");
	scenSizer->AddSpacer(5);
	scenList->SetSizerAndFit(scenSizer);
	scenList->SetMinSize(wxSize(250, -1));

	mainSizer->Add(scenList, 0, wxEXPAND);

	bandsList = new wxStaticBox(this, wxID_ANY, "Bands");
	bandsSizer = new wxStaticBoxSizer(wxVERTICAL, bandsList, "Bands");
	bandsList->SetSizerAndFit(bandsSizer);

	mainSizer->Add(bandsList, 1, wxEXPAND | wxLEFT, 5);

	this->SetSizerAndFit(mainSizer);
}

void ScenariosPanel::AddScenario(Scenario* scen)
{
	ScenCtrl* scenCtrl = new ScenCtrl(scenList, scen);
	scenarios.push_back(scenCtrl);
	scenSizer->Add(scenCtrl, 0, wxEXPAND | wxBOTTOM, 5);
	ChangeSelection(scenCtrl);
	Refresh();
	Layout();
	scenCtrl->Bind(wxEVT_LEFT_UP, &ScenariosPanel::OnScenSelect, this);
}

void ScenariosPanel::ChangeSelection(ScenCtrl* scenCtrl)
{
	Freeze();

	scenCtrl->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	if (curScen) curScen->SetBackgroundColour(wxColour(255, 255, 255));
	curScen = scenCtrl;

	if (bandsPanel) bandsPanel->Destroy();
	bandsPanel = new BandsPanel(bandsList, curScen->scenario);
	bandsSizer->Add(bandsPanel, 1, wxEXPAND);

	Thaw();
	Layout();
}

void ScenariosPanel::OnScenSelect(wxMouseEvent& e)
{
	ScenCtrl* scenCtrl = dynamic_cast<ScenCtrl*>(e.GetEventObject());
	if (scenCtrl != curScen) ChangeSelection(scenCtrl);
	e.Skip();
}