#pragma once
#include "Global.h"
#include "NameSetter.h"
#include "DVNFileData.h"

class SideMenuCtrl : public wxButton {
private:
	wxPanel* mainPanel;

	wxMenu* contextMenu;
	
	void OnContextMenu(wxMouseEvent& e) {
		PopupMenu(contextMenu, e.GetPosition());

	}
	void OnRename(wxCommandEvent& e) {
		NameSetter* nameSetter = new NameSetter(base, "Enter scenario name", DVNFileData::ValidateName);
		nameSetter->ShowModal();
		if (nameSetter->ok) {
			data->Rename(nameSetter->name);
			SetLabel(nameSetter->name);
		}
	}
	void OnDelete(wxCommandEvent& eIn) {
		wxCommandEvent eOut(wxEVT_COMMAND_MENU_SELECTED);
		eOut.SetEventObject(this);
		mainPanel->GetEventHandler()->ProcessEvent(eOut);
	}
public:
	DVNFileData* data;
	SideMenuCtrl(wxWindow* parent, wxPanel* mainPanel, DVNFileData* data) : wxButton(parent, wxID_ANY, data->GetName(), wxDefaultPosition, wxSize(-1, 43)) {
		this->data = data;
		this->mainPanel = mainPanel;
		SetBackgroundColour(wxColour(255, 255, 255));

		contextMenu = new wxMenu();

		wxMenuItem* rename = new wxMenuItem(contextMenu, wxID_ANY, "Rename");
		wxMenuItem* deleteItem = new wxMenuItem(contextMenu, wxID_DELETE, "Delete");

		rename->SetBitmap(wxArtProvider::GetBitmap(wxART_EDIT));
		deleteItem->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE));

		contextMenu->Append(rename);
		contextMenu->Append(deleteItem);

		contextMenu->Bind(wxEVT_MENU, &SideMenuCtrl::OnRename, this, rename->GetId());
		contextMenu->Bind(wxEVT_MENU, &SideMenuCtrl::OnDelete, this, deleteItem->GetId());


		Bind(wxEVT_RIGHT_UP, &SideMenuCtrl::OnContextMenu, this);
	}
};