#include "ModelConstants.h"
#include "GetExeVersion.h"

class AboutDialog : public wxDialog {
public:
    AboutDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, "About the program") {
        SetSize(FromDIP(wxSize(300, 200)));

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        wxStaticText* aboutText = new wxStaticText(this, wxID_ANY,
            wxString::Format("%s Controller\nVersion: %s\nThis is a program for creating configurations for the %s jammer.", JAMMER_NAME, GetExeVersion(), JAMMER_NAME),
            FromDIP(wxPoint(20, 20)));
        sizer->Add(aboutText, 0, wxALL, 10);
        SetSizerAndFit(sizer);

        aboutText->Bind(wxEVT_KEY_DOWN, &AboutDialog::OnKeyDown, this);
    }

private:
    void OnKeyDown(wxKeyEvent& e) {
        if (e.GetKeyCode() == WXK_ESCAPE)
            Close();
        else
            e.Skip();
    }
};
