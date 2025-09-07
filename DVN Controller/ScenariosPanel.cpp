#include "ScenariosPanel.h"
#include "SideNotebookContent.h"

ScenariosPanel::ScenariosPanel(wxWindow* parent, const char style) 
			  : SideNotebook(parent, "Scenarios", style & CONTENT ? Scenario::ValidateName : Scenario::ValidateNameUnique)
{
	this->style = style;

	if (!(style & READ_ONLY)) {
		contextMenu = new wxMenu();
		wxMenuItem* rename = new wxMenuItem(contextMenu, wxID_ANY, "Rename");
		rename->SetBitmap(wxBitmapBundle::FromSVG(penSVG, CONTEXT_MENU_ICON_SIZE));
		contextMenu->Append(rename);
		contextMenu->Bind(wxEVT_MENU, &ScenariosPanel::OnRename, this, rename->GetId());

		if (style & DELETABLE) {
			wxMenuItem* duplicate = new wxMenuItem(contextMenu, wxID_ANY, "Duplicate");
			duplicate->SetBitmap(wxBitmapBundle::FromSVG(copySVG, CONTEXT_MENU_ICON_SIZE));
			contextMenu->Append(duplicate);
			contextMenu->Bind(wxEVT_MENU, &ScenariosPanel::OnDuplicate, this, duplicate->GetId());

			wxMenuItem* deleteItem = new wxMenuItem(contextMenu, wxID_DELETE, "Delete");
			deleteItem->SetBitmap(wxBitmapBundle::FromSVG(deleteSVG, CONTEXT_MENU_ICON_SIZE));
			contextMenu->Append(deleteItem);
			contextMenu->Bind(wxEVT_MENU, &ScenariosPanel::OnDelete, this, deleteItem->GetId());
		}
		if (style & LOADABLE) {
			wxMenuItem* save = new wxMenuItem(contextMenu, wxID_ANY, "Save as template");
			save->SetBitmap(wxBitmapBundle::FromSVG(saveCMSVG, CONTEXT_MENU_ICON_SIZE));
			contextMenu->Append(save);
			contextMenu->Bind(wxEVT_MENU, &ScenariosPanel::OnSave, this, save->GetId());

			wxMenuItem* load = new wxMenuItem(contextMenu, wxID_ANY, "Load from template");
			load->SetBitmap(wxBitmapBundle::FromSVG(openCMSVG, CONTEXT_MENU_ICON_SIZE));
			contextMenu->Append(load);
			contextMenu->Bind(wxEVT_MENU, &ScenariosPanel::OnLoad, this, load->GetId());
		}
	}

	SetContent(new BandsPanel(this, Scenario::placeHolder, style & READ_ONLY));
	if (style & CONTENT) {
		SideNotebookContent* parent = dynamic_cast<SideNotebookContent*>(GetParent());
		assert(parent != nullptr && "ScenariosPanel parent is not a SideNotebookContent or derived");
		Load* source = dynamic_cast<Load*>(parent->GetSource());
		assert(source != nullptr && "Source is not a Load or derived");
		for (Scenario& child : source->GetScenarios())
			NewPage(&child);
	}
	content->UnInit();
}

StatusCode ScenariosPanel::NewPage(DVNFileData* data)
{
	Scenario* scen = dynamic_cast<Scenario*>(data);
	assert(scen != nullptr && "data is not a Scenario or derived!");
	return SideNotebook::NewPage(scen);
}

void ScenariosPanel::MarkPagesValidity()
{
	for (SideMenuCtrl* page : pages) {
		MarkPageValidity(page);
	}
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

void ScenariosPanel::OnStatusUpdate(wxCommandEvent& e)
{
	MarkPageValidity(cur);

	SideNotebook::OnStatusUpdate(e);
}

void ScenariosPanel::ChangeSelection(SideMenuCtrl* scenCtrl)
{
	SideNotebook::ChangeSelection(scenCtrl);

	int invalidBands = ((Scenario*)cur->GetSource())->invalidBands;
	if(invalidBands)
		statusBar.SetStatus(ToString(InvalidBands, invalidBands));
	else
		statusBar.SetStatus("");
}

void ScenariosPanel::MarkPageValidity(SideMenuCtrl* page)
{
	int invalidBands = ((Scenario*)page->GetSource())->invalidBands;
	if (invalidBands)
		page->SetForegroundColour(DARK_RED);
	else
		page->SetForegroundColour(*wxBLACK);
}

void ScenariosPanel::OnLoad(wxCommandEvent& e)
{
	ScenSelectDialog dialog(base);
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	if (dialog.ShowModal() == wxID_OK) {
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
	}
	target->Refresh();
}

void ScenariosPanel::OnSave(wxCommandEvent& e) {
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	Scenario scenario = *(Scenario*)target->GetSource();
	NameSetter nameSetter(base, "Enter template name", Scenario::ValidateNameUnique, scenario.DVNFileData::GetName());
	nameSetter.ShowModal();
	if (nameSetter.ok) {
		scenario.DVNFileData::Rename(nameSetter.name);
		scenario.Save();
	}
	target->Refresh();
}

void ScenariosPanel::OnDuplicate(wxCommandEvent& e)
{
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	Duplicate(target);
	target->Refresh();
}

void ScenariosPanel::OnDelete(wxCommandEvent& e)
{
	wxMessageDialog dialog(base, "If you delete a scenario you won't be able to get it back!", "Are you sure about that?", wxYES_NO | wxICON_EXCLAMATION);
	SideMenuCtrl* target = (SideMenuCtrl*)contextMenu->GetInvokingWindow();
	assert(target != nullptr && "Delete target is not SideMenuCtrl or derived");
	if (dialog.ShowModal() == wxID_YES) {
		if (exists(target->GetSource()->GetPath())) {
			remove(target->GetSource()->GetPath());
		}
		Close(target);
	}
	else
		target->Refresh();
}