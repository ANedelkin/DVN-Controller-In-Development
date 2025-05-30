#pragma once

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>

#include "Global.h"
#include "Status.h"

class NameSetter : public wxDialog
{
private:
	wxTextCtrl* input;

	string(*validator)(const string& name);

	void OnOK(wxCommandEvent& e);
public:
	string name = "";
	bool ok = false;
	
	NameSetter(wxWindow* parent, const wxString& title, string(*validator)(const string& name), const string& defaultValue = "");
};

