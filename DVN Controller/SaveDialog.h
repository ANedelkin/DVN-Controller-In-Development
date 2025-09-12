#pragma once

#include <wx/wx.h>
#define _CRT_SECURE_NO_WARNINGS
#include <string>

using namespace std;

#include "TrimToWidth.h"

#define SAVING_MANY 0b00000010

class SaveDialog : public wxDialog
{
public:
    enum
    {
        ID_CANCEL = wxID_CANCEL,
        ID_SAVE = wxID_HIGHEST + 1,
        ID_SKIP,
        ID_SAVE_ALL,
        ID_SKIP_ALL,
    };

    SaveDialog(wxWindow* parent, const string& messageTxt, const string& nameTxt, char style = 0)
        : wxDialog(parent, wxID_ANY, "Unsaved file")
    {
        SetSize(FromDIP(wxSize(400, -1)));

        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        wxPanel* content = new wxPanel(this);
        wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

        wxPanel* text = new wxPanel(content);
        wxBoxSizer* textSizer = new wxBoxSizer(wxVERTICAL);
        AddText(messageTxt, text, textSizer);
        AddText("\"" + TrimToWidth(nameTxt, 350, wxFont(17, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM, false, "Segoe UI")) + "\"", text, textSizer);
        text->SetSizerAndFit(textSizer);
        contentSizer->Add(text, 0, wxEXPAND | wxALL, FromDIP(15));

        wxButton* saveButton = new wxButton(content, ID_SAVE, "Save");
        wxButton* skipButton = new wxButton(content, ID_SKIP, "Don't save");
        wxButton* cancelButton = new wxButton(content, ID_CANCEL, "Cancel");

        wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
        horizontalSizer->Add(saveButton, 1, wxALL, FromDIP(5));
        horizontalSizer->Add(skipButton, 1, wxALL, FromDIP(5));
        horizontalSizer->Add(cancelButton, 1, wxALL, FromDIP(5));
        contentSizer->Add(horizontalSizer, 0, wxEXPAND, FromDIP(10));

        mainSizer->Add(content, 1, wxEXPAND | wxALL, FromDIP(10));

        if (style & SAVING_MANY) {
            wxButton* saveAllButton = new wxButton(content, ID_SAVE_ALL, "Save all");
            wxButton* skipAllButton = new wxButton(content, ID_SKIP_ALL, "Don't save any");

            contentSizer->Add(saveAllButton, 0, wxEXPAND | wxALL, FromDIP(5));
            contentSizer->Add(skipAllButton, 0, wxEXPAND | wxALL, FromDIP(5));
        }

        content->SetSizerAndFit(contentSizer);
        SetSizerAndFit(mainSizer);

        Connect(wxEVT_BUTTON, wxCommandEventHandler(SaveDialog::OnButtonClick));
    }

private:
    void OnButtonClick(wxCommandEvent& e)
    {
        EndModal(e.GetId());
    }
    void AddText(const wxString& text, wxWindow* parent, wxBoxSizer* sizer) {
        wxStaticText* staticText = new wxStaticText(parent, wxID_ANY, text);
        staticText->SetWindowStyle(wxALIGN_CENTER);
        staticText->SetFont(wxFont(17, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM, false, "Segoe UI"));
        staticText->SetForegroundColour(wxColour(65, 103, 179));
        staticText->Wrap(GetSize().GetWidth());
        sizer->Add(staticText, 0, wxALIGN_CENTER);
    }
};
