#pragma once

#include "Global.h"
#include "Load.h"
#include "SideNotebook.h"
#include "LoadsPanelContent.h"

class LoadsPanel : public SideNotebook
{
private:
	bool Save(SideMenuCtrl* page) override;
	bool SaveAs(SideMenuCtrl* page);
	void OnDuplicate(wxCommandEvent& e);
	void OnDelete(wxCommandEvent& e);
	void OnClose(wxCommandEvent& e);
	void OnRename(wxCommandEvent& e);
public:
	void SaveCurrentAs();
	LoadsPanel(wxWindow* parent);
	StatusCode NewPage(Load* data);
};

