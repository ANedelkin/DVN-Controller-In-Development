#include "LoadsPanelContent.h"

LoadsPanelContent::LoadsPanelContent(wxWindow* parent) : SideNotebookContent(parent, Load::placeHolder)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	scenPanel = new ScenariosPanel(this, CONTENT);
	sizer->Add(scenPanel, 1, wxEXPAND);
	SetSizerAndFit(sizer);
}

void LoadsPanelContent::SetSource(DVNFileData* source)
{
	this->source = source;
	vector<SideMenuCtrl*>& pages = scenPanel->GetPages();
	for (char i = 0; i < pages.size(); i++)
	{
		pages[i]->SetSource(source->children[i]);
	}
	scenPanel->UpdateContent();
}

void LoadsPanelContent::Init()
{
	SideNotebookContent::Init();
	scenPanel->pagesList->Show();
	scenPanel->content->Init();
}

void LoadsPanelContent::UnInit()
{
	SideNotebookContent::UnInit();
	scenPanel->pagesList->Hide();
	scenPanel->content->UnInit();
}
