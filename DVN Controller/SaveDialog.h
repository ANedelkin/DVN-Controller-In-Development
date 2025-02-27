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
        : wxDialog(parent, wxID_ANY, "Unsaved file", wxDefaultPosition, wxSize(300, 200))
    {
        wxPanel* panel = new wxPanel(this);
        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

        wxStaticText* message = new wxStaticText(panel, wxID_ANY, fileName + " is unsaved, how would you like to proceed?");
        vbox->Add(message, 0, wxEXPAND | wxALL, 10);

        wxButton* saveButton = new wxButton(panel, ID_SAVE, "Save");
        wxButton* skipButton = new wxButton(panel, ID_SKIP, "Skip");
        wxButton* saveAllButton = new wxButton(panel, ID_SAVE_ALL, "Save All");
        wxButton* skipAllButton = new wxButton(panel, ID_SKIP_ALL, "Skip All");
        wxButton* cancelButton = new wxButton(panel, ID_CANCEL, "Cancel");

        wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
        hbox->Add(saveButton, 0, wxRIGHT, 5);
        hbox->Add(skipButton, 0, wxRIGHT, 5);
        hbox->Add(saveAllButton, 0, wxRIGHT, 5);
        hbox->Add(skipAllButton, 0, wxRIGHT, 5);
        hbox->Add(cancelButton, 0);

        vbox->Add(hbox, 0, wxALIGN_CENTER | wxBOTTOM, 10);

        panel->SetSizer(vbox);

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