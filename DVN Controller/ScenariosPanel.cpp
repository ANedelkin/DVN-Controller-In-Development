#include "ScenariosPanel.h"

ScenariosPanel::ScenariosPanel(wxWindow* parent) : wxPanel(parent)
{
	curScen = nullptr;

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	scenList = new wxStaticBox(this, wxID_ANY, "Scenarios");
	scenSizer = new wxStaticBoxSizer(wxVERTICAL, scenList, "Scenarios");
	scenSizer->AddSpacer(5);
	scenList->SetSizerAndFit(scenSizer);
	scenList->SetMinSize(wxSize(210, -1));

	mainSizer->Add(scenList, 0, wxEXPAND);

	bandsBox = new wxStaticBox(this, wxID_ANY, "Bands");
	bandsSizer = new wxStaticBoxSizer(wxVERTICAL, bandsBox, "Bands");
	bandsBox->SetSizerAndFit(bandsSizer);

	mainSizer->Add(bandsBox, 1, wxEXPAND | wxLEFT, 5);

	this->SetSizerAndFit(mainSizer);
}

void ScenariosPanel::AddScenario(Scenario* scen)
{
	ScenCtrl* scenCtrl = new ScenCtrl(scenList, scen);
	scenarios.push_back(scenCtrl);
	scenSizer->Add(scenCtrl, 0, wxEXPAND | wxBOTTOM, 10);
	ChangeSelection(scenCtrl);
	Refresh();
	Layout();
	scenCtrl->Bind(wxEVT_LEFT_UP, &ScenariosPanel::OnScenSelect, this);
}

void ScenariosPanel::ChangeSelection(ScenCtrl* scenCtrl) //TODO: Optimize drawing by not creating a new BandsPanel, but just changing it values
{
	Freeze();

	scenCtrl->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	if (curScen) curScen->SetBackgroundColour(wxColour(255, 255, 255));
	curScen = scenCtrl;

	if (bandsPanel) bandsPanel->ChangeScenario(curScen->scenario);
	else {
		bandsPanel = new BandsPanel(bandsBox, curScen->scenario);
		bandsSizer->Add(bandsPanel);
	} 

	Thaw();
	Layout();
}

void ScenariosPanel::OnScenSelect(wxMouseEvent& e)
{
	ScenCtrl* scenCtrl = dynamic_cast<ScenCtrl*>(e.GetEventObject());
	if (scenCtrl != curScen) ChangeSelection(scenCtrl);
	e.Skip();
}