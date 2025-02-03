#pragma once

#include "BandRow.h"

void BandRow::OnResize(wxSizeEvent& e) {
    if (toBeInited) {
        background->SetSize(GetSize());
        num->Raise();
        name->Raise();
        startValue->Raise();
        endValue->Raise();
        e.Skip();
        toBeInited = false;
    }
}
void BandRow::OnNameEnter(wxCommandEvent& e) {
    if (!Rename())
        base->SetFocus();
    e.Skip();
}
void BandRow::OnStartEnter(wxCommandEvent& e) {
    if (!Rename())
        base->SetFocus();
    e.Skip();
}
void BandRow::OnEndEnter(wxCommandEvent& e) {
    if (!Rename())
        base->SetFocus();
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

    name->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);
    startValue->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);
    endValue->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);

    name->Bind(wxEVT_TEXT_ENTER, &BandRow::OnNameEnter, this);
    startValue->Bind(wxEVT_TEXT_ENTER, &BandRow::OnStartEnter, this);
    endValue->Bind(wxEVT_TEXT_ENTER, &BandRow::OnEndEnter, this);

    name->SetClientData((void*)BandName);
    startValue->SetClientData((void*)Start);
    endValue->SetClientData((void*)End);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->AddSpacer(10);
    sizer->Add(num, 0, wxALIGN_CENTER | wxRIGHT | wxTOP | wxBOTTOM, 10);
    sizer->Add(name, 0, wxRIGHT | wxTOP | wxBOTTOM, 10);
    sizer->Add(startValue, 0, wxRIGHT | wxTOP | wxBOTTOM, 10);
    sizer->Add(endValue, 0, wxTOP | wxBOTTOM, 10);
    sizer->AddSpacer(10);
    this->SetSizerAndFit(sizer);
}

void BandRow::ChangeScenario(Scenario* scenario) {
    this->scenario = scenario;
    name->SetValue(scenario->GetName(bandNum));
    startValue->SetValue(to_string(scenario->GetStartValue(bandNum)));
    endValue->SetValue(to_string(scenario->GetEndValue(bandNum)));
}

Status BandRow::Rename() {
    string newName = name->GetLineText(0).ToStdString();
    Status stat = BandPreset::Validate(newName);
    if (stat) {
        wxMessageDialog dialog(base, errorMessages[stat], "Error", wxOK | wxICON_ERROR);
        dialog.ShowModal();
    }
    else scenario->Rename(newName, bandNum);
    return stat;
}
Status BandRow::ChangeStartValue() {
    int newStart = std::stoi(startValue->GetLineText(0).ToStdString());
    return Success;
}