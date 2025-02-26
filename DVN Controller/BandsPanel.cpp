#include "BandsPanel.h"
#include "Load.h"

BandsPanel::BandsPanel(wxWindow* parent, wxPanel* mainPanel, Scenario* scenario) : SideNotebookPanel(parent, mainPanel, scenario) {
    wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBox* bandsBox = new wxStaticBox(this, wxID_ANY, "Bands");
    wxStaticBoxSizer* mainSizer = new wxStaticBoxSizer(wxHORIZONTAL, bandsBox, "Bands");
    
    table = new wxPanel(bandsBox);
    wxPanel* labels = new wxPanel(table);
    wxScrolled<wxPanel>* scrollWrapper = new wxScrolled<wxPanel>(table);
    content = new wxPanel(scrollWrapper);

    scrollWrapper->SetScrollRate(0, FromDIP(5));

    wxBoxSizer* labelsSizer = new wxBoxSizer(wxHORIZONTAL);
    tableSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* scrollSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

        wxFont labelsFont = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

        wxStaticText* nameLabel = new wxStaticText(labels, wxID_ANY, "Name", wxDefaultPosition, FromDIP(wxSize(250, -1)));
        wxStaticText* startValueLabel = new wxStaticText(labels, wxID_ANY, "Start frequecy", wxDefaultPosition, FromDIP(wxSize(110, -1)));
        wxStaticText* endValueLabel = new wxStaticText(labels, wxID_ANY, "End frequency", wxDefaultPosition, FromDIP(wxSize(110, -1)));
        wxStaticText* activeLabel = new wxStaticText(labels, wxID_ANY, "Status");

        nameLabel->SetFont(labelsFont);
        startValueLabel->SetFont(labelsFont);
        endValueLabel->SetFont(labelsFont);
        activeLabel->SetFont(labelsFont);

        labelsSizer->AddSpacer(FromDIP(35));
        labelsSizer->Add(nameLabel, 0, wxALIGN_CENTER | wxRIGHT, FromDIP(11));
        labelsSizer->Add(startValueLabel, 0, wxALIGN_CENTER | wxRIGHT, FromDIP(11));
        labelsSizer->Add(endValueLabel, 0, wxALIGN_CENTER | wxRIGHT, FromDIP(11));
        labelsSizer->Add(activeLabel, 0, wxALIGN_CENTER);
        labelsSizer->AddSpacer(FromDIP(10));

        labels->SetSizerAndFit(labelsSizer);

    this->source = scenario;

    for (char i = 0; i < BANDS_COUNT; i++) {
        bandRows[i] = new BandRow(content, scenario, i);
        contentSizer->Add(bandRows[i], 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
    }

    content->SetSizerAndFit(contentSizer);

    scrollSizer->Add(content, 1, wxEXPAND | wxRIGHT, FromDIP(15));

    scrollWrapper->SetSizerAndFit(scrollSizer);

    tableSizer->Add(labels, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
    tableSizer->Add(scrollWrapper, 1, wxEXPAND);

    table->SetSizerAndFit(tableSizer);

    mainSizer->Add(table, 0, wxTOP | wxBOTTOM | wxEXPAND, FromDIP(10));
    bandsBox->SetSizerAndFit(mainSizer);

    panelSizer->Add(bandsBox);
    this->SetSizerAndFit(panelSizer);
}

void BandsPanel::ChangeSource(DVNFileData* source)
{
    this->source = source;
    Scenario* scen = dynamic_cast<Scenario*>(source);
    for (char i = 0; i < bandRows.size(); i++)
    {
        bandRows[i]->ChangeScenario(scen);
    }
}

void BandsPanel::Init()
{
    SideNotebookPanel::Init();
    table->Show();
}

void BandsPanel::UnInit() {
    SideNotebookPanel::UnInit();
    table->Hide();
}