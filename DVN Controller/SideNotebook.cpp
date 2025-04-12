#include "SideNotebook.h"
#include "ScenSelectDialog.h"

SideNotebook::SideNotebook(wxWindow* parent, string sideMenuTxt, string(*pageNameValidator)(const string& name)) : wxPanel(parent)
{
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
	return AddPage(page);
}

StatusCode SideNotebook::AddPage(SideMenuCtrl* page)
{
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
	if (cur) cur->SetBackgroundColour(wxColour(255, 255, 255));
	page->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	cur = page;

	DVNFileData* s = cur->GetSource();
	content->SetSource(s);
}

void SideNotebook::OnSelect(wxCommandEvent& e)
{
	SideMenuCtrl* page = dynamic_cast<SideMenuCtrl*>(e.GetEventObject());
	if (page != cur) ChangeSelection(page);
	e.Skip();
}

void SideNotebook::OnUnsave(wxCommandEvent& e)
{
	Unsave(false, (SideMenuCtrl*)e.GetEventObject());
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

void SideNotebook::Rename(SideMenuCtrl* page, const string& name, bool renameFile)
{
	if (page->GetSource()->GetName() != name) {
		DVNFileData* source = page->GetSource();
		string oldPath = source->GetPath();
		source->Rename(name);
		if(renameFile)
			rename(oldPath, source->GetPath());
		page->SetLabel(name);
	}
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

bool SideNotebook::Save(SideMenuCtrl* page, bool saveAs) //move  to load class
{
	DVNFileData* curData = page->GetSource();
	if (curData->folder == "" || saveAs) {
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

void SideNotebook::UpdateContent()
{
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
			switch (SaveDialog(this, pages[i]->GetSource()->GetName(), SAVING_MANY).ShowModal()) {
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