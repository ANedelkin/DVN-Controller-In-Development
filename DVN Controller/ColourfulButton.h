#pragma once

#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

#include "Colours.h"

class ColourfulBtn : public wxButton
{
private:
	wxStaticText* txt;
public:
	enum Status {
		On,
		Off
	};
	ColourfulBtn(wxWindow* parent, const wxString& label);
	void SetStatus(Status status);
	void Disable();
	void SetLabel(const wxString& label) override;
	bool SetForegroundColour(const wxColour& colour) override;
};