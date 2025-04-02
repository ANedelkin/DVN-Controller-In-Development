#pragma once

#include "NameSetter.h"


NameSetter::NameSetter(wxWindow* parent, const wxString& title, Status(*validator)(const string& name), const string& defaultValue, const string& oldName)
          : wxDialog(parent, wxID_ANY, title)
          , validator(validator)
          , name(oldName)
{
    SetSize(FromDIP(wxSize(250, 190)));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* inputPanel = new wxPanel(this);
    wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);

    //wxStaticText* text = new wxStaticText(inputPanel, wxID_ANY, "Enter name:", wxPoint(20, 20));
    input = new wxTextCtrl(inputPanel, wxID_ANY, defaultValue, FromDIP(wxPoint(20, 50)), FromDIP(wxSize(200, 25)));

    //inputSizer->Add(text, 0, wxALIGN_CENTER | wxALL, 10);
    inputSizer->Add(input, 1, wxALIGN_CENTER | wxALL, FromDIP(10));
    inputPanel->SetSizerAndFit(inputSizer);

    wxPanel* buttonsPanel = new wxPanel(this);
    wxBoxSizer* buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* okButton = new wxButton(buttonsPanel, wxID_OK, "OK", FromDIP(wxPoint(20, 90)));
    wxButton* cancelButton = new wxButton(buttonsPanel, wxID_CANCEL, "Cancel", FromDIP(wxPoint(120, 90)));

    buttonsSizer->AddStretchSpacer();
    buttonsSizer->Add(okButton);
    buttonsSizer->Add(cancelButton);
    buttonsPanel->SetSizerAndFit(buttonsSizer);

    sizer->Add(inputPanel, 1, wxTOP | wxBOTTOM, FromDIP(10));
    sizer->Add(buttonsPanel, 0, wxEXPAND | wxBOTTOM | wxRIGHT, FromDIP(10));
    SetSizerAndFit(sizer);

    okButton->SetDefault();
    okButton->Bind(wxEVT_BUTTON, &NameSetter::OnOK, this);
}

void NameSetter::OnOK(wxCommandEvent& e) {
    string newName = input->GetValue().ToUTF8().data();
    if (newName == this->name) {
        ok = true;
        Close();
        return;
    }
    Status stat = validator(newName);
    if (stat) {
        if (stat == NameTooLong)
            ErrorMessage(this, stat, 0, NAME_MAX_LENGTH);
        else if (stat == ScenarioAlreadyExists)
            ErrorMessage(this, stat, 0, newName.c_str());
        else 
            ErrorMessage(this, stat, 0);
        return;
    }
    this->name = newName;
    ok = true;
    Close();
}