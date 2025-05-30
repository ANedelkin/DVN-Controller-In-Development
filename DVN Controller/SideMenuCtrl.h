#pragma once

#include <wx/window.h>
#include <wx/font.h>
#include <wx/menu.h>
#include <wx/button.h>
#include <wx/dcclient.h>

#include "Global.h"
#include "NameSetter.h"
#include "DVNFileData.h"
#include "LayoutConstants.h"
#include "Events.h"

class SideMenuCtrl : public wxButton {
private:
	DVNFileData* source;
	
	wxPanel* mainPanel;
	wxMenu* contextMenu = nullptr;

	void OnKey(wxKeyEvent& e);
	void OnFocus(wxFocusEvent& e);
	void OnContextMenu(wxContextMenuEvent& e);
	void PopUpContextMenu(const wxPoint& position);
public:
	SideMenuCtrl(wxWindow* parent, wxPanel* mainPanel, DVNFileData* source);

	DVNFileData* GetSource();
	void SetSource(DVNFileData* source);
	void SetContextMenu(wxMenu* menu);
	void Unsave();
	void Save();
	void MarkSaved();
	void MarkUnsaved();
	void SetLabel(const wxString& label) override;
};