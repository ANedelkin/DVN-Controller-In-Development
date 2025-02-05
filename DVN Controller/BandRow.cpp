#pragma once

#include "BandRow.h"

void BandRow::OnResize(wxSizeEvent& e) {
    if (toBeInited) {
        background->SetSize(GetSize());
        num->Raise();
        name->Raise();
        startValue->Raise();
        endValue->Raise();
        statBtn->Raise();
        e.Skip();
        toBeInited = false;
    }
}
void BandRow::OnNameEnter(wxCommandEvent& e) {
    if (!Rename())
        focused = nullptr;
    base->SetFocus();
    e.Skip();
}
void BandRow::OnFreqEnter(wxCommandEvent& e) {
    if (!ChangeFreqs()) {
        focused = nullptr;
        base->SetFocus();
    }
    e.Skip();
}
void BandRow::OnStatusChanged(wxMouseEvent& e)
{
    if (scenario->IsActive(bandNum)) {
        scenario->TurnOff(bandNum);
        statBtn->SetForegroundColour(wxColour(*wxRED));
        statBtn->SetLabel("OFF");
    }
    else {
        Status stat = scenario->TurnOn(bandNum);
        if (stat) {
            wxMessageDialog(base, errorMessages[stat], "Error", wxICON_ERROR).ShowModal();
        }
        else {
            statBtn->SetForegroundColour(DARK_GREEN);
            statBtn->SetLabel("ON");
        }
    }
    e.Skip();
}
void BandRow::OnFocus(wxFocusEvent& e) {
    focused = this->FindFocus();
    e.Skip();
}

BandRow::BandRow(wxWindow* parent, Scenario* scenario, char bandNum) : wxWindow(parent, wxID_ANY) {
    this->scenario = scenario;
    this->bandNum = bandNum;

    background = new wxButton(this, wxID_ANY);
    background->SetBackgroundColour(wxColour(255, 255, 255));
    background->Disable();

    InitForeground();

    Bind(wxEVT_SIZE, &BandRow::OnResize, this);
    //Bind(wxEVT_PAINT, &BandRow::OnPaint, this);
}

void BandRow::InitForeground() {
    num = new wxStaticText(this, wxID_ANY, to_string(bandNum + 1) + '.', wxDefaultPosition, wxSize(15, -1));
    num->SetBackgroundColour(wxColour(255, 255, 255));
    name = new wxTextCtrl(this, wxID_ANY, scenario->GetName(bandNum), wxDefaultPosition, wxSize(250, -1), wxTE_PROCESS_ENTER);
    startValue = new wxTextCtrl(this, wxID_ANY, to_string(scenario->GetStartValue(bandNum)), wxDefaultPosition, wxSize(110, -1), wxTE_PROCESS_ENTER);
    endValue = new wxTextCtrl(this, wxID_ANY, to_string(scenario->GetEndValue(bandNum)), wxDefaultPosition, wxSize(110, -1), wxTE_PROCESS_ENTER);
    bool active = scenario->IsActive(bandNum);
    statBtn = new wxButton(this, wxID_ANY, active ? "ON" : "OFF");
    statBtn->SetBackgroundColour(wxColour(*wxWHITE));
    statBtn->SetForegroundColour(wxColour(active ? *wxGREEN : *wxRED));

    BindEventHandlers();
    SetUpSizers();
}

void BandRow::BindEventHandlers()
{
    name->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);
    startValue->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);
    endValue->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);

    name->Bind(wxEVT_TEXT_ENTER, &BandRow::OnNameEnter, this);
    startValue->Bind(wxEVT_TEXT_ENTER, &BandRow::OnFreqEnter, this);
    endValue->Bind(wxEVT_TEXT_ENTER, &BandRow::OnFreqEnter, this);

    name->SetClientData((void*)BandName);
    startValue->SetClientData((void*)Freq);
    endValue->SetClientData((void*)Freq);

    statBtn->Bind(wxEVT_LEFT_UP, &BandRow::OnStatusChanged, this);
}

void BandRow::SetUpSizers()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->AddSpacer(10);
    sizer->Add(num, 0, wxALIGN_CENTER | wxRIGHT | wxTOP | wxBOTTOM, 10);
    sizer->Add(name, 0, wxRIGHT | wxTOP | wxBOTTOM, 10);
    sizer->Add(startValue, 0, wxRIGHT | wxTOP | wxBOTTOM, 10);
    sizer->Add(endValue, 0, wxRIGHT | wxTOP | wxBOTTOM, 10);
    sizer->Add(statBtn, 0, wxTOP | wxBOTTOM, 10);
    sizer->AddSpacer(10);
    this->SetSizerAndFit(sizer);
}

void BandRow::ChangeScenario(Scenario* scenario) {
    this->scenario = scenario;

    name->SetValue(scenario->GetName(bandNum));
    startValue->SetValue(to_string(scenario->GetStartValue(bandNum)));
    endValue->SetValue(to_string(scenario->GetEndValue(bandNum)));

    bool active = scenario->IsActive(bandNum);
    statBtn->SetLabel(active ? "ON" : "OFF");
    statBtn->SetBackgroundColour(wxColour(*wxWHITE));
    statBtn->SetForegroundColour(wxColour(active ? *wxGREEN : *wxRED));
}

Status BandRow::Rename() {
    string newName = name->GetLineText(0).ToStdString();
    Status stat = scenario->Rename(newName, bandNum);
    if (stat) {
        wxMessageDialog dialog(base, errorMessages[stat], "Error", wxOK | wxCANCEL | wxICON_ERROR);
        dialog.SetOKCancelLabels("Enter new name", "Keep old name");
        int id = dialog.ShowModal();
        if (id == wxID_CANCEL) {
            name->SetValue(scenario->GetName(bandNum));
            return Success;
        }
    }
    return stat;
}

Status BandRow::ChangeFreqs() {
    int newStart = stoi(startValue->GetLineText(0).ToStdString());
    int newEnd = stoi(endValue->GetLineText(0).ToStdString());

    Status stat = scenario->SetBandValues(bandNum, newStart, newEnd);
    if (stat) {
        wxMessageDialog dialog(base, errorMessages[stat], "Error", wxOK | wxCANCEL | wxICON_ERROR);
        dialog.SetOKCancelLabels("Enter new value", "Keep old value");
        int id = dialog.ShowModal();
        if (id == wxID_CANCEL) {
            startValue->SetValue(to_string(scenario->GetStartValue(bandNum)));
            endValue->SetValue(to_string(scenario->GetEndValue(bandNum)));
            return Success;
        }
    }
    return stat;
}