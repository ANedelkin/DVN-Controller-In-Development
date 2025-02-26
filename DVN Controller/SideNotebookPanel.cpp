#include "SideNotebookPanel.h"

SideNotebookPanel::SideNotebookPanel(wxWindow* parent, wxPanel* mainPanel, DVNFileData* source) : wxPanel(parent)
{
	this->source = source;
	this->isInited = false;

	this->mainPanel = mainPanel;
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
	e.SetClientData(source);
	mainPanel->GetEventHandler()->ProcessEvent(e);
}


