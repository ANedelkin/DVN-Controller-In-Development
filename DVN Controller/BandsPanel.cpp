#include "BandsPanel.h"

BandsPanel::BandsPanel(wxWindow* parent, Scenario* scenario) : wxPanel(parent) {
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxPanel* mainPanel = new wxPanel(this);
    wxPanel* labelsPanel = new wxPanel(mainPanel);
    table = new wxScrolled<wxPanel>(mainPanel);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* labelsSizer = new wxBoxSizer(wxHORIZONTAL);
    tableSizer = new wxBoxSizer(wxVERTICAL);

    //mainPanel->SetBackgroundColour(wxColour(240, 240, 240));

    wxFont labelsFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    wxStaticText* nameLabel = new wxStaticText(labelsPanel, wxID_ANY, "Name", wxDefaultPosition, wxSize(250, -1));
    wxStaticText* startValueLabel = new wxStaticText(labelsPanel, wxID_ANY, "Start frequecy", wxDefaultPosition, wxSize(150, -1));
    wxStaticText* endValueLabel = new wxStaticText(labelsPanel, wxID_ANY, "End frequency", wxDefaultPosition, wxSize(150, -1));

    nameLabel->SetFont(labelsFont);
    startValueLabel->SetFont(labelsFont);
    endValueLabel->SetFont(labelsFont);
    
    labelsSizer->AddSpacer(20);
    labelsSizer->Add(nameLabel, 0, wxRIGHT | wxALIGN_CENTER, 5);
    labelsSizer->Add(startValueLabel, 0, wxRIGHT | wxALIGN_CENTER, 5);
    labelsSizer->Add(endValueLabel, 0, wxALIGN_CENTER);

    labelsPanel->SetSizerAndFit(labelsSizer);
    
    table->SetScrollRate(0, 10);

    this->scenario = scenario;

    for (size_t i = 0; i < BANDS_COUNT; i++) {
        BandRow* row = new BandRow(table, scenario, i);
        tableSizer->Add(row, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 15);
    }

    table->SetSizerAndFit(tableSizer);

    mainSizer->Add(labelsPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 15);
    mainSizer->Add(table, 1, wxEXPAND);

    mainPanel->SetSizerAndFit(mainSizer);

    sizer->Add(mainPanel, 0, wxTOP | wxBOTTOM | wxEXPAND, 20);
    this->SetSizerAndFit(sizer);
}

