#include "LoadsPanelContent.h"

LoadsPanelContent::LoadsPanelContent(wxWindow* parent) : SideNotebookContent(parent, Load::placeHolder)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	scenPanel = new ScenariosPanel(this, CONTENT | LOADABLE);
	sizer->Add(scenPanel, 1, wxEXPAND);
	SetSizerAndFit(sizer);
}

void LoadsPanelContent::Select(int i)
{
	scenPanel->Select(i);
}

void LoadsPanelContent::SetSource(DVNFileData* source)
{
	Load* load = dynamic_cast<Load*>(source);
	assert(load != nullptr && "source is not a Load or derived");
	this->source = load;
	vector<SideMenuCtrl*>& pages = scenPanel->GetPages();
	for (char i = 0; i < pages.size(); i++)
	{
		pages[i]->SetSource(&load->GetScenarios()[i]);
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
