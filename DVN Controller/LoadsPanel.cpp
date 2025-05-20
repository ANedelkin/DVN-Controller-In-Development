#include "LoadsPanel.h"

void LoadsPanel::SaveCurrentAs()
{
	if (cur) SaveAs(cur);
}

LoadsPanel::LoadsPanel(wxWindow* parent) : SideNotebook(parent, "Loads", Load::ValidateName)
{
	contextMenu = new wxMenu();

	wxMenuItem* rename = new wxMenuItem(contextMenu, wxID_ANY, "Rename");
	rename->SetBitmap(wxBitmap(penXPM));
	contextMenu->Append(rename);
	contextMenu->Bind(wxEVT_MENU, &LoadsPanel::OnRename, this, rename->GetId());

	wxMenuItem* duplicate = new wxMenuItem(contextMenu, wxID_ANY, "Duplicate");
	duplicate->SetBitmap(wxBitmap(copyXPM));
	contextMenu->Append(duplicate);
	contextMenu->Bind(wxEVT_MENU, &LoadsPanel::OnDuplicate, this, duplicate->GetId());

	wxMenuItem* deleteItem = new wxMenuItem(contextMenu, wxID_DELETE, "Delete");
	deleteItem->SetBitmap(wxBitmap(trashCanXPM));
	contextMenu->Append(deleteItem);
	contextMenu->Bind(wxEVT_MENU, &LoadsPanel::OnDelete, this, deleteItem->GetId());

	wxMenuItem* closeItem = new wxMenuItem(contextMenu, wxID_CLOSE, "Close");
	closeItem->SetBitmap(wxBitmap(closeXPM));
	contextMenu->Append(closeItem);
	contextMenu->Bind(wxEVT_MENU, &LoadsPanel::OnClose, this, closeItem->GetId());

	LoadsPanelContent* scenPanel = new LoadsPanelContent(this);
	scenPanel->UnInit();
	SetContent(scenPanel);
}

StatusCode LoadsPanel::NewPage(Load* data)
{
	for (SideMenuCtrl* page : pages) {
		if (page->GetSource()->GetPath() == data->GetPath() && page->GetSource()->folder != "") {
			ShowError(base, ToString(FileAlreadyOpen, data->GetName().c_str(), page->GetSource()->GetName().c_str()));
			return FileAlreadyOpen;
		}
	}

	return SideNotebook::NewPage(data);
}

void LoadsPanel::ChangeSelection(SideMenuCtrl* page)
{
	SideNotebook::ChangeSelection(page);
	((LoadsPanelContent*)content)->Select(0);
}

bool LoadsPanel::CheckForUnsaved()
{
	for (char i = 0; i < pages.size(); i++)
	{
		if (((Load*)pages[i]->GetSource())->AlteredFromOutside())
			return RecreateSource(pages[i]);
	}
	return SideNotebook::CheckForUnsaved();
}

bool LoadsPanel::RecreateSource(SideMenuCtrl* page)
{
	switch (SaveDialog(this, "\"" + page->GetSource()->GetName() + "\"" + " was changed from outside the program. If you don't save it somewhere else, it will be lost.").ShowModal()) {
	case SaveDialog::ID_SAVE:
		if (!SaveAs(page)) return false;
		page->MarkSaved();
		return true;
	case SaveDialog::ID_SKIP:
		Close(page);
		return false;
	case SaveDialog::ID_CANCEL:
		return false;
	}
	return false;
}

bool LoadsPanel::Save(SideMenuCtrl* page)
{
	Load* curData = dynamic_cast<Load*>(page->GetSource());
	assert(curData != nullptr && "Page's data is not load or derived.");
	bool f = true;
	if (curData->AlteredFromOutside())
		return RecreateSource(page);
	if (curData->folder == "")
		f = SaveAs(page);
	if (f) {
		SideNotebook::Save(page);
		page->MarkSaved();
	}

	return f;
}

bool LoadsPanel::SaveAs(SideMenuCtrl* page)
{
	DVNFileData* curData = page->GetSource();
	wxFileDialog dialog(this, "Select a folder to save \"" + curData->GetName() + "\"", "", curData->GetNameWithExt(), "Load files (*.dvnl)|*.dvnl", wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
	if (dialog.ShowModal() == wxID_OK) {
		string name = wxFileName(dialog.GetPath()).GetName().ToStdString();
		string folder = dialog.GetDirectory().ToStdString();
		for (SideMenuCtrl* existing : pages) {
			if (existing == page) continue;
			if (existing->GetSource()->GetPath() == folder + "\\" + name + existing->GetSource()->GetExtension()) {
				ShowError(base, ToString(FileAlreadyOpen, name.c_str()));
				return false;
			}
		}
		curData->Rename(name);
		page->SetLabel(name);
		curData->folder = folder;
		Save(page);
	}
	else return false;
	
	return true;
}

void LoadsPanel::OnDuplicate(wxCommandEvent& e)
{
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	Duplicate(target);
	target->Refresh();
}

void LoadsPanel::OnDelete(wxCommandEvent& e)
{
	wxMessageDialog dialog(base, "If you delete a load you won't be able to get it back!", "Are you sure about that?", wxYES_NO | wxICON_EXCLAMATION);
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	if (dialog.ShowModal() == wxID_YES) {
		if (exists(target->GetSource()->GetPath())) {
			remove(target->GetSource()->GetPath());
		}
		Close(target);
	}
	else
		target->Refresh();
}

void LoadsPanel::OnClose(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	if (!target->GetSource()->upToDate) {
		switch (SaveDialog(base, "\"" + target->GetSource()->GetName() + "\"" + " is unsaved, how would you like to proceed?").ShowModal()) {
		case SaveDialog::ID_SAVE:
			if (Save(target))
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
	else
		Close(target);
}

void LoadsPanel::OnRename(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	Rename(target);
	target->Refresh();
}