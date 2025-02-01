#include "LoadPanel.h"

LoadPanel::LoadPanel(Load* load, wxWindow* parent) : wxPanel(parent)
{
	this->load = load;
	scenPanel = new wxStaticBox(this, wxID_ANY, "Scenarios");
	scenSelectPanel = new wxPanel(scenPanel);
	bandsPanel = new wxStaticBox(this, wxID_ANY, "Bands");
	wxButton* newCustom = new wxButton(scenPanel, wxID_ANY, "Create custom scenario");
	wxButton* newFromPreset = new wxButton(scenPanel, wxID_ANY, "Create scenario from preset");

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
	scenPanel->SetMinSize(wxSize(230, -1));
	mainSizer->Add(scenPanel, 0, wxEXPAND);
	mainSizer->Add(bandsPanel, 1, wxEXPAND | wxLEFT, 5);
	this->SetSizerAndFit(mainSizer);

	wxStaticBoxSizer* scenSizer = new wxStaticBoxSizer(wxVERTICAL, scenPanel, "Scenarios");
	scenSizer->AddSpacer(5);
	scenSizer->Add(scenSelectPanel, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
	scenSizer->AddSpacer(15);
	scenSizer->Add(newCustom, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
	scenSizer->Add(newFromPreset, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

	newCustom->Bind(wxEVT_COMMAND_LEFT_CLICK, &LoadPanel::OnScenCust, this);
	
	scenPanel->SetSizerAndFit(scenSizer);

	noScenariosText = new wxStaticText(bandsPanel, wxID_ANY, wxT("There are no open bands."));
	noScenariosText->SetForegroundColour(wxColour(128, 128, 128));
	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, bandsPanel, "Bands");
	sizer->AddStretchSpacer(10);
	sizer->Add(noScenariosText, 0, wxALIGN_CENTER | wxALL, 10);
	sizer->AddStretchSpacer(12);
	bandsPanel->SetSizerAndFit(sizer);
	bandsPanel->Show(true);
}

void LoadPanel::OnScenCust(wxCommandEvent& e) {
	NameSetter* nameSetter = new NameSetter(this, "Enter Scenario Name", Scenario::ValidateName);
}
