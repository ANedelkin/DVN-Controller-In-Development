#pragma once
#include "Global.h"

class NameSetter : public wxDialog
{
private:
	wxTextCtrl* input;

	Status(*validator)(const string& name);
public:
	string name = "";
	bool ok = false;
	NameSetter(wxWindow* parent, const wxString& title, Status(*validator)(const string& name), const string& defaultValue = "");
	void OnOK(wxCommandEvent& e);
};

