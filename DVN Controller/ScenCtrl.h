#pragma once
#include "Global.h"
#include "Scenario.h"
#include "NameSetter.h"

class ScenCtrl : public wxButton {
private:
	wxPanel* mainPanel;

	wxMenu* contextMenu;
	
	void OnContextMenu(wxMouseEvent& e) {
		PopupMenu(contextMenu, e.GetPosition());

	}
	void OnRename(wxCommandEvent& e) {
		NameSetter* nameSetter = new NameSetter(base, "Enter scenario name", Scenario::ValidateName);
		nameSetter->ShowModal();
		if (nameSetter->ok) {
			scenario->Rename(nameSetter->name);
			SetLabel(nameSetter->name);
		}
	}
	void OnDelete(wxCommandEvent& eIn) {
		wxCommandEvent eOut(wxEVT_COMMAND_MENU_SELECTED);
		eOut.SetEventObject(this);
		mainPanel->GetEventHandler()->ProcessEvent(eOut);
	}
public:
	Scenario* scenario;
	ScenCtrl(wxWindow* parent, wxPanel* mainPanel, Scenario* scenario) : wxButton(parent, wxID_ANY, scenario->GetName(), wxDefaultPosition, wxSize(-1, 43)) {
		this->scenario = scenario;
		this->mainPanel = mainPanel;
		SetBackgroundColour(wxColour(255, 255, 255));

		contextMenu = new wxMenu();

		wxMenuItem* rename = new wxMenuItem(contextMenu, wxID_ANY, "Rename");
		wxMenuItem* deleteItem = new wxMenuItem(contextMenu, wxID_DELETE, "Delete");

		rename->SetBitmap(wxArtProvider::GetBitmap(wxART_EDIT));
		deleteItem->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE));

		contextMenu->Append(rename);
		contextMenu->Append(deleteItem);

		contextMenu->Bind(wxEVT_MENU, &ScenCtrl::OnRename, this, rename->GetId());
		contextMenu->Bind(wxEVT_MENU, &ScenCtrl::OnDelete, this, deleteItem->GetId());


		Bind(wxEVT_RIGHT_UP, &ScenCtrl::OnContextMenu, this);
	}
};