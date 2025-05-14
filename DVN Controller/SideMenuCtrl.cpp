#include "SideMenuCtrl.h"

void SideMenuCtrl::OnContextMenu(wxContextMenuEvent& e) {
	PopUpContextMenu(wxWindow::ScreenToClient(e.GetPosition()));
}

void SideMenuCtrl::OnKey(wxKeyEvent& e)
{
	if (e.GetUnicodeKey() == 'M') {
		wxWindow* target = (wxWindow*)e.GetEventObject();
		PopUpContextMenu(wxPoint(0, target->GetSize().GetHeight()));
	}
	else e.Skip();
}

void SideMenuCtrl::PopUpContextMenu(const wxPoint& position)
{
	if (contextMenu) {
		PopupMenu(contextMenu, position);
	}
}

SideMenuCtrl::SideMenuCtrl(wxWindow* parent, wxPanel* mainPanel, DVNFileData* source) : wxButton(parent, wxID_ANY) {
	SetMinSize(FromDIP(wxSize(NAME_INPUT_LEN, 40)));

	this->source = source;
	this->mainPanel = mainPanel;
	SetBackgroundColour(wxColour(255, 255, 255));

	Bind(wxEVT_CONTEXT_MENU, &SideMenuCtrl::OnContextMenu, this);
	Bind(wxEVT_KEY_UP, &SideMenuCtrl::OnKey, this);
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

void SideMenuCtrl::SetLabel(const wxString& label)
{
	wxString temp = label;
	wxSize buttonSize = GetClientSize();
	wxClientDC dc(this);
	wxFont font = GetFont();
	font.SetWeight(wxFONTWEIGHT_BOLD);
	dc.SetFont(font);

	int textWidth, textHeight;
	dc.GetTextExtent(temp, &textWidth, &textHeight);

	int maxWidth = GetSize().GetWidth() - FromDIP(50);
	if (textWidth > maxWidth) {
		do {
			temp = temp.Left(temp.Length() - 3);
			dc.GetTextExtent(temp, &textWidth, &textHeight);
		} 
		while (textWidth > maxWidth);
		temp += "...";
	}
	wxButton::SetLabel(temp);
}
