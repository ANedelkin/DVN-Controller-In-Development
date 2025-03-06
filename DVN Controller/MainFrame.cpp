#include "MainFrame.h"

MainFrame::MainFrame(const string& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxIcon icon(ICON);
	SetIcon(icon);

	SetMinSize(FromDIP(wxSize(1122, 710)));

	base = this;

	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	CreateToolBar();

	notebook = new wxNotebook(mainPanel, wxID_ANY);

	scenariosPanel = new SideNotebook(notebook, "Scenarios");
	BandsPanel* scenBandsPanel = new BandsPanel(scenariosPanel, new Scenario());
	scenBandsPanel->UnInit();
	scenariosPanel->SetContent(scenBandsPanel);
	LoadScenarios();
	scenariosPanel->Bind(EVT_UNSAVE, &SideNotebook::OnUnsave, scenariosPanel);

	loadsPanel = new SideNotebook(notebook, "Loads");
	SideNotebook* loadScenPanel = new SideNotebook(loadsPanel, "Scenarios", new Load());
	BandsPanel* loadBandsPanel = new BandsPanel(loadScenPanel, new Scenario());
	loadScenPanel->SetContent(loadBandsPanel);
	loadScenPanel->UnInit();
	loadsPanel->SetContent(loadScenPanel);
	loadsPanel->Bind(EVT_UNSAVE, &SideNotebook::OnUnsave, loadsPanel);

	notebook->AddPage(scenariosPanel, "Scenarios");
	notebook->AddPage(loadsPanel, "Loads");	
	
	mainSizer->Add(toolBar, 0, wxEXPAND);
	mainSizer->Add(notebook, 0, wxEXPAND);

	mainPanel->SetSizer(mainSizer);

	topSizer->Add(mainPanel, 1, wxEXPAND);
	SetSizer(topSizer);

	notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &MainFrame::OnTabChanged, this);
}

void MainFrame::CreateToolBar()
{
	toolBar = new wxPanel(mainPanel, wxID_ANY);
	wxBoxSizer* toolBarSizer = new wxBoxSizer(wxHORIZONTAL);

	#define CTRL_HEIGHT 30

	newBtn = new wxButton(toolBar, wxID_ANY, "New");
	openBtn = new wxButton(toolBar, wxID_ANY, "Open");
	openBtn->Hide();
	saveBtn = new wxButton(toolBar, wxID_ANY, "Save");
	addBtn = new wxButton(toolBar, wxID_ANY, "Add existing");

	separator = new wxStaticLine(toolBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVERTICAL);
	separator->Hide();

	selJammLabel = new wxStaticText(toolBar, wxID_ANY, "Select jammer: ");
	devComboBox = new wxComboBox(toolBar, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(150, -1)), 0, nullptr, wxCB_READONLY);
	selJammLabel->Hide();
	devComboBox->Hide();


	loadToBtn = new wxButton(toolBar, wxID_UP, "Load to jammer");
	loadFromBtn = new wxButton(toolBar, wxID_DOWN, "Load from jammer");
	loadToBtn->Hide();
	loadFromBtn->Hide();

	aboutBtn = new wxButton(toolBar, wxID_ANY, "About");

	#define PADDING FromDIP(5)

	toolBarSizer->Add(newBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(openBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(saveBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(addBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(separator, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(selJammLabel, 0, wxALIGN_CENTER | wxALL, PADDING);
	toolBarSizer->Add(devComboBox, 0, wxALIGN_CENTER | wxRIGHT | wxTOP | wxBOTTOM, PADDING + 1);
	toolBarSizer->Add(loadToBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(loadFromBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->AddStretchSpacer();
	toolBarSizer->Add(aboutBtn, 0, wxEXPAND | wxALL, PADDING);

	toolBar->SetSizerAndFit(toolBarSizer);

	newBtn->Bind(wxEVT_LEFT_UP, &MainFrame::OnNew, this);
	openBtn->Bind(wxEVT_LEFT_UP, &MainFrame::OnOpen, this);
	saveBtn->Bind(wxEVT_LEFT_UP, &MainFrame::OnSave, this);

	Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

void MainFrame::LoadScenarios()
{
	vector<Scenario*> scenarios = Scenario::LoadScenarios();
	for (char i = 0; i < scenarios.size(); i++)
	{
		scenariosPanel->AddPage(scenarios[i], false);
	}
}

void MainFrame::NewScenario()
{
	NameSetter* nameSetter = new NameSetter(this, "Enter scenario name", Scenario::ValidateName);
	nameSetter->ShowModal();
	if (nameSetter->ok) scenariosPanel->AddPage(new Scenario(nameSetter->name), false);
	scenariosPanel->Unsave(true);
}

void MainFrame::NewLoad()
{
	NameSetter* nameSetter = new NameSetter(this, "Enter load name", Load::ValidateName);
	nameSetter->ShowModal();
	if (nameSetter->ok)  {
		loadsPanel->AddPage(new Load(nameSetter->name), false);
		loadsPanel->Unsave(true);
	}
}

void MainFrame::OnTabChanged(wxNotebookEvent& e) {
	if (e.GetSelection() == Loads) {
		addBtn->Hide();
		openBtn->Show();
		separator->Show();
		selJammLabel->Show();
		devComboBox->Show();
		loadToBtn->Show();
		loadFromBtn->Show();
		Layout();
	}
	else {
		addBtn->Show();
		openBtn->Hide();
		separator->Hide();
		selJammLabel->Hide();
		devComboBox->Hide();
		loadToBtn->Hide();
		loadFromBtn->Hide();
	}
	Layout();
}

void MainFrame::OnNew(wxMouseEvent& e) {
	switch (notebook->GetSelection())
	{
	case Scenarios:
		NewScenario();
		break;
	case Loads:
		NewLoad();
		break;
	default:
		break;
	}
}

void MainFrame::OnOpen(wxMouseEvent& e)
{
	wxFileDialog dialog(this, "Select Load/s", "", "", "Load files (*.dvnl)|*.dvnl", wxFD_MULTIPLE);
	wxArrayString paths;

	if (dialog.ShowModal() == wxID_OK) {
		dialog.GetPaths(paths);
		for (const auto& path : paths)
		{
			ifstream stream(path.ToStdString());
			stringstream data;
			data << stream.rdbuf();
			wxFileName fn(path);
			const string name = fn.GetName().ToStdString();
			loadsPanel->AddPage(Load::ToLoad(name, fn.GetPath().ToStdString(), data), false);
		}
	}
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

void MainFrame::OnClose(wxCloseEvent& e)
{
	if (!e.CanVeto() || loadsPanel->CheckForUnsaved() && scenariosPanel->CheckForUnsaved()) {
		e.Skip();
		return;
	}
	e.Veto(); //It's presitentin' time
}