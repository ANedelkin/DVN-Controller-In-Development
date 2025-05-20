#pragma once

#include <wx/artprov.h>
#include <wx/filename.h>
#define _CRT_SECURE_NO_WARNINGS
#include <filesystem>

#include "Load.h"
#include "SideNotebook.h"
#include "LoadsPanelContent.h"

#include "penXPM.xpm"
#include "closeXPM.xpm"
#include "copyXPM.xpm"
#include "trashCanXPM.xpm"

using namespace filesystem;

class LoadsPanel : public SideNotebook
{
private:
	bool RecreateSource(SideMenuCtrl* page);
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
	void ChangeSelection(SideMenuCtrl* page) override;
	bool CheckForUnsaved() override;
};

