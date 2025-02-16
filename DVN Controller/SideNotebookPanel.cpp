#include "SideNotebookPanel.h"

SideNotebookPanel::SideNotebookPanel(wxWindow* parent, DVNFileData* source) : wxPanel(parent)
{
	this->source = source;
	this->isInited = false;
}

void SideNotebookPanel::Save()
{
	source->Save();
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


