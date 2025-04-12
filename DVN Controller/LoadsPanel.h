#pragma once

#include "Global.h"
#include "Load.h"
#include "SideNotebook.h"
#include "LoadsPanelContent.h"

class LoadsPanel : public SideNotebook
{
private:
	void OnDelete(wxCommandEvent& e);
	void OnClose(wxCommandEvent& e);
	void OnRename(wxCommandEvent& e);
public:
	LoadsPanel(wxWindow* parent);
	StatusCode AddPage(Load* data);
};

