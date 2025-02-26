#pragma once
#include "Global.h"
#include "DVNFileData.h"

class SideNotebookPanel : public wxPanel
{
protected:
	DVNFileData* source;
	bool isInited;

	wxPanel* mainPanel;
public:
	SideNotebookPanel(wxWindow* parent, wxPanel* mainPanel, DVNFileData* source);
	virtual void ChangeSource(DVNFileData* source) = 0;
	bool IsInited();
	virtual void Init();
	virtual void UnInit();
	void MarkUnsaved();
};

