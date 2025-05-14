#pragma once

#include <wx/artprov.h>
#define _CRT_SECURE_NO_WARNINGS
#include <filesystem>

#include "Global.h"
#include "Scenario.h"
#include "Load.h"
#include "BandsPanel.h"
#include "SideNotebook.h"
#include "ScenSelectDialog.h"

using namespace filesystem;

class ScenariosPanel : public SideNotebook
{
private:
	char style;

	void OnLoad(wxCommandEvent& e);
	void OnSave(wxCommandEvent& e);
	void OnDuplicate(wxCommandEvent& e);
	void OnDelete(wxCommandEvent& e);
	void OnRename(wxCommandEvent& e);
public:
	ScenariosPanel(wxWindow* parent, const char style = 0);
	StatusCode NewPage(DVNFileData* data) override;

	friend class LoadsPanelContent;
};

