#pragma once
#include "Global.h"
#include "NameSetter.h"
#include "DVNFileData.h"

class SideMenuCtrl : public wxButton {
private:
	wxPanel* mainPanel;

	wxMenu* contextMenu;

	DVNFileData* source;

	void OnContextMenu(wxMouseEvent& e);
	void OnRename(wxCommandEvent& e);
	void OnDelete(wxCommandEvent& eIn);
	void OnSave(wxCommandEvent& e);
public:
	SideMenuCtrl(wxWindow* parent, wxPanel* mainPanel, DVNFileData* source, bool subMen);

	DVNFileData* GetSource();
	void ChangeSource(DVNFileData* source);
	void Unsave();
	void Save();
	void MarkSaved();
	void MarkUnsaved();
};