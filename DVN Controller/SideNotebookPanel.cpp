#include "SideNotebookPanel.h"

SideNotebookPanel::SideNotebookPanel(wxWindow* parent, DVNFileData* source) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	this->source = source;
	this->isInited = false;

}

bool SideNotebookPanel::IsInited()  { return isInited; }

void SideNotebookPanel::Init()
{
	isInited = true;
}

void SideNotebookPanel::UnInit()
{
	isInited = false;
}

void SideNotebookPanel::MarkUnsaved()
{
	wxCommandEvent e(EVT_UNSAVE);
	GetParent()->GetEventHandler()->ProcessEvent(e);
}


