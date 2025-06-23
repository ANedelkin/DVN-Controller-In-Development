#pragma once

#include "DVNFileData.h"

#define CONTEXT_MENU_ICON_SIZE wxSize(16, 16)

class SideNotebookContent : public wxPanel
{
protected:
	DVNFileData* source;
	bool isInited;
public:
	SideNotebookContent(wxWindow* parent, DVNFileData* source = nullptr);
	virtual void SetSource(DVNFileData* source) = 0;
	DVNFileData* GetSource();
	bool IsInited();
	virtual void Init();
	virtual void UnInit();
	void MarkUnsaved();
};

