#include "SideNotebookContent.h"

SideNotebookContent::SideNotebookContent(wxWindow* parent, DVNFileData* source) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
	this->source = source;
	this->isInited = false;

}

DVNFileData* SideNotebookContent::GetSource() { return source; }

bool SideNotebookContent::IsInited()  { return isInited; }

void SideNotebookContent::Init()
{
	isInited = true;
}

void SideNotebookContent::UnInit()
{
	isInited = false;
}

void SideNotebookContent::MarkUnsaved()
{
	/*wxCommandEvent e(EVT_UNSAVE);
	GetParent()->GetEventHandler()->ProcessEvent(e);*/
}


