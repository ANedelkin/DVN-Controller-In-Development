#include "SideNotebook.h"
#include "ScenSelectDialog.h"

SideNotebook::SideNotebook(wxWindow* parent, string sideMenuTxt, DVNFileData* source) : SideNotebookPanel(parent, source)
{
	contextMenu = new wxMenu();

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

	this->source = source;
}

void SideNotebook::SetContent(SideNotebookPanel* content) {
	//if (this->content) {
	//	this->content->DestroyChildren();
	//	this->content->Destroy();
	//}
	this->content = content;
	mainSizer->Add(this->content, 1, wxEXPAND | wxLEFT, FromDIP(5));
	if (source) {
		for (DVNFileData* child : source->children) {
			NewPage(child);
		}
	}
}

Status SideNotebook::NewPage(DVNFileData* data)
{
	SideMenuCtrl* page = new SideMenuCtrl(pagesList, this, data);
	return AddPage(page);
}

Status SideNotebook::AddPage(SideMenuCtrl* page)
{
	page->SetContextMenu(contextMenu);

	pages.push_back(page);
	pagesSizer->Add(page, 0, wxEXPAND | wxBOTTOM, FromDIP(10));
	ChangeSelection(page);
	if (!content->IsInited()) content->Init();
	Refresh();
	Layout();
	page->Bind(wxEVT_LEFT_UP, &SideNotebook::OnSelect, this);
	return Success;
}

vector<SideMenuCtrl*> SideNotebook::GetPages()
{
	return pages;
}

void SideNotebook::ChangeSelection(SideMenuCtrl* page)
{
	if (cur) cur->SetBackgroundColour(wxColour(255, 255, 255));
	page->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	cur = page;

	DVNFileData* s = cur->GetSource();
	content->SetSource(s);
}

void SideMenuCtrl::OnRename(wxCommandEvent& e) {
	NameSetter* nameSetter = new NameSetter(base, "Enter name", DVNFileData::ValidateName, source->GetName());
	nameSetter->ShowModal();
	if (nameSetter->ok && source->GetName() != nameSetter->name) {
		source->Rename(nameSetter->name);
		SetLabel(nameSetter->name);
		MarkUnsaved();
	}
}

void SideNotebook::OnSelect(wxMouseEvent& e)
{
	SideMenuCtrl* page = dynamic_cast<SideMenuCtrl*>(e.GetEventObject());
	if (page != cur) ChangeSelection(page);
	e.Skip();
}

void SideNotebook::OnUnsave(wxCommandEvent& e)
{
	Unsave(false);
}

void SideNotebook::Unsave(bool created)
{
	DVNFileData* source = cur->GetSource();
	string ss = source->SaveString();
	string nm = source->GetName();
	if (!created && (source->oldSaveString == ss && source->oldName == nm))
		cur->MarkSaved();
	else
		cur->Unsave();
}

void SideNotebook::Close(SideMenuCtrl* win)
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

bool SideNotebook::Save(SideMenuCtrl* page, bool saveAs)
{
	DVNFileData* curData = page->GetSource();
	if (curData->folder == "" || saveAs) {
		wxFileDialog dialog(this, "Select a folder to save \"" + curData->GetName() + "\"" + curData->GetName(), "", curData->GetNameWithExt(), "Load files (*.dvnl)|*.dvnl", wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
		if (dialog.ShowModal() == wxID_OK) {
			string name = wxFileName(dialog.GetPath()).GetName().ToStdString();
			string folder = dialog.GetDirectory().ToStdString();
			for (SideMenuCtrl* existing : pages) {
				if (existing == page) continue;
				if (existing->GetSource()->GetOldPath() == folder + "\\" + name + existing->GetSource()->GetExtension()) {
					ErrorMessage(base, FileAlreadyOpen, name.c_str());
					return false;
				}
			}
			curData->Rename(name);
			curData->folder = folder;
		}
		else return false;
	}
	page->Save();
	return true;
}

void SideNotebook::Select(char i)
{
	if(pages.size() > i) ChangeSelection(pages[i]);
}

void SideNotebook::SetSource(DVNFileData* source) {
	this->source = source;
	for (char i = 0; i < pages.size(); i++)
	{
		pages[i]->SetSource(source->children[i]);
	}
	content->SetSource(cur->GetSource());
}

void SideNotebook::SaveCurrent(bool saveAs)
{
	if (cur) Save(cur, saveAs);
}

SideMenuCtrl* SideNotebook::GetCurrent() {
	return cur;
}

bool SideNotebook::CheckForUnsaved()
{
	for (char i = 0; i < pages.size(); i++)
	{
		if (!pages[i]->GetSource()->upToDate) {
			switch (SaveDialog(this, pages[i]->GetSource()->GetName()).ShowModal()) {
			case SaveDialog::ID_SAVE:
				if (!Save(pages[i], false)) return false;
				break;
			case SaveDialog::ID_CANCEL:
			case wxID_CANCEL:
				return false;
			case SaveDialog::ID_SAVE_ALL:
				for (char j = i; j < pages.size(); j++) {
					if (!Save(pages[j], false)) return false;
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