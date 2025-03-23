#pragma once

#include "Global.h"

class ColourfulBtn : public wxButton
{
private:
	wxStaticText* txt;
public:
	ColourfulBtn(wxWindow* parent, const wxString& label);
	void Disable();
	void SetLabel(const wxString& label) override;
	bool SetForegroundColour(const wxColour& colour) override;
};