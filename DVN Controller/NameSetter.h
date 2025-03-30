#pragma once
#include "Global.h"

class NameSetter : public wxDialog
{
private:
	wxTextCtrl* input;

public:
	string name = "";
	bool ok = false;
	NameSetter(wxWindow* parent, const wxString& title, const string& defaultValue = "");
	Status ValidateName(const string& name);
	void OnOK(wxCommandEvent& e);
};

