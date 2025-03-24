#include "SideMenuCtrl.h"

void SideMenuCtrl::OnContextMenu(wxMouseEvent& e) {
	PopupMenu(contextMenu, e.GetPosition());

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

void SideMenuCtrl::OnDelete(wxCommandEvent& eIn) {
	wxCommandEvent eOut(EVT_DELETE);
	eOut.SetEventObject(this);
	mainPanel->GetEventHandler()->ProcessEvent(eOut);
}

void SideMenuCtrl::OnClose(wxCommandEvent& eIn) {
	wxCommandEvent eOut(EVT_CLOSE_PAGE);
	eOut.SetEventObject(this);
	mainPanel->GetEventHandler()->ProcessEvent(eOut);
}

void SideMenuCtrl::OnSave(wxCommandEvent& e) {
	source->Save();
}

void SideMenuCtrl::OnLoad(wxCommandEvent& eIn)
{
	wxCommandEvent eOut(EVT_LOAD);
	eOut.SetEventObject(this);
	mainPanel->GetEventHandler()->ProcessEvent(eOut);
}

SideMenuCtrl::SideMenuCtrl(wxWindow* parent, wxPanel* mainPanel, DVNFileData* source, char style) : wxButton(parent, wxID_ANY, source->GetName()) {
	SetMinSize(FromDIP(wxSize(-1, 40)));

	this->source = source;
	this->mainPanel = mainPanel;
	SetBackgroundColour(wxColour(255, 255, 255));

	contextMenu = new wxMenu();

	wxMenuItem* rename = new wxMenuItem(contextMenu, wxID_ANY, "Rename");
	rename->SetBitmap(wxArtProvider::GetBitmap(wxART_EDIT));
	contextMenu->Append(rename);
	contextMenu->Bind(wxEVT_MENU, &SideMenuCtrl::OnRename, this, rename->GetId());

	if (style & LOADABLE) {
		wxMenuItem* save = new wxMenuItem(contextMenu, wxID_ANY, "Save as template");
		save->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE));
		contextMenu->Append(save);
		contextMenu->Bind(wxEVT_MENU, &SideMenuCtrl::OnSave, this, save->GetId());

		wxMenuItem* load = new wxMenuItem(contextMenu, wxID_ANY, "Load from template");
		load->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN));
		contextMenu->Append(load);
		contextMenu->Bind(wxEVT_MENU, &SideMenuCtrl::OnLoad, this, load->GetId());
	}
	if (style & DELETABLE) {
		wxMenuItem* deleteItem = new wxMenuItem(contextMenu, wxID_DELETE, "Delete");
		deleteItem->SetBitmap(wxArtProvider::GetBitmap(wxART_DELETE));
		contextMenu->Append(deleteItem);
		contextMenu->Bind(wxEVT_MENU, &SideMenuCtrl::OnDelete, this, deleteItem->GetId());
	}
	if (style & CLOSEABLE) {
		wxMenuItem* closeItem = new wxMenuItem(contextMenu, wxID_CLOSE, "Close");
		closeItem->SetBitmap(wxArtProvider::GetBitmap(wxART_CLOSE));
		contextMenu->Append(closeItem);
		contextMenu->Bind(wxEVT_MENU, &SideMenuCtrl::OnClose, this, closeItem->GetId());
	}


	Bind(wxEVT_RIGHT_UP, &SideMenuCtrl::OnContextMenu, this);
}

DVNFileData* SideMenuCtrl::GetSource() { return source; }

void SideMenuCtrl::ChangeSource(DVNFileData* source) {
	delete this->source;
	this->source = source;
	SetLabel(source->GetName());
}

void SideMenuCtrl::Refresh()
{
	SetLabel(source->GetName());
}

void SideMenuCtrl::Unsave() {
	source->upToDate = false;
	wxFont newFont = GetFont();
	newFont.SetWeight(wxFONTWEIGHT_BOLD);
	SetFont(newFont);
}

void SideMenuCtrl::Save() {
	MarkSaved();
	source->Save();
}

void SideMenuCtrl::MarkSaved()
{
	source->upToDate = true;
	wxFont newFont = GetFont();
	newFont.SetWeight(wxFONTWEIGHT_NORMAL);
	SetFont(newFont);
}

void SideMenuCtrl::MarkUnsaved()
{
	wxCommandEvent e(EVT_UNSAVE);
	GetParent()->GetEventHandler()->ProcessEvent(e);
}