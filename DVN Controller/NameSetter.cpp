#pragma once

#include "NameSetter.h"


NameSetter::NameSetter(wxWindow* parent, const wxString& title, Status(*validator)(const string& name), const string& defaultValue) 
          : wxDialog(parent, wxID_ANY, title)
{
    SetSize(FromDIP(wxSize(250, 190)));
    this->validator = validator;

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
    string temp = input->GetValue().ToStdString();
    Status stat = validator(temp);
    if (stat) {
        wxMessageBox(errorMessages[stat], "Error", wxOK | wxICON_ERROR, this);
        return;
    }
    name = temp;
    ok = true;
    this->Close();
}