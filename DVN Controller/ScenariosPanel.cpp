#include "ScenariosPanel.h"
#include "SideNotebookContent.h"

ScenariosPanel::ScenariosPanel(wxWindow* parent, const char style) 
			  : SideNotebook(parent, "Scenarios", Scenario::ValidateNameUnique)
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

	SetContent(new BandsPanel(this, Scenario::placeHolder, style));
	if (style & CONTENT) {
		SideNotebookContent* parent = dynamic_cast<SideNotebookContent*>(GetParent());
		assert(parent != nullptr && "ScenariosPanel parent is not a SideNotebookContent or derived");
		Load* source = dynamic_cast<Load*>(parent->GetSource());
		assert(source != nullptr && "Source is not a Load or derived");
		for (Scenario* child : source->GetScenarios())
			NewPage(child);
	}
	content->UnInit();
}

StatusCode ScenariosPanel::NewPage(Scenario* data)
{
	return SideNotebook::NewPage(data);
}

void ScenariosPanel::OnRename(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	bool isContent = style & CONTENT;
	if (Rename(target, !isContent) && isContent) {
		SideNotebookContent* parent = dynamic_cast<SideNotebookContent*>(GetParent());
		assert(parent != nullptr && "ScenariosPanel parent is not a SideNotebookContent or derived");
		parent->MarkUnsaved();
	}
	target->Refresh();
}

void ScenariosPanel::OnLoad(wxCommandEvent& e)
{
	ScenSelectDialog dialog(base);
	if (dialog.ShowModal() == wxID_OK) {
		SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
		Scenario* selection = dialog.GetSelection();
		*(Scenario*)target->GetSource() = *selection;
		target->SetLabel(target->GetSource()->GetName());
		target->MarkUnsaved();
		if (cur == target) ChangeSelection(cur);
		if (style & CONTENT) {
			SideNotebookContent* parent = dynamic_cast<SideNotebookContent*>(GetParent());
			assert(parent != nullptr && "ScenariosPanel parent is not a SideNotebookContent or derived");
			parent->MarkUnsaved();
		}
		else
			Unsave(false, target);
		target->Refresh();
	}
}

void ScenariosPanel::OnSave(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	Scenario scenario = *(Scenario*)target->GetSource();
	NameSetter nameSetter(base, "Enter template name", Scenario::ValidateNameUnique, scenario.DVNFileData::GetName());
	nameSetter.ShowModal();
	if (nameSetter.ok) {
		scenario.DVNFileData::Rename(nameSetter.name);
		Save(target);
	}
	target->Refresh();
}

void ScenariosPanel::OnDelete(wxCommandEvent& e)
{
	wxMessageDialog dialog(base, "If you delete a scenario you won't be able to get it back!", "Are you sure about that?", wxYES_NO | wxICON_EXCLAMATION);
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