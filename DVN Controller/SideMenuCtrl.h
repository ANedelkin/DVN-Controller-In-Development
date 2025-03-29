#pragma once
#include "Global.h"
#include "NameSetter.h"
#include "DVNFileData.h"

class SideMenuCtrl : public wxButton {
private:
	wxPanel* mainPanel;
	wxMenu* contextMenu = nullptr;
	DVNFileData* source;

	void OnContextMenu(wxMouseEvent& e);
	//void OnRename(wxCommandEvent& e);
	//void OnDelete(wxCommandEvent& eIn);
	//void OnClose(wxCommandEvent& eIn);
	//void OnSave(wxCommandEvent& e);
	//void OnLoad(wxCommandEvent& eIn);
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
	void OnRename(wxCommandEvent& e);
};