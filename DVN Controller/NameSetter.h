#pragma once
#include "Global.h"

class NameSetter : public wxDialog
{
private:
	wxTextCtrl* input;

	Status(*validator)(const string& name);

	void OnOK(wxCommandEvent& e);
public:
	string name = "";
	bool ok = false;
	
	NameSetter(wxWindow* parent, const wxString& title, Status(*validator)(const string& name), const string& defaultValue = "", const string& oldName = "\001");
};

