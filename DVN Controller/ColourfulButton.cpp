#include "ColourfulButton.h"

ColourfulBtn::ColourfulBtn(wxWindow* parent, const wxString& label) : wxButton(parent, wxID_ANY, label)
{
	SetBackgroundColour(wxColour(*wxWHITE));
}

void ColourfulBtn::SetStatus(Status status)
{
	if (status) { //Off
		SetForegroundColour(*wxBLACK);
		SetLabel("OFF");
	}
	else { //On
		SetForegroundColour(DARK_GREEN);
		SetLabel("ON");
	}

}

void ColourfulBtn::Disable()
{
	wxButton::Disable();

	txt = new wxStaticText(this, wxID_ANY, GetLabel(), wxDefaultPosition, wxDefaultSize);
	SetBackgroundColour(txt->GetBackgroundColour());
	txt->SetForegroundColour(GetForegroundColour());

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->AddStretchSpacer(1);
	sizer->Add(txt, 0, wxALIGN_CENTER);
	sizer->AddStretchSpacer(1);
	SetSizer(sizer);
}

void ColourfulBtn::SetLabel(const wxString& label) {
	wxButton::SetLabel(label);
	if (txt) {
		txt->SetLabel(label);
		Layout();
	}
}

bool ColourfulBtn::SetForegroundColour(const wxColour& colour)
{
	if (txt) txt->SetForegroundColour(colour);
	else wxButton::SetForegroundColour(colour);
	return true;
}
