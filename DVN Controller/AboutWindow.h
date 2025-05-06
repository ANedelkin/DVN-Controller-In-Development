#include "Global.h"

class AboutDialog : public wxDialog {
public:
    AboutDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, "About the program") {
        SetSize(FromDIP(wxSize(300, 200)));

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        wxStaticText* aboutText = new wxStaticText(this, wxID_ANY,
            format("{0} Controller\nVersion {1}\nThis is a program for creating configurations for the {0} jammer.", JAMMER_NAME, VERSION),
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
