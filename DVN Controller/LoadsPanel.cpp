#include "LoadsPanel.h"

void LoadsPanel::SaveCurrentAs()
{
	if (cur) SaveAs(cur);
}

LoadsPanel::LoadsPanel(wxWindow* parent) : SideNotebook(parent, "Loads", Load::ValidateName)
{
	contextMenu = new wxMenu();

	wxMenuItem* rename = new wxMenuItem(contextMenu, wxID_ANY, "Rename");
	rename->SetBitmap(wxBitmapBundle::FromSVG(penSVG, CONTEXT_MENU_ICON_SIZE));
	contextMenu->Append(rename);
	contextMenu->Bind(wxEVT_MENU, &LoadsPanel::OnRename, this, rename->GetId());

	wxMenuItem* duplicate = new wxMenuItem(contextMenu, wxID_ANY, "Duplicate");
	duplicate->SetBitmap(wxBitmapBundle::FromSVG(copySVG, CONTEXT_MENU_ICON_SIZE));
	contextMenu->Append(duplicate);
	contextMenu->Bind(wxEVT_MENU, &LoadsPanel::OnDuplicate, this, duplicate->GetId());

	wxMenuItem* deleteItem = new wxMenuItem(contextMenu, wxID_DELETE, "Delete");
	deleteItem->SetBitmap(wxBitmapBundle::FromSVG(deleteSVG, CONTEXT_MENU_ICON_SIZE));
	contextMenu->Append(deleteItem);
	contextMenu->Bind(wxEVT_MENU, &LoadsPanel::OnDelete, this, deleteItem->GetId());

	wxMenuItem* closeItem = new wxMenuItem(contextMenu, wxID_CLOSE, "Close");
	closeItem->SetBitmap(wxBitmapBundle::FromSVG(closeSVG, CONTEXT_MENU_ICON_SIZE));
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
	((LoadsPanelContent*)content)->MarkPagesValidity();
}

bool LoadsPanel::CheckForUnsaved()
{
	for (SideMenuCtrl* page : pages)
	{
		if (((Load*)page->GetSource())->AlteredFromOutside())
			if (!RecreateSource(page))
				return false;
	}
	return SideNotebook::CheckForUnsaved();
}

bool LoadsPanel::RecreateSource(SideMenuCtrl* page)
{
	switch (SaveDialog(this, "Couldn't find following file. Save it as, or it may be lost.", page->GetSource()->GetName()).ShowModal()) {
	case SaveDialog::ID_SAVE:
		if (!SaveAs(page)) return false;
		page->MarkSaved();
		return true;
	case SaveDialog::ID_SKIP:
		return true;
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
	if (curData->folder == "" || !ifstream(curData->GetPath()))
		f = SaveAs(page);
	if (f) {
		SideNotebook::Save(page);
	}

	return f;
}

bool LoadsPanel::SaveAs(SideMenuCtrl* page)
{
	DVNFileData* curData = page->GetSource();
	wxFileDialog dialog(this, "Select a folder to save \"" + curData->GetName() + "\"", "", curData->GetNameWithExt(), "Load files (*.jld)|*.jld", wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
	if (dialog.ShowModal() == wxID_OK) {
		string name = wxFileName(dialog.GetPath()).GetName().ToStdString();
		if (name == Load::placeHolder->GetExtension())
			name = "";
		string folder = dialog.GetDirectory().ToStdString();
		string stat = Load::ValidateName(name);
		if (!stat.empty()) {
			ShowError(base, stat);
			return false;
		}
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
		SideNotebook::Save(page);
	}
	else return false;
	
	return true;
}

void LoadsPanel::OnDuplicate(wxCommandEvent& e)
{
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	if(Duplicate(target))
		Unsave(true);
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
		MarkCurValidity();
	}
	else
		target->Refresh();
}

void LoadsPanel::OnClose(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	if (!target->GetSource()->upToDate) {
		switch (SaveDialog(base, "Following file is unsaved, how would you like to proceed?", target->GetSource()->GetName()).ShowModal()) {
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
	else {
		Close(target);
		MarkCurValidity();
	}
}

void LoadsPanel::OnRename(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	Rename(target);
	target->Refresh();
}

void LoadsPanel::OnStatusUpdate(wxCommandEvent& e)
{
	SideNotebook::OnStatusUpdate(e);

	if (!e.GetSkipped())
		return;

	for (const Scenario& scen : ((Load*)cur->GetSource())->GetScenarios())
	{
		if (scen.invalidBands > 0)
		{
			cur->SetForegroundColour(DARK_RED);
			return;
		}
	}
	cur->SetForegroundColour(*wxBLACK);
}

void LoadsPanel::MarkCurValidity()
{
	if(pages.size())
		((LoadsPanelContent*)content)->MarkCurValidity();
	else
		statusBar.SetStatus("");
}