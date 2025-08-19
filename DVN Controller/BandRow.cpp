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
void BandRow::EmptyHandler(wxEvent& e)
{
}
void BandRow::OnKey(wxKeyEvent& e) {
    if (!ProcessKey(e.GetKeyCode()))
        e.Skip();
}
void BandRow::OnStatusChanged(wxCommandEvent& e)
{
    if (scenario->IsActive(bandNum)) {
        scenario->TurnOff(bandNum);
        statBtn->SetForegroundColour(*wxRED);
        statBtn->SetLabel("OFF");
    }
    else {
        scenario->TurnOn(bandNum);
        statBtn->SetForegroundColour(DARK_GREEN);
        statBtn->SetLabel("ON");
    }
    MarkUnsaved();
    e.Skip();
}

void BandRow::OnTextCtrlFocus(wxFocusEvent& e) {
    focused = FindFocus();
    ScrollTo();
    e.Skip();
}

void BandRow::OnFocus(wxFocusEvent& e)
{
    ScrollTo();
    e.Skip();
}

void BandRow::ScrollTo()
{
    wxCommandEvent eOut(EVT_SCROLL_TO);
    eOut.SetEventObject(this);
    GetParent()->GetEventHandler()->ProcessEvent(eOut);
}

BandRow::BandRow(wxWindow* parent, Scenario* scenario, const char bandNum, bool readOnly) 
        : wxPanel(parent, wxID_ANY), 
          readOnly(readOnly) 
{
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

    name = new wxTextCtrl(this, wxID_ANY, scenario->GetName(bandNum), wxDefaultPosition, FromDIP(wxSize(NAME_INPUT_LEN, -1)), wxTE_PROCESS_ENTER);
    startValue = new wxTextCtrl(this, wxID_ANY, to_string(scenario->GetFreq(bandNum, 0)), wxDefaultPosition, FromDIP(wxSize(FREQ_INPUT_LEN, -1)), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_DIGITS));
    endValue = new wxTextCtrl(this, wxID_ANY, to_string(scenario->GetFreq(bandNum, 1)), wxDefaultPosition, FromDIP(wxSize(FREQ_INPUT_LEN, -1)), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_DIGITS));

    name->SetMaxLength(NAME_MAX_LENGTH);
    startValue->SetMaxLength(4);
    endValue->SetMaxLength(4);
    
    bool active = scenario->IsActive(bandNum);
    statBtn = new ColourfulBtn(this, active ? "ON" : "OFF");
    statBtn->SetForegroundColour(wxColour(active ? DARK_GREEN : *wxRED));

    if (readOnly) {
        name->Disable();
        startValue->Disable();
        endValue->Disable();
        statBtn->Disable();
    }
    else {
        name->SetClientData((void*)Name);
        startValue->SetClientData((void*)Start);
        endValue->SetClientData((void*)End);
        statBtn->SetClientData((void*)StatBtn);
    }

    SetUpSizers();
}

void BandRow::BindEventHandlers()
{
    Bind(wxEVT_SIZE, &BandRow::OnResize, this);

    if (!readOnly) {
        name->Bind(wxEVT_SET_FOCUS, &BandRow::OnTextCtrlFocus, this);
        startValue->Bind(wxEVT_SET_FOCUS, &BandRow::OnTextCtrlFocus, this);
        endValue->Bind(wxEVT_SET_FOCUS, &BandRow::OnTextCtrlFocus, this);

        name->Bind(wxEVT_CHAR_HOOK, &BandRow::OnKey, this);
        startValue->Bind(wxEVT_CHAR_HOOK, &BandRow::OnKey, this);
        endValue->Bind(wxEVT_CHAR_HOOK, &BandRow::OnKey, this);

        name->Bind(wxEVT_CONTEXT_MENU, &BandRow::EmptyHandler, this);
        startValue->Bind(wxEVT_CONTEXT_MENU, &BandRow::EmptyHandler, this);
        endValue->Bind(wxEVT_CONTEXT_MENU, &BandRow::EmptyHandler, this);

        statBtn->Bind(wxEVT_BUTTON, &BandRow::OnStatusChanged, this);
        statBtn->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);
        unfocused->Bind(wxEVT_SET_FOCUS, &BandRow::OnFocus, this);
    }
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
    startValue->SetValue(to_string(scenario->GetFreq(bandNum, 0)));
    endValue->SetValue(to_string(scenario->GetFreq(bandNum, 1)));

    bool active = scenario->IsActive(bandNum);
    statBtn->SetLabel(active ? "ON" : "OFF");
    statBtn->SetForegroundColour(wxColour(active ? DARK_GREEN : *wxRED));
}

string BandRow::Rename() {
    string newName = name->GetValue().ToUTF8().data();
    if (newName == scenario->GetName(bandNum)) {
        name->SetInsertionPoint(0);
        return ToString(Success);
    }
    string stat = scenario->Rename(newName, bandNum);
    if (!stat.empty()) {
        int res = ShowError(base, stat, DIALOG);
        if (res == wxID_CANCEL) {
            name->SetValue(scenario->GetName(bandNum));
            name->SetInsertionPoint(0);
            return ToString(Success);
        }
    }
    MarkUnsaved();
    return stat;
}

string BandRow::UpdateFreq(int freqToChange)
{
    assert(freqToChange == Start || freqToChange == End);
    string stat;
    wxTextCtrl* ctrl = (int)freqToChange == Start ? startValue : endValue;
    int i = (int)freqToChange == Start ? 0 : 1;
    int newValue;
    stat = Validation::TryParse(ctrl->GetValue(), &newValue) ? ToString(Success) : ToString(FreqNotPositiveNumber);
    if (!stat.empty()) {
        if (ShowError(base, stat, DIALOG) == wxID_CANCEL) {
            ctrl->SetValue(to_string(scenario->GetFreq(bandNum, i)));
            return ToString(Success);
        }
    }
    else {
        if (newValue == scenario->GetFreq(bandNum, i)) return ToString(Success);
        stat = scenario->SetFreq(bandNum, i, newValue);
        if (!stat.empty()) {
            if (ShowError(base, stat, DIALOG) == wxID_CANCEL) {
                ctrl->SetValue(to_string(scenario->GetFreq(bandNum, i)));
                return ToString(Success);
            }
        }
        else ctrl->SetValue(to_string(scenario->GetFreq(bandNum, i)));
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

bool BandRow::ProcessKey(int key)
{
    if (key != WXK_CONTROL && wxGetKeyState(WXK_CONTROL) && key != 'A' && key != 'C' && key != 'X' && key != 'V' && key != 'Z' && 
                                                            key != WXK_RIGHT && key != WXK_LEFT && key != WXK_BACK && key != WXK_DELETE)
        return true;
    int tabDir = wxGetKeyState(WXK_SHIFT);
    if ((key == WXK_TAB || key == WXK_RETURN || key == WXK_ESCAPE)) {
        bool updateSuccess;
        if ((int)focused->GetClientData() == Name)
            updateSuccess = Rename().empty();
        else
            updateSuccess = UpdateFreq((int)focused->GetClientData()).empty();

        if (updateSuccess) {
            wxWindow* cur = focused;
            focused = nullptr;
            if (key == WXK_TAB) {
                if (tabDir)
                    cur->Navigate(wxNavigationKeyEvent::IsBackward);
                else
                    cur->Navigate();
            }
            else
                Unfocus();
        }
        return true;
    }
    else return false;
}
