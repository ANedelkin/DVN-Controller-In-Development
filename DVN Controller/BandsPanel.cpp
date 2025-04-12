#include "BandsPanel.h"
#include "Load.h"

BandsPanel::BandsPanel(wxWindow* parent, Scenario* scenario, const char style) : SideNotebookContent(parent, scenario) {
    wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBox* bandsBox = new wxStaticBox(this, wxID_ANY, "Bands");
    wxStaticBoxSizer* mainSizer = new wxStaticBoxSizer(wxHORIZONTAL, bandsBox, "Bands");
    
    table = new wxPanel(bandsBox);
    wxPanel* labels = new wxPanel(table);
    scrollWrapper = new wxScrolled<wxPanel>(table);
    content = new wxPanel(scrollWrapper);

    scrollWrapper->SetScrollRate(0, FromDIP(5));

    wxBoxSizer* labelsSizer = new wxBoxSizer(wxHORIZONTAL);
    tableSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* scrollSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

    wxFont labelsFont = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    wxStaticText* nameLabel = new wxStaticText(labels, wxID_ANY, "Name", wxDefaultPosition, FromDIP(wxSize(NAME_INPUT_LEN, -1)));
    wxStaticText* startValueLabel = new wxStaticText(labels, wxID_ANY, "Start frequecy", wxDefaultPosition, FromDIP(wxSize(FREQ_INPUT_LEN, -1)));
    wxStaticText* endValueLabel = new wxStaticText(labels, wxID_ANY, "End frequency", wxDefaultPosition, FromDIP(wxSize(FREQ_INPUT_LEN, -1)));
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

    for (char i = 0; i < GetBandsCount(); i++) {
        bandRows[i] = new BandRow(content, scenario, i, style);
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

void BandsPanel::SetSource(DVNFileData* source) //Make only called when needed, not on initial scenario loading
{
    this->source = source;
    Scenario* scen = dynamic_cast<Scenario*>(source);
    for (char i = 0; i < GetBandsCount(); i++)
    {
        bandRows[i]->ChangeScenario(scen);
    }
    scrollWrapper->Scroll(0, 0);
}

void BandsPanel::Init()
{
    SideNotebookContent::Init();
    table->Show();
}

void BandsPanel::UnInit() {
    SideNotebookContent::UnInit();
    table->Hide();
}