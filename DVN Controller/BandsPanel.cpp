#include "BandsPanel.h"

BandsPanel::BandsPanel(wxWindow* parent, Scenario* scenario) : wxPanel(parent) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxPanel* table = new wxPanel(this);
    wxPanel* labels = new wxPanel(table);
    wxScrolled<wxPanel>* scrollWrapper = new wxScrolled<wxPanel>(table);
    content = new wxPanel(scrollWrapper);

    scrollWrapper->SetScrollRate(0, 5);

    wxBoxSizer* labelsSizer = new wxBoxSizer(wxHORIZONTAL);
    tableSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* scrollSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

    //Labels:

    wxFont labelsFont = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    wxStaticText* nameLabel = new wxStaticText(labels, wxID_ANY, "Name", wxDefaultPosition, wxSize(250, -1));
    wxStaticText* startValueLabel = new wxStaticText(labels, wxID_ANY, "Start frequecy", wxDefaultPosition, wxSize(110, -1));
    wxStaticText* endValueLabel = new wxStaticText(labels, wxID_ANY, "End frequency", wxDefaultPosition, wxSize(110, -1));
    wxStaticText* activeLabel = new wxStaticText(labels, wxID_ANY, "Status");

    nameLabel->SetFont(labelsFont);
    startValueLabel->SetFont(labelsFont);
    endValueLabel->SetFont(labelsFont);
    activeLabel->SetFont(labelsFont);
    
    labelsSizer->AddSpacer(35);
    labelsSizer->Add(nameLabel, 0, wxALIGN_CENTER | wxRIGHT, 11);
    labelsSizer->Add(startValueLabel, 0, wxALIGN_CENTER | wxRIGHT, 11);
    labelsSizer->Add(endValueLabel, 0, wxALIGN_CENTER | wxRIGHT, 11);
    labelsSizer->Add(activeLabel, 0, wxALIGN_CENTER);
    labelsSizer->AddSpacer(10);

    labels->SetSizerAndFit(labelsSizer);

    //!Labels

    this->scenario = scenario;

    for (char i = 0; i < BANDS_COUNT; i++) {
        bandRows[i] = new BandRow(content, scenario, i);
        contentSizer->Add(bandRows[i], 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    }

    content->SetSizerAndFit(contentSizer);

    scrollSizer->Add(content, 1, wxEXPAND | wxRIGHT, 15);

    scrollWrapper->SetSizerAndFit(scrollSizer);

    tableSizer->Add(labels, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    tableSizer->Add(scrollWrapper, 1, wxEXPAND);

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

