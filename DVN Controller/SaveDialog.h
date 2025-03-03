#pragma once

#include "Global.h"

class SaveDialog : public wxDialog
{
public:
    enum
    {
        ID_SAVE = wxID_HIGHEST + 1,
        ID_SKIP,
        ID_SAVE_ALL,
        ID_SKIP_ALL,
        ID_CANCEL
    };

    SaveDialog(wxWindow* parent, const string& fileName)
        : wxDialog(parent, wxID_ANY, "Unsaved file")
    {
        wxPanel* content = new wxPanel(this);
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* message = new wxStaticText(
            content, 
            wxID_ANY, 
            "\"" + fileName + "\"" + " is unsaved, how would you like to proceed?"
        );
        message->SetWindowStyle(wxALIGN_CENTER);
        message->SetFont(wxFont(FromDIP(15), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM, false, "Segoe UI"));
        message->SetForegroundColour(wxColour(65, 103, 179));
        message->Wrap(GetSize().GetWidth());
        //mainSizer->AddStretchSpacer(1);
        verticalSizer->Add(message, 1, wxEXPAND | wxALIGN_CENTER | wxALL, FromDIP(10));
        //mainSizer->AddStretchSpacer(1);

        wxButton* saveButton = new wxButton(content, ID_SAVE, "Save");
        wxButton* skipButton = new wxButton(content, ID_SKIP, "Don't save");
        wxButton* cancelButton = new wxButton(content, ID_CANCEL, "Cancel");
        wxButton* saveAllButton = new wxButton(content, ID_SAVE_ALL, "Save all unsaved files");
        wxButton* skipAllButton = new wxButton(content, ID_SKIP_ALL, "Don't save any unsaved files");

        wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
        horizontalSizer->Add(saveButton, 1, wxALL, FromDIP(5));
        horizontalSizer->Add(skipButton, 1, wxALL, FromDIP(5));
        horizontalSizer->Add(cancelButton, 1, wxALL, FromDIP(5));
        verticalSizer->Add(horizontalSizer, 0, wxEXPAND, FromDIP(10));

        verticalSizer->Add(saveAllButton, 0, wxEXPAND | wxALL, FromDIP(5));
        verticalSizer->Add(skipAllButton, 0, wxEXPAND | wxALL, FromDIP(5));

        content->SetSizerAndFit(verticalSizer);
        mainSizer->Add(content, 1, wxEXPAND | wxALL, FromDIP(10));

        SetSizerAndFit(mainSizer);

        saveButton->Bind(wxEVT_BUTTON, &SaveDialog::OnSave, this);
        skipButton->Bind(wxEVT_BUTTON, &SaveDialog::OnSkip, this);
        saveAllButton->Bind(wxEVT_BUTTON, &SaveDialog::OnSaveAll, this);
        skipAllButton->Bind(wxEVT_BUTTON, &SaveDialog::OnSkipAll, this);
        cancelButton->Bind(wxEVT_BUTTON, &SaveDialog::OnCancel, this);
    }

private:
    void OnSave(wxCommandEvent& event)
    {
        EndModal(ID_SAVE);
    }

    void OnSkip(wxCommandEvent& event)
    {
        EndModal(ID_SKIP);
    }

    void OnSaveAll(wxCommandEvent& event)
    {
        EndModal(ID_SAVE_ALL);
    }

    void OnSkipAll(wxCommandEvent& event)
    {
        EndModal(ID_SKIP_ALL);
    }

    void OnCancel(wxCommandEvent& event)
    {
        EndModal(ID_CANCEL);
    }
};