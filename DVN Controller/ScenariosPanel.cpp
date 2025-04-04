#include "ScenariosPanel.h"

ScenariosPanel::ScenariosPanel(wxWindow* parent, Load* source, const char style) : SideNotebook(parent, "Scenarios", source)
{
	this->style = style;

	if (!(style & READ_ONLY)) {
		contextMenu = new wxMenu();
		wxMenuItem* rename = new wxMenuItem(contextMenu, wxID_ANY, "Rename");
		rename->SetBitmap(wxArtProvider::GetBitmap(wxART_EDIT));
		contextMenu->Append(rename);
		contextMenu->Bind(wxEVT_MENU, &ScenariosPanel::OnRename, this, rename->GetId());

		if (style & DELETABLE) {
			wxMenuItem* deleteItem = new wxMenuItem(contextMenu, wxID_DELETE, "Delete");
			deleteItem->SetBitmap(wxArtProvider::GetBitmap(wxART_DELETE));
			contextMenu->Append(deleteItem);
			contextMenu->Bind(wxEVT_MENU, &ScenariosPanel::OnDelete, this, deleteItem->GetId());
		}
		if (style & LOADABLE) {
			wxMenuItem* save = new wxMenuItem(contextMenu, wxID_ANY, "Save as template");
			save->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE));
			contextMenu->Append(save);
			contextMenu->Bind(wxEVT_MENU, &ScenariosPanel::OnSave, this, save->GetId());

			wxMenuItem* load = new wxMenuItem(contextMenu, wxID_ANY, "Load from template");
			load->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN));
			contextMenu->Append(load);
			contextMenu->Bind(wxEVT_MENU, &ScenariosPanel::OnLoad, this, load->GetId());
		}
	}

	SetContent(new BandsPanel(this, new Scenario(), style));
	content->UnInit();
}

Status ScenariosPanel::AddPage(Scenario* data)
{
	for (SideMenuCtrl* page : pages) {
		if (page->GetSource()->GetNewPath() == data->GetNewPath()) {
			ErrorMessage(base, ScenarioAlreadyExists, 0, data->DVNFileData::GetName().c_str());
			return ScenarioAlreadyExists;
		}
	}

	return SideNotebook::NewPage(data);
}

void ScenariosPanel::OnRename(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	NameSetter* nameSetter = new NameSetter(base, "Enter name", Scenario::ValidateName, target->GetSource()->GetName());
	nameSetter->ShowModal();
	if (nameSetter->ok && target->GetSource()->GetName() != nameSetter->name) {
		target->GetSource()->Rename(nameSetter->name);
		target->SetLabel(nameSetter->name);
		if (source)
			MarkUnsaved();
		else
			Unsave(false, target);
	}
	target->Refresh();
}

void ScenariosPanel::OnLoad(wxCommandEvent& e)
{
	ScenSelectDialog* dialog = new ScenSelectDialog(base);
	if (dialog->ShowModal() == wxID_OK) {
		SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
		Scenario* selection = dialog->GetSelection();
		*(Scenario*)target->GetSource() = *selection;
		target->SetLabel(target->GetSource()->GetName());
		target->MarkUnsaved();
		if (cur == target) ChangeSelection(cur);
		if (source)
			MarkUnsaved();
		else
			Unsave(false, target);
		target->Refresh();
	}
}

void ScenariosPanel::OnSave(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	Scenario scenario = *(Scenario*)target->GetSource();
	NameSetter* nameSetter = new NameSetter(base, "Enter template name", Scenario::ValidateNameUnique, scenario.DVNFileData::GetName());
	nameSetter->ShowModal();
	if (nameSetter->ok) {
		scenario.DVNFileData::Rename(nameSetter->name);
		Save(target, false);
	}
	target->Refresh();
}

void ScenariosPanel::OnDelete(wxCommandEvent& e)
{
	wxMessageDialog dialog(base, "If you delete a scenario you won't be able to get it back!", "Are you sure about that?", wxYES_NO | wxICON_EXCLAMATION);
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