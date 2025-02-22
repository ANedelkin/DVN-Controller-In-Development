#include "ScenariosFrame.h"


ScenariosFrame::ScenariosFrame(wxWindow* parent, bool isForSelecting) : 
	wxDialog(parent, wxID_ANY, "Scenario templates", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)
{

	this->isForSelecting = isForSelecting;

	wxIcon icon(ICON);
	SetIcon(icon);

	SetMinSize(FromDIP(wxSize(1000, 710)));
	SetSize(FromDIP(wxSize(1000, 710)));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	CreateToolBar();

	ScenariosPanel* scenPanel = new ScenariosPanel(this);

	mainSizer->Add(toolBar, 0, wxEXPAND);
	mainSizer->Add(scenPanel, 1, wxEXPAND);
	SetSizer(mainSizer);
}

void ScenariosFrame::CreateToolBar()
{
	toolBar = new wxPanel(this, wxID_ANY);
	wxBoxSizer* toolBarSizer = new wxBoxSizer(wxHORIZONTAL);

#define CTRL_HEIGHT 30

	newBtn = new wxButton(toolBar, wxID_ANY, "New");
	addBtn = new wxButton(toolBar, wxID_ANY, "Add existing");
	saveBtn = new wxButton(toolBar, wxID_ANY, "Save");

#define PADDING FromDIP(5)

	toolBarSizer->Add(newBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(addBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(saveBtn, 0, wxEXPAND | wxALL, PADDING);

	toolBar->SetSizerAndFit(toolBarSizer);

	newBtn->Bind(wxEVT_LEFT_UP, &ScenariosFrame::OnNew, this);
	saveBtn->Bind(wxEVT_LEFT_UP, &ScenariosFrame::OnSave, this);
	addBtn->Bind(wxEVT_LEFT_UP, &ScenariosFrame::OnAdd, this);
}

void ScenariosFrame::OnNew(wxMouseEvent& e)
{
}

void ScenariosFrame::OnSave(wxMouseEvent& e)
{
}

void ScenariosFrame::OnAdd(wxMouseEvent& e)
{
}
