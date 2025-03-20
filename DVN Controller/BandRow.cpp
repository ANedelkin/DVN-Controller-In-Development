#pragma once

#include "BandRow.h"

void BandRow::OnResize(wxSizeEvent& e) {
    if (toBeInited) {
        background->SetSize(GetSize());
        name->Raise();
        startValue->Raise();
        endValue->Raise();
        statBtn->Raise();
        num->Raise();
        e.Skip();
        toBeInited = false;
    }
}
void BandRow::OnNameEnter(wxKeyEvent& e) {
    int key = e.GetKeyCode();
    if ((key == WXK_TAB || key == WXK_RETURN || key == WXK_ESCAPE) && !Rename()) {
        if (key == WXK_TAB) {
            if(wxGetKeyState(WXK_SHIFT)) 
                focused->Navigate(wxNavigationKeyEvent::IsBackward);
            else 
                focused->Navigate();
        }
        else {
            Unfocus();
            focused = nullptr;
        }
    }
    e.Skip();
}
void BandRow::OnStartEnter(wxKeyEvent& e) {
    int key = e.GetKeyCode();
    if ((key == WXK_TAB || key == WXK_RETURN || key == WXK_ESCAPE) && !ChangeStart()) {
        if (key == WXK_TAB) {
            if (wxGetKeyState(WXK_SHIFT))
                focused->Navigate(wxNavigationKeyEvent::IsBackward);
            else
                focused->Navigate();
        }
        else {
            Unfocus();
            focused = nullptr;
        }
    }
    e.Skip();
}
void BandRow::OnEndEnter(wxKeyEvent& e) {
    int key = e.GetKeyCode();
    if ((key == WXK_TAB || key == WXK_RETURN || key == WXK_ESCAPE) && !ChangeEnd()) {
        if (key == WXK_TAB) {
            if (wxGetKeyState(WXK_SHIFT))
                focused->Navigate(wxNavigationKeyEvent::IsBackward);
            else
                focused->Navigate();
        }
        else {
            Unfocus();
            focused = nullptr;
        }
    }
    e.Skip();
}
void BandRow::OnStatusChanged(wxMouseEvent& e)
{
    Refresh();
    if (scenario->IsActive(bandNum)) {
        scenario->TurnOff(bandNum);
        statBtn->SetForegroundColour(wxColour(*wxRED));
        statBtn->SetLabel("OFF");
        MarkUnsaved();
    }
    else {
        Status stat = scenario->TurnOn(bandNum);
        if (stat) {
            wxMessageDialog(base, errorMessages.at(stat), "Error", wxICON_ERROR).ShowModal();
            return;
        }
        statBtn->SetForegroundColour(DARK_GREEN);
        statBtn->SetLabel("ON");
    }
    MarkUnsaved();
    e.Skip();
}

void BandRow::OnFocus(wxFocusEvent& e) {
    focused = FindFocus();
    e.Skip();
}

BandRow::BandRow(wxWindow* parent, Scenario* scenario, char bandNum) : wxPanel(parent, wxID_ANY) {
    this->scenario = scenario;
    this->bandNum = bandNum;

    unfocused = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxSize(0, 0));
    background = new wxButton(this, wxID_ANY, wxEmptyString);
    background->SetBackgroundColour(wxColour(255, 255, 255));
    background->Disable();

    InitForeground();
    BindEventHandlers();

    SetMinSize(FromDIP(wxSize(-1, 40)));
}

void BandRow::InitForeground() {
    num = new wxStaticText(this, wxID_ANY, to_string(bandNum + 1) + '.', wxDefaultPosition, FromDIP(wxSize(15, -1)));
    num->SetBackgroundColour(wxColour(255, 255, 255));
    name = new wxTextCtrl(this, wxID_ANY, scenario->GetName(bandNum), wxDefaultPosition, FromDIP(wxSize(250, -1)), wxTE_PROCESS_ENTER);
    startValue = new wxTextCtrl(this, wxID_ANY, to_string(scenario->GetStartValue(bandNum)), wxDefaultPosition, FromDIP(wxSize(110, -1)), wxTE_PROCESS_ENTER);
    endValue = new wxTextCtrl(this, wxID_ANY, to_string(scenario->GetEndValue(bandNum)), wxDefaultPosition, FromDIP(wxSize(110, -1)), wxTE_PROCESS_ENTER);
    bool active = scenario->IsActive(bandNum);
    statBtn = new wxButton(this, wxID_ANY, active ? "ON" : "OFF");
    statBtn->SetBackgroundColour(wxColour(*wxWHITE));
    statBtn->SetForegroundColour(wxColour(active ? DARK_GREEN : *wxRED));

    name->SetClientData((void*)BandName);
    startValue->SetClientData((void*)Start);
    endValue->SetClientData((void*)End);

    SetUpSizers();
}

void BandRow::BindEventHandlers()
{
    Bind(wxEVT_SIZE, &BandRow::OnResize, this);

    name->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);
    startValue->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);
    endValue->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);

    name->Bind(wxEVT_KEY_DOWN, &BandRow::OnNameEnter, this);
    startValue->Bind(wxEVT_KEY_DOWN, &BandRow::OnStartEnter, this);
    endValue->Bind(wxEVT_KEY_DOWN, &BandRow::OnEndEnter, this);

    statBtn->Bind(wxEVT_LEFT_UP, &BandRow::OnStatusChanged, this);
}

void BandRow::SetUpSizers()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->AddSpacer(FromDIP(10));
    sizer->Add(num, 0, wxALIGN_CENTER | wxRIGHT, FromDIP(10));
    sizer->Add(name, 0, wxALIGN_CENTER | wxRIGHT, FromDIP(10));
    sizer->Add(startValue, 0, wxALIGN_CENTER | wxRIGHT, FromDIP(10));
    sizer->Add(endValue, 0, wxALIGN_CENTER | wxRIGHT, FromDIP(10));
    sizer->Add(statBtn, 0, wxALIGN_CENTER);
    sizer->AddSpacer(FromDIP(10));
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
    statBtn->SetForegroundColour(wxColour(active ? DARK_GREEN : *wxRED));
}

Status BandRow::Rename() {
    string newName = name->GetLineText(0).ToStdString();
    if (newName == scenario->GetName(bandNum)) return Success;
    Status stat = scenario->Rename(newName, bandNum);
    if (stat) {
        wxMessageDialog dialog(base, errorMessages.at(stat), "Error", wxOK | wxCANCEL | wxICON_ERROR);
        dialog.SetOKCancelLabels("Enter new name", "Keep old name");
        int id = dialog.ShowModal();
        if (id == wxID_CANCEL) {
            name->SetValue(scenario->GetName(bandNum));
            return Success;
        }
    }
    MarkUnsaved();
    return stat;
}
    
Status BandRow::ChangeStart() {
    int newStart = stoi(startValue->GetLineText(0).ToStdString());
    if(newStart == scenario->GetStartValue(bandNum)) return Success;
    Status stat = scenario->SetStartValue(bandNum, newStart);
    if (stat) {
        wxMessageDialog dialog(base, errorMessages.at(stat), "Error", wxOK | wxCANCEL | wxICON_ERROR);
        dialog.SetOKCancelLabels("Enter new value", "Keep old value");
        int id = dialog.ShowModal();
        if (id == wxID_CANCEL) {
            startValue->SetValue(to_string(scenario->GetStartValue(bandNum)));
            return Success;
        }
    }
    MarkUnsaved();
    return stat;
}

Status BandRow::ChangeEnd() {
    int newEnd = stoi(endValue->GetLineText(0).ToStdString());
    if (newEnd == scenario->GetEndValue(bandNum)) return Success;
    Status stat = scenario->SetEndValue(bandNum, newEnd);
    if (stat) {
        wxMessageDialog dialog(base, errorMessages.at(stat), "Error", wxOK | wxCANCEL | wxICON_ERROR);
        dialog.SetOKCancelLabels("Enter new value", "Keep old value");
        int id = dialog.ShowModal();
        if (id == wxID_CANCEL) {
            endValue->SetValue(to_string(scenario->GetEndValue(bandNum)));
            return Success;
        }
    }
    MarkUnsaved();
    return stat;
}

void BandRow::Unfocus()
{
    unfocused->SetFocus();
}

void BandRow::MarkUnsaved()
{
    wxCommandEvent e(EVT_UNSAVE);
    GetParent()->GetEventHandler()->ProcessEvent(e);
}
