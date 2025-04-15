#pragma once
#include "Global.h"
#include "DVNFileData.h"

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

