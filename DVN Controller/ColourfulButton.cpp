#include "ColourfulButton.h"

ColourfulBtn::ColourfulBtn(wxWindow* parent, const wxString& label) : wxButton(parent, wxID_ANY, label)
{
	SetBackgroundColour(wxColour(*wxWHITE));
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
	if (!IsEnabled()) txt->SetLabel(label);
	Layout();
}

bool ColourfulBtn::SetForegroundColour(const wxColour& colour)
{
	if (IsEnabled()) wxButton::SetForegroundColour(colour);
	else txt->SetForegroundColour(colour);
	return true;
}
