#include "SideNotebook.h"
#include "ScenSelectDialog.h"

SideNotebook::SideNotebook(wxWindow* parent, string sideMenuTxt, string(*pageNameValidator)(const string& name)) : wxPanel(parent)
{
	this->pageNameValidator = pageNameValidator;

	contextMenu = new wxMenu();

	mainSizer = new wxBoxSizer(wxHORIZONTAL);

	pagesBox = new wxStaticBox(this, wxID_ANY, sideMenuTxt);
	pagesBoxSizer = new wxStaticBoxSizer(wxVERTICAL, pagesBox, sideMenuTxt);
	pagesBoxSizer->AddSpacer(FromDIP(38));
	pagesBox->SetSizerAndFit(pagesBoxSizer);
	pagesBox->SetMinSize(FromDIP(wxSize(NAME_INPUT_LEN + 30, -1)));

	scrollWrapper = new wxScrolledWindow(pagesBox);
	wxBoxSizer* scrollSizer = new wxBoxSizer(wxVERTICAL);
	scrollWrapper->SetSizerAndFit(scrollSizer);
	scrollWrapper->SetScrollRate(0, FromDIP(5));
	
	pagesSizer = new wxBoxSizer(wxVERTICAL);
	pagesList = new wxPanel(scrollWrapper);
	pagesList->SetSizer(pagesSizer);

	pagesBoxSizer->Add(scrollWrapper, 1, wxEXPAND);
	scrollSizer->Add(pagesList, 0, wxEXPAND | wxRIGHT, FromDIP(5));
	mainSizer->Add(pagesBox, 0, wxEXPAND);

	this->SetSizerAndFit(mainSizer);
}

void SideNotebook::SetContent(SideNotebookContent* content) {
	this->content = content;
	mainSizer->Add(this->content, 1, wxEXPAND | wxLEFT, FromDIP(5));
}

StatusCode SideNotebook::NewPage(DVNFileData* data)
{
	SideMenuCtrl* page = new SideMenuCtrl(pagesList, this, data);
	page->SetContextMenu(contextMenu);

	pages.push_back(page);
	pagesSizer->Add(page, 0, wxEXPAND | wxBOTTOM, FromDIP(10));
	ChangeSelection(page);

	if (!content->IsInited()) content->Init();
	Layout();
	Refresh();

	page->SetLabel(page->GetSource()->GetName());
	page->Bind(wxEVT_BUTTON, &SideNotebook::OnSelect, this);
	
	return Success;
}

vector<SideMenuCtrl*>& SideNotebook::GetPages()
{
	return pages;
}

void SideNotebook::ChangeSelection(SideMenuCtrl* page)
{
	if (cur) 
		cur->SetBackgroundColour(wxColour(255, 255, 255));
	page->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	cur = page;

	DVNFileData* s = cur->GetSource();
	content->SetSource(s);
}

void SideNotebook::OnSelect(wxCommandEvent& e)
{
	SideMenuCtrl* page = dynamic_cast<SideMenuCtrl*>(e.GetEventObject());
	if (page != cur) 
		ChangeSelection(page);
	e.Skip();
}

void SideNotebook::OnUnsave(wxCommandEvent& e)
{
	SideMenuCtrl* target = dynamic_cast<SideMenuCtrl*>(e.GetEventObject());
	assert(target != nullptr && "Unsave target is not SideMenuCtrl or derived.");
	Unsave(false, target);
}

void SideNotebook::Unsave(bool created, SideMenuCtrl* target)
{
	target = target ? target : cur;
	DVNFileData* source = target->GetSource();
	string ss = source->SaveString();
	string nm = source->GetName();
	if (!created && (source->oldSaveString == ss))
		target->MarkSaved();
	else
		target->Unsave();
}

bool SideNotebook::Rename(SideMenuCtrl* page, bool renameFile)
{
	NameSetter nameSetter = NameSetter(base, "Enter name", pageNameValidator, page->GetSource()->GetName());
	nameSetter.ShowModal();
	if (nameSetter.ok && page->GetSource()->GetName() != nameSetter.name) {
		DVNFileData* source = page->GetSource();
		string oldPath = source->GetPath();
		source->Rename(nameSetter.name);
		if(renameFile)
			rename(oldPath, source->GetPath());
		page->SetLabel(nameSetter.name);
		return true;
	}
	return false;
}

void SideNotebook::Close(SideMenuCtrl* page)
{
	int i = find(pages.begin(), pages.end(), page) - pages.begin();
	pages.erase(pages.begin() + i);
	if (cur == page) {
		if (pages.size() > 0) {
			if (i < pages.size()) ChangeSelection(pages[i]);
			else ChangeSelection(pages[i - 1]);
		}
		else {
			content->UnInit();
			cur = nullptr;
		}
	}
	pagesSizer->Remove(i);
	Layout();
	page->Destroy();
}

bool SideNotebook::Save(SideMenuCtrl* page)
{
	page->GetSource()->Save();
	return true;
}

void SideNotebook::Select(char i)
{
	if(pages.size() > i) ChangeSelection(pages[i]);
}

void SideNotebook::UpdateContent()
{
	content->SetSource(cur->GetSource());
}

void SideNotebook::SaveCurrent()
{
	if (cur && Save(cur)) cur->MarkSaved();
}

SideMenuCtrl* SideNotebook::GetCurrent() {
	return cur;
}

bool SideNotebook::CheckForUnsaved()
{
	for (char i = 0; i < pages.size(); i++)
	{
		if (!pages[i]->GetSource()->upToDate) {
			switch (SaveDialog(this, pages[i]->GetSource()->GetName(), SAVING_MANY).ShowModal()) {
			case SaveDialog::ID_SAVE:
				if (!Save(pages[i])) return false;
				break;
			case SaveDialog::ID_CANCEL:
			case wxID_CANCEL:
				return false;
			case SaveDialog::ID_SAVE_ALL:
				for (char j = i; j < pages.size(); j++) {
					if (!Save(pages[j])) return false;
				}
				return true;
			case SaveDialog::ID_SKIP_ALL:
				return true;
			}
		}
	}
	return true;
}