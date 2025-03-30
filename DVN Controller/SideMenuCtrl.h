#pragma once
#include "Global.h"
#include "NameSetter.h"
#include "DVNFileData.h"

class SideMenuCtrl : public wxButton {
private:
	DVNFileData* source;
	
	wxPanel* mainPanel;
	wxMenu* contextMenu = nullptr;

	void OnContextMenu(wxContextMenuEvent& e);
public:
	SideMenuCtrl(wxWindow* parent, wxPanel* mainPanel, DVNFileData* source);

	DVNFileData* GetSource();
	void SetSource(DVNFileData* source);
	void SetContextMenu(wxMenu* menu);
	void Refresh();
	void Unsave();
	void Save();
	void MarkSaved();
	void MarkUnsaved();
	void SetLabel(const wxString& label) override;

	//void OnRename(wxCommandEvent& e);
};