#include "SideNotebook.h"

SideNotebook::SideNotebook(wxWindow* parent, string sideMenuTxt, DVNFileData* source) : SideNotebookPanel(parent, source)
{
	mainSizer = new wxBoxSizer(wxHORIZONTAL);

	pagesBox = new wxStaticBox(this, wxID_ANY, sideMenuTxt);
	pagesBoxSizer = new wxStaticBoxSizer(wxVERTICAL, pagesBox, sideMenuTxt);
	pagesBoxSizer->AddSpacer(FromDIP(38));
	pagesBox->SetSizerAndFit(pagesBoxSizer);
	pagesBox->SetMinSize(FromDIP(wxSize(210, -1)));

	pagesSizer = new wxBoxSizer(wxVERTICAL);
	pagesList = new wxPanel(pagesBox);
	pagesList->SetSizerAndFit(pagesSizer);

	pagesBoxSizer->Add(pagesList, 1, wxEXPAND);

	mainSizer->Add(pagesBox, 0, wxEXPAND);

	this->SetSizerAndFit(mainSizer);

	Bind(EVT_DELETE, &SideNotebook::OnDelete, this);

	this->source = source;
}

void SideNotebook::SetContent(SideNotebookPanel* content) {
	if (this->content) {
		this->content->DestroyChildren();
		this->content->Destroy();
	}
	this->content = content;
	mainSizer->Add(this->content, 1, wxEXPAND | wxLEFT, FromDIP(5));
	if (source) {
		for (DVNFileData* child : source->children) {
			AddPage(child, true);
		}
	}
}

void SideNotebook::AddPage(DVNFileData* data, bool subMenu)
{
	SideMenuCtrl* page = new SideMenuCtrl(pagesList, this, data, subMenu);
	pages.push_back(page);
	pagesSizer->Add(page, 0, wxEXPAND | wxBOTTOM, FromDIP(10));
	ChangeSelection(page);
	Refresh();
	Layout();
	page->Bind(wxEVT_LEFT_UP, &SideNotebook::OnSelect, this);
}

void SideNotebook::ChangePage(DVNFileData* data)
{
	this->source = data;
	for (char i = 0; i < pages.size(); i++)
	{
		pages[i]->ChangeSource(data->children[i]);
		ChangeSelection(pages[i]);
	}
}

void SideNotebook::ChangeSelection(SideMenuCtrl* page)
{
	page->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	if (cur) cur->SetBackgroundColour(wxColour(255, 255, 255));
	cur = page;

	DVNFileData* s = cur->GetSource();
	content->ChangeSource(s);
	if (!content->IsInited()) content->Init();
}

void SideNotebook::OnSelect(wxMouseEvent& e)
{
	SideMenuCtrl* page = dynamic_cast<SideMenuCtrl*>(e.GetEventObject());
	if (page != cur) ChangeSelection(page);
	e.Skip();
}

void SideNotebook::OnDelete(wxCommandEvent& e)
{
	wxMessageDialog dialog(base, "If you delete this you won't be able to get it back!", "Are you sure about that?", wxYES_NO | wxICON_EXCLAMATION);
	if (dialog.ShowModal() == wxID_YES) {
		SideMenuCtrl* target = dynamic_cast<SideMenuCtrl*>(e.GetEventObject());
		if (exists(target->GetSource()->GetPath())) {
			remove(target->GetSource()->GetPath());
		}
		Remove(target);
	}
}

void SideNotebook::OnUnsave(wxCommandEvent& e)
{
	Unsave();
}

void SideNotebook::Unsave()
{
	cur->Unsave();
}

void SideNotebook::Remove(SideMenuCtrl* win)
{
	int i = find(pages.begin(), pages.end(), win) - pages.begin();
	pages.erase(pages.begin() + i);
	if (cur == win) {
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
	win->Destroy();
}

void SideNotebook::Save(SideMenuCtrl* page)
{
	DVNFileData* curData = page->GetSource();
	if (curData->folder == "") {
		wxDirDialog dialog(this, "Select a folder", "", wxDD_DEFAULT_STYLE);
		if (dialog.ShowModal() == wxID_OK) {
			curData->folder = dialog.GetPath();
		}
		else return;
	}
	page->Save();
}

void SideNotebook::ChangeSource(DVNFileData* source) {
	this->source = source;
	for (char i = 0; i < pages.size(); i++)
	{
		pages[i]->ChangeSource(source->children[i]);
	}
	content->ChangeSource(cur->GetSource());
}

void SideNotebook::SaveCurrent()
{
	if (cur) Save(cur);
}

bool SideNotebook::CheckForUnsaved()
{
	for (char i = 0; i < pages.size(); i++)
	{
		if (!pages[i]->GetSource()->upToDate) {
			switch (SaveDialog(this, pages[i]->GetSource()->GetName()).ShowModal()) {
			case SaveDialog::ID_SAVE:
				Save(pages[i]);
				break;
			case SaveDialog::ID_CANCEL:
			case wxID_CANCEL:
				return false;
			case SaveDialog::ID_SAVE_ALL:
				for (char j = i; j < pages.size(); j++) {
					Save(pages[i]);
				}
				return true;
			case SaveDialog::ID_SKIP_ALL:
				return true;
			}
		}
	}
	return true;
}

void SideNotebook::Init() {
	SideNotebookPanel::Init();
	pagesList->Show();
	content->Init();
}

void SideNotebook::UnInit() {
	SideNotebookPanel::UnInit();
	pagesList->Hide();
	content->UnInit();
}