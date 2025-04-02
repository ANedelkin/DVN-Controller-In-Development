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
		if (page->GetSource()->GetOldPath() == data->GetOldPath() && page->GetSource()->folder != "") {
			ErrorMessage(base, FileAlreadyOpen, 0, data->GetName().c_str(), page->GetSource()->GetName().c_str());
			return FileAlreadyOpen;
		}
	}

	return SideNotebook::NewPage(data);
}

void LoadsPanel ::OnDelete(wxCommandEvent& e)
{
	wxMessageDialog dialog(base, "If you delete a load you won't be able to get it back!", "Are you sure about that?", wxYES_NO | wxICON_EXCLAMATION);
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	if (dialog.ShowModal() == wxID_YES) {
		if (exists(target->GetSource()->GetOldPath())) {
			remove(target->GetSource()->GetOldPath());
		}
		Close(target);
	}
	else
		target->Refresh();
}

void LoadsPanel::OnClose(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	switch (SaveDialog(base, target->GetSource()->GetName()).ShowModal()) {
	case SaveDialog::ID_SAVE:
		if (Save(target, false))
			Close(target);
		break;
	case SaveDialog::ID_SKIP:
		Close(target);
		break;
	default:
		target->Refresh();
		break;
	}
}

void LoadsPanel::OnRename(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	NameSetter* nameSetter = new NameSetter(base, "Enter name", target->GetSource()->GetName()); //Create a different Load::ValidateName
	nameSetter->ShowModal();
	if (nameSetter->ok && target->GetSource()->GetName() != nameSetter->name) {
		target->GetSource()->Rename(nameSetter->name);
		target->SetLabel(nameSetter->name);
		Unsave(false, target);
	}
	target->Refresh();
}
