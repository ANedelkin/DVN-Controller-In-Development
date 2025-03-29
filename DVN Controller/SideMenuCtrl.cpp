#include "SideMenuCtrl.h"

void SideMenuCtrl::OnContextMenu(wxMouseEvent& e) {
	if(contextMenu)
		PopupMenu(contextMenu, e.GetPosition());
}

SideMenuCtrl::SideMenuCtrl(wxWindow* parent, wxPanel* mainPanel, DVNFileData* source) : wxButton(parent, wxID_ANY, source->GetName()) {
	SetMinSize(FromDIP(wxSize(-1, 40)));

	this->source = source;
	this->mainPanel = mainPanel;
	SetBackgroundColour(wxColour(255, 255, 255));

	Bind(wxEVT_RIGHT_UP, &SideMenuCtrl::OnContextMenu, this);
}

DVNFileData* SideMenuCtrl::GetSource() { return source; }

void SideMenuCtrl::SetSource(DVNFileData* source) {
	this->source = source;
	SetLabel(source->GetName());
}

void SideMenuCtrl::SetContextMenu(wxMenu* menu)
{
	contextMenu = menu;
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