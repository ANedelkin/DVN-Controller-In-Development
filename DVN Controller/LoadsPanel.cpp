#include "LoadsPanel.h"

LoadsPanel::LoadsPanel(wxWindow* parent) : SideNotebook(parent, "Loads", nullptr)
{
	contextMenu = new wxMenu();

	wxMenuItem* rename = new wxMenuItem(contextMenu, wxID_ANY, "Rename");
	rename->SetBitmap(wxArtProvider::GetBitmap(wxART_EDIT));
	contextMenu->Append(rename);
	contextMenu->Bind(wxEVT_MENU, &LoadsPanel::OnRename, this, rename->GetId());

	wxMenuItem* deleteItem = new wxMenuItem(contextMenu, wxID_DELETE, "Delete");
	deleteItem->SetBitmap(wxArtProvider::GetBitmap(wxART_DELETE));
	contextMenu->Append(deleteItem);
	contextMenu->Bind(wxEVT_MENU, &LoadsPanel::OnDelete, this, deleteItem->GetId());

	wxMenuItem* closeItem = new wxMenuItem(contextMenu, wxID_CLOSE, "Close");
	closeItem->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE));
	contextMenu->Append(closeItem);
	contextMenu->Bind(wxEVT_MENU, &LoadsPanel::OnClose, this, closeItem->GetId());

	ScenariosPanel* scenPanel = new ScenariosPanel(this, new Load(), LOADABLE);
	scenPanel->Select(0);
	scenPanel->UnInit();
	SetContent(scenPanel);
}

Status LoadsPanel::AddPage(Load* data)
{
	for (SideMenuCtrl* page : pages) {
		if (page->GetSource()->GetNewPath() == data->GetNewPath() && page->GetSource()->folder != "") {
			ErrorMessage(base, ScenarioAlreadyExists, data->GetName().c_str());
			return ScenarioAlreadyExists;
		}
	}

	return SideNotebook::NewPage(data);
}

void LoadsPanel ::OnDelete(wxCommandEvent& e)
{
	wxMessageDialog dialog(base, "If you delete a load you won't be able to get it back!", "Are you sure about that?", wxYES_NO | wxICON_EXCLAMATION);
	if (dialog.ShowModal() == wxID_YES) {
		SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
		if (exists(target->GetSource()->GetOldPath())) {
			remove(target->GetSource()->GetOldPath());
		}
		Close(target);
	}
}

void LoadsPanel::OnClose(wxCommandEvent& e) {
	Close((SideMenuCtrl*)contextMenu->GetInvokingWindow());
}

void LoadsPanel::OnRename(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	NameSetter* nameSetter = new NameSetter(base, "Enter name", DVNFileData::ValidateName, target->GetSource()->GetName()); //Create a different Load::ValidateName
	nameSetter->ShowModal();
	if (nameSetter->ok && target->GetSource()->GetName() != nameSetter->name) {
		target->GetSource()->Rename(nameSetter->name);
		target->SetLabel(nameSetter->name);
		target->Unsave();
		MarkUnsaved();
	}
}
