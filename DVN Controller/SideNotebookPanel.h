#pragma once
#include "Global.h"
#include "DVNFileData.h"

class SideNotebookPanel : public wxPanel
{
protected:
	DVNFileData* source;
	bool isInited;
public:
	SideNotebookPanel(wxWindow* parent, DVNFileData* source);
	virtual void SetSource(DVNFileData* source) = 0;
	DVNFileData* GetSource();
	bool IsInited();
	virtual void Init();
	virtual void UnInit();
	void MarkUnsaved();
};

