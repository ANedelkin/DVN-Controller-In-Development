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

	Bind(wxEVT_COMMAND_MENU_SELECTED, &SideNotebook::OnDelete, this);

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
		Remove(dynamic_cast<wxWindowBase*>(e.GetEventObject()));
	}
}

void SideNotebook::Remove(wxWindowBase* win)
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

void SideNotebook::RemoveAll()
{
	for (SideMenuCtrl* page : pages)
	{
		Remove(page);
	}
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
	content->Save();
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