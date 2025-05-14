#pragma once

#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/sizer.h>

#include "ModelConstants.h"
#include "Status.h"
#include "ftd2xx.h"

class JammersWindow : public wxDialog
{
private:
	char serNum[9] = {};

	wxListBox* list;
	wxButton* select;

	void OnEnterPressed(wxKeyEvent& e);
	void OnSelectClicked(wxCommandEvent& e);
	void OnRefreshClicked(wxCommandEvent& e);

	void LoadJammers();
	void Select();
public:
	JammersWindow(wxWindow* parent);
	char* GetSerNum();
};

