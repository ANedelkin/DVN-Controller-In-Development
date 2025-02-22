#include "MainFrame.h"

MainFrame::MainFrame(const string& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxIcon icon(ICON);
	SetIcon(icon);

	SetMinSize(FromDIP(wxSize(1122, 710)));

	base = this;

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	CreateToolBar();

	notebook = new wxNotebook(this, wxID_ANY);

	scenariosPanel = new SideNotebook(notebook, "Scenarios");
	BandsPanel* scenBandsPanel = new BandsPanel(scenariosPanel, new Scenario());
	scenBandsPanel->UnInit();
	scenariosPanel->SetContent(scenBandsPanel);

	loadsPanel = new SideNotebook(notebook, "Loads");
	SideNotebook* loadScenPanel = new SideNotebook(loadsPanel, "Scenarios", new Load());
	BandsPanel* loadBandsPanel = new BandsPanel(loadScenPanel, new Scenario());
	loadScenPanel->SetContent(loadBandsPanel);
	loadScenPanel->UnInit();
	loadsPanel->SetContent(loadScenPanel);

	notebook->AddPage(scenariosPanel, "Scenarios");
	notebook->AddPage(loadsPanel, "Loads");	
	
	mainSizer->Add(toolBar, 0, wxEXPAND);
	mainSizer->Add(notebook, 0, wxEXPAND);

	SetSizer(mainSizer);
}

void MainFrame::CreateToolBar()
{
	toolBar = new wxPanel(this, wxID_ANY);
	wxBoxSizer* toolBarSizer = new wxBoxSizer(wxHORIZONTAL);

	#define CTRL_HEIGHT 30

	newBtn = new wxButton(toolBar, wxID_ANY, "New");
	openBtn = new wxButton(toolBar, wxID_ANY, "Open");
	saveBtn = new wxButton(toolBar, wxID_ANY, "Save");
	scenBtn = new wxButton(toolBar, wxID_ANY, "Scenario templates");

	separator = new wxStaticLine(toolBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVERTICAL);

	selJammLabel = new wxStaticText(toolBar, wxID_ANY, "Select jammer: ");
	devComboBox = new wxComboBox(toolBar, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(150, -1)), 0, nullptr, wxCB_READONLY);


	loadToBtn = new wxButton(toolBar, wxID_UP, "Load to jammer");
	loadFromBtn = new wxButton(toolBar, wxID_DOWN, "Load from jammer");

	aboutBtn = new wxButton(toolBar, wxID_ANY, "About");

	#define PADDING FromDIP(5)

	toolBarSizer->Add(newBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(openBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(saveBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(scenBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(separator, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(selJammLabel, 0, wxALIGN_CENTER | wxALL, PADDING);
	toolBarSizer->Add(devComboBox, 0, wxALIGN_CENTER | wxRIGHT | wxTOP | wxBOTTOM, PADDING + 1);
	toolBarSizer->Add(loadToBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(loadFromBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->AddStretchSpacer();
	toolBarSizer->Add(aboutBtn, 0, wxEXPAND | wxALL, PADDING);

	toolBar->SetSizerAndFit(toolBarSizer);

	newBtn->Bind(wxEVT_LEFT_UP, &MainFrame::OnNew, this);
	saveBtn->Bind(wxEVT_LEFT_UP, &MainFrame::OnSave, this);
	scenBtn->Bind(wxEVT_LEFT_UP, &MainFrame::OnScenarios, this);
}

void MainFrame::NewScenario()
{
	NameSetter* nameSetter = new NameSetter(this, "Enter scenario name", Scenario::ValidateName);
	nameSetter->ShowModal();
	if (nameSetter->ok) scenariosPanel->AddPage(new Scenario(nameSetter->name), false);
}

void MainFrame::NewLoad()
{
	NameSetter* nameSetter = new NameSetter(this, "Enter load name", Load::ValidateName);
	nameSetter->ShowModal();
	if (nameSetter->ok) loadsPanel->AddPage(new Load(nameSetter->name), false);
}

void MainFrame::OnNew(wxMouseEvent& e) {
	NameSetter* nameSetter = new NameSetter(this, "Enter load name", Load::ValidateName);
	nameSetter->ShowModal();
	//if (nameSetter->ok) {
	//	loads.push_back(Load(nameSetter->name));
	//	LoadsPanel* loadPanel = new LoadPanel(&loads[loads.size() - 1], notebook);
	//	loadPanel->SetBackgroundColour(wxColour(255, 255, 255));
	//	if (!notebook->GetPageCount()) {
	//		emptyMessagePanel->Show(false);
	//		notebook->Show();
	//	}
	//	notebook->AddPage(loadPanel, nameSetter->name, true);
	//	Layout();
	//}
}

void MainFrame::OnSave(wxMouseEvent& e)
{
	switch (notebook->GetSelection())
	{
	case Scenarios:
		scenariosPanel->SaveCurrent();
		break;
	case Loads:
		loadsPanel->SaveCurrent();
		break;
	default:
		break;
	}
	e.Skip();
}

void MainFrame::OnScenarios(wxMouseEvent& e)
{
	ScenariosFrame* frame = new ScenariosFrame(this, false);
	frame->ShowModal();
}

