#pragma once
#include "Global.h"
#include "NameSetter.h"
#include "DVNFileData.h"

class SideMenuCtrl : public wxButton {
private:
	wxPanel* mainPanel;

	wxMenu* contextMenu;

	DVNFileData* source;

	void OnContextMenu(wxMouseEvent& e) {
		PopupMenu(contextMenu, e.GetPosition());

	}
	void OnRename(wxCommandEvent& e) {
		NameSetter* nameSetter = new NameSetter(base, "Enter name", DVNFileData::ValidateName);
		nameSetter->ShowModal();
		if (nameSetter->ok) {
			source->Rename(nameSetter->name);
			SetLabel(nameSetter->name);
		}
	}
	void OnDelete(wxCommandEvent& eIn) {
		wxCommandEvent eOut(EVT_DELETE);
		eOut.SetEventObject(this);
		mainPanel->GetEventHandler()->ProcessEvent(eOut);
	}
	void OnSave(wxCommandEvent& e) {
		source->Save();
	}
public:
	SideMenuCtrl(wxWindow* parent, wxPanel* mainPanel, DVNFileData* source, bool subMenu) : wxButton(parent, wxID_ANY, source->GetName()) {
		SetMinSize(FromDIP(wxSize(-1, 40)));

		this->source = source;
		this->mainPanel = mainPanel;
		SetBackgroundColour(wxColour(255, 255, 255));

		contextMenu = new wxMenu();

		wxMenuItem* rename = new wxMenuItem(contextMenu, wxID_ANY, "Rename");
		rename->SetBitmap(wxArtProvider::GetBitmap(wxART_EDIT));
		contextMenu->Append(rename);
		contextMenu->Bind(wxEVT_MENU, &SideMenuCtrl::OnRename, this, rename->GetId());

		if (subMenu) {
			wxMenuItem* save = new wxMenuItem(contextMenu, wxID_ANY, "Save as template");
			save->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE));
			contextMenu->Append(save);
			contextMenu->Bind(wxEVT_MENU, &SideMenuCtrl::OnSave, this, save->GetId());
		}
		else {
			wxMenuItem* deleteItem = new wxMenuItem(contextMenu, wxID_DELETE, "Delete");
			deleteItem->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE));
			contextMenu->Append(deleteItem);
			contextMenu->Bind(wxEVT_MENU, &SideMenuCtrl::OnDelete, this, deleteItem->GetId());
		}


		Bind(wxEVT_RIGHT_UP, &SideMenuCtrl::OnContextMenu, this);
	}

	DVNFileData* GetSource() { return source; }
	void ChangeSource(DVNFileData* source) {
		this->source = source;
		SetLabel(source->GetName());
	}

	void Unsave() {
		source->upToDate = false;
		wxFont newFont = GetFont();
		newFont.SetWeight(wxFONTWEIGHT_BOLD);
		SetFont(newFont);
	}
};