#pragma once

#include "Global.h"
#include "Load.h"
#include "SideNotebook.h"
#include "ScenariosPanel.h"

class LoadsPanel : public SideNotebook
{
private:
	void OnDelete(wxCommandEvent& e);
	void OnClose(wxCommandEvent& e);
	void OnRename(wxCommandEvent& e);
public:
	LoadsPanel(wxWindow* parent);
	Status AddPage(Load* data);
};

