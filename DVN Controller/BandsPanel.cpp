#include "BandsPanel.h"

void BandsPanel::OnScrollTo(wxCommandEvent& e)
{
    wxWindow* target = (wxWindow*)e.GetEventObject();
    wxPoint pos = target->GetPosition();
    int viewStart = scrollWrapper->GetViewStart().y;
    int x, pxPerUnit;
    scrollWrapper->GetScrollPixelsPerUnit(&x, &pxPerUnit);
    int y = pos.y / pxPerUnit;

    if (y < viewStart)
        scrollWrapper->Scroll(0, y);
    else  {
        int targetSize = target->GetSize().y / pxPerUnit;
        wxSize swSize = scrollWrapper->GetClientSize();
        int curEnd = viewStart + swSize.y / pxPerUnit - targetSize;
        if (y > curEnd) {
            int x, scrollRate;
            scrollWrapper->GetScrollPixelsPerUnit(&x, &scrollRate);
            int viewStartY;
            scrollWrapper->GetViewStart(&x, &viewStartY);
            int viewStartPy = viewStartY * scrollRate;
            int targetY = viewStartPy;
            if (pos.y < viewStartPy) {
                targetY = pos.y;
            }
            else if (pos.y + target->GetSize().y > viewStartPy + swSize.y) {
                targetY = pos.y + target->GetSize().y - swSize.y;
            }
            int targetUnitY = targetY / scrollRate;
            scrollWrapper->Scroll(0, targetUnitY + 1);
        }
    }
}

BandsPanel::BandsPanel(wxWindow* parent, Scenario* scenario, const bool readOnly) : SideNotebookContent(parent, scenario) {
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

    labels->SetSizer(labelsSizer);

    this->source = scenario;

    for (char i = 0; i < GetBandsCount(); i++) {
        bandRows[i] = new BandRow(content, scenario, i, readOnly);
        contentSizer->Add(bandRows[i], 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
    }

    content->SetSizer(contentSizer);

    scrollSizer->Add(content, 1, wxEXPAND | wxRIGHT, FromDIP(15));

    scrollWrapper->SetSizerAndFit(scrollSizer);

    tableSizer->Add(labels, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
    tableSizer->Add(scrollWrapper, 1, wxEXPAND);

    table->SetSizer(tableSizer);

    mainSizer->Add(table, 0, wxTOP | wxBOTTOM | wxEXPAND, FromDIP(10));
    bandsBox->SetSizerAndFit(mainSizer);

    panelSizer->Add(bandsBox);
    SetSizerAndFit(panelSizer);

    //Bind(EVT_SCROLL_TO, &BandsPanel::OnScrollTo, this);
}

void BandsPanel::SetSource(DVNFileData* source)
{
    this->source = source;
    Scenario* scen = dynamic_cast<Scenario*>(source);
    assert(scen != nullptr && "source is not Scenario or derived");
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