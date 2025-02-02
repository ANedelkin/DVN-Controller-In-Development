#include "BandsPanel.h"

BandsPanel::BandsPanel(wxWindow* parent, Scenario* scenario) : wxPanel(parent) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxPanel* table = new wxPanel(this);
    wxPanel* labels = new wxPanel(table);
    content = new wxPanel(table);

    wxBoxSizer* labelsSizer = new wxBoxSizer(wxHORIZONTAL);
    tableSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

    wxFont labelsFont = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    wxStaticText* nameLabel = new wxStaticText(labels, wxID_ANY, "Name", wxDefaultPosition, wxSize(250, -1));
    wxStaticText* startValueLabel = new wxStaticText(labels, wxID_ANY, "Start frequecy", wxDefaultPosition, wxSize(110, -1));
    wxStaticText* endValueLabel = new wxStaticText(labels, wxID_ANY, "End frequency", wxDefaultPosition, wxSize(110, -1));

    nameLabel->SetFont(labelsFont);
    startValueLabel->SetFont(labelsFont);
    endValueLabel->SetFont(labelsFont);
    
    labelsSizer->AddSpacer(20);
    labelsSizer->Add(nameLabel, 0, wxRIGHT | wxALIGN_CENTER, 5);
    labelsSizer->Add(startValueLabel, 0, wxRIGHT | wxALIGN_CENTER, 5);
    labelsSizer->Add(endValueLabel, 0, wxALIGN_CENTER);

    labels->SetSizerAndFit(labelsSizer);

    this->scenario = scenario;

    for (char i = 0; i < BANDS_COUNT; i++) {
        bandRows[i] = new BandRow(content, scenario, i);
        contentSizer->Add(bandRows[i], 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    }

    content->SetSizerAndFit(contentSizer);

    tableSizer->Add(labels, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    tableSizer->Add(content, 1, wxEXPAND);

    table->SetSizerAndFit(tableSizer);

    mainSizer->Add(table, 0, wxTOP | wxBOTTOM | wxEXPAND, 10);
    this->SetSizerAndFit(mainSizer);
}

void BandsPanel::ChangeScenario(Scenario* scen)
{
    this->scenario = scen;
    for (char i = 0; i < BANDS_COUNT; i++)
    {
        bandRows[i]->ChangeScenario(scen);
    }
}

