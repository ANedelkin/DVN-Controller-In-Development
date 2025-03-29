#pragma once

#include "Global.h"
#include "Load.h"
#include "SideNotebook.h"
#include "ScenariosPanel.h"

class LoadsPanel : public SideNotebook
{
public:
	LoadsPanel(wxWindow* parent);
	Status AddPage(Load* data);
	void OnDelete(wxCommandEvent& e);
	void OnClose(wxCommandEvent& e);
	void OnRename(wxCommandEvent& e);
};

