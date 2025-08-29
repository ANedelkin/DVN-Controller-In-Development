#pragma once

#include "SideNotebookContent.h"
#include "ScenariosPanel.h"

class LoadsPanelContent : public SideNotebookContent
{
private:
	ScenariosPanel* scenPanel;
public:
	LoadsPanelContent(wxWindow* parent);
	void Select(int i);
	void SetSource(DVNFileData* source) override;
	void Init() override;
	void UnInit() override;
	void MarkPagesValidity();
};

