#pragma once
#include "Global.h"

class NameSetter : public wxDialog
{
private:
	wxTextCtrl* input;

	Status(*validator)(string& name);
public:
	string name = "";
	bool ok = false;
	NameSetter(wxWindow* parent, const wxString& title, Status(*validator)(string& name));
	void OnOK(wxCommandEvent& e);
};

