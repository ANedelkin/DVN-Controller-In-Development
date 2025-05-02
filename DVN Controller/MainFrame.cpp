#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, string(JAMMER_NAME) + " Controller") {
	wxIcon icon(ICON);
	SetIcon(icon);

	wxSize size = FromDIP(wxSize(1122, 710));

	SetMinSize(size);
	SetSize(size);

	base = this;

	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	CreateToolBar();

	notebook = new wxNotebook(mainPanel, wxID_ANY);

	scenariosPanel = new ScenariosPanel(notebook, DELETABLE);
	scenariosPanel->Bind(EVT_UNSAVE, &SideNotebook::OnUnsave, scenariosPanel);

	loadsPanel = new LoadsPanel(notebook);
	loadsPanel->Bind(EVT_UNSAVE, &SideNotebook::OnUnsave, loadsPanel);

	notebook->AddPage(loadsPanel, "Loads");	
	notebook->AddPage(scenariosPanel, "Scenarios");
	
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
	saveBtn = new wxButton(toolBar, wxID_ANY, "Save");
	saveAsBtn = new wxButton(toolBar, wxID_ANY, "Save As");
	addBtn = new wxButton(toolBar, wxID_ANY, "Add existing");
	addBtn->Hide();

	separator = new wxStaticLine(toolBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVERTICAL);

	loadToBtn = new wxButton(toolBar, wxID_UP, "Load to jammer");
	loadFromBtn = new wxButton(toolBar, wxID_DOWN, "Load from jammer");

	aboutBtn = new wxButton(toolBar, wxID_ANY, "About");

	#define PADDING FromDIP(5)

	toolBarSizer->Add(newBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(openBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(saveBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(saveAsBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(addBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(separator, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(loadToBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->Add(loadFromBtn, 0, wxEXPAND | wxALL, PADDING);
	toolBarSizer->AddStretchSpacer();
	toolBarSizer->Add(aboutBtn, 0, wxEXPAND | wxALL, PADDING);

	toolBar->SetSizerAndFit(toolBarSizer);

	newBtn->Bind(wxEVT_BUTTON, &MainFrame::OnNew, this);
	openBtn->Bind(wxEVT_BUTTON, &MainFrame::OnOpen, this);
	saveBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSave, this);
	addBtn->Bind(wxEVT_BUTTON, &MainFrame::OnAdd, this);
	saveAsBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSaveAs, this);
	loadToBtn->Bind(wxEVT_BUTTON, &MainFrame::OnLoadToJmr, this);
	loadFromBtn->Bind(wxEVT_BUTTON, &MainFrame::OnLoadFromJmr, this);
	aboutBtn->Bind(wxEVT_BUTTON, &MainFrame::OnAbout, this);

	Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

void MainFrame::LoadScenarios()
{
	vector<Scenario*> scenarios = Scenario::LoadScenarios();
	Freeze();
	if (scenarios.size() > 0) {
		for (char i = 0; i < scenarios.size(); i++)
		{
			if (scenarios[i]->ok)
				scenariosPanel->NewPage(scenarios[i]);
		}
		scenariosPanel->Select(0);
	}
	Thaw();
}

void MainFrame::UpdateScenarios()
{
	vector<Scenario*> scenarios = Scenario::LoadScenarios();
	vector<SideMenuCtrl*> pages = scenariosPanel->GetPages();
	for (char i = 0; i < scenarios.size(); i++)
	{
		if (scenarios[i]->ok) {
			bool f = true;
			for (char j = 0; j < pages.size() && f; j++)
			{
				if (scenarios[i]->GetPath() == pages[j]->GetSource()->GetPath())
					f = false;
			}
			if (f) scenariosPanel->NewPage(scenarios[i]);
			else delete scenarios[i];
		}
	}
}

void MainFrame::NewScenario()
{
	NameSetter nameSetter(this, "Enter scenario name", Scenario::ValidateNameUnique);
	nameSetter.ShowModal();
	if (nameSetter.ok && !scenariosPanel->NewPage(new Scenario(nameSetter.name))) {
		scenariosPanel->Unsave(true);
	}
}

void MainFrame::NewLoad()
{
	NameSetter nameSetter(this, "Enter load name", Load::ValidateName);
	nameSetter.ShowModal();
	if (nameSetter.ok && !loadsPanel->NewPage(new Load(nameSetter.name))) loadsPanel->Unsave(true);
}

void MainFrame::OnTabChanged(wxNotebookEvent& e) {
	if (e.GetSelection() == Loads) {
		addBtn->Hide();
		openBtn->Show();
		saveAsBtn->Show();
		separator->Show();
		loadToBtn->Show();
		loadFromBtn->Show();
		Layout();
	}
	else {
		addBtn->Show();
		openBtn->Hide();
		saveAsBtn->Hide();
		separator->Hide();
		loadToBtn->Hide();
		loadFromBtn->Hide();
		if (scenariosPanel->GetPages().size())
			UpdateScenarios();
		else
			LoadScenarios();
	}
	Layout();
}

void MainFrame::OnNew(wxCommandEvent& e) {
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

void MainFrame::OnOpen(wxCommandEvent& e)
{
	wxFileDialog dialog(this, "Select Load/s", "", "", "Load files (*.dvnl)|*.dvnl", wxFD_MULTIPLE);
	wxArrayString paths;

	Freeze();
	if (dialog.ShowModal() == wxID_OK) {
		dialog.GetPaths(paths);
		for (const auto& path : paths)
		{
			ifstream stream(path.ToStdString());
			wxFileName fn(path);
			const string name = fn.GetName().ToStdString();
			if (stream.is_open()) {
				stringstream data;
				data << stream.rdbuf();
				Load* load = Load::ToLoad(name, fn.GetPath().ToStdString(), data);
				if (load->ok)
					loadsPanel->NewPage(load);
				else
					ShowError(this, ToString(InvalidFile, name.c_str()));
			}
			else ShowError(this, ToString(FileNonexistent, name.c_str()));
		}
	}
	Thaw();
}

void MainFrame::OnAdd(wxCommandEvent& e)
{
	wxFileDialog dialog(this, "Select Scenario/s", "", "", "Scenario files (*.dvns)|*.dvns", wxFD_MULTIPLE);
	wxArrayString paths;

	if (dialog.ShowModal() == wxID_OK) {
		dialog.GetPaths(paths);
		for (const auto& path : paths)
		{
			ifstream stream(path.ToStdString());
			wxFileName fn(path);
			const string name = fn.GetName().ToStdString();
			if (stream.is_open()) {
				stringstream data;
				data << stream.rdbuf();
				Scenario* scenario = Scenario::ToScenario(name, data);
				if (scenario->ok) {
					scenariosPanel->NewPage(scenario);
					scenariosPanel->SaveCurrent();
				}
				else
					ShowError(this, ToString(InvalidFile, name.c_str()));
			}
			else ShowError(this, ToString(FileNonexistent, name.c_str()));
		}
	}
}

void MainFrame::OnSave(wxCommandEvent& e)
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

void MainFrame::OnSaveAs(wxCommandEvent& e)
{
	loadsPanel->SaveCurrentAs();
}

void MainFrame::OnLoadFromJmr(wxCommandEvent& e)
{
	JammersWindow jammersWindow(this);
	jammersWindow.ShowModal();
}

void MainFrame::OnLoadToJmr(wxCommandEvent& e)
{
	JammersWindow jammersWindow(this);
	jammersWindow.ShowModal();
}

void MainFrame::OnAbout(wxCommandEvent& e)
{
	AboutDialog(this).ShowModal();
}

void MainFrame::OnClose(wxCloseEvent& e)
{
	if (!e.CanVeto() || loadsPanel->CheckForUnsaved() && scenariosPanel->CheckForUnsaved()) {
		e.Skip();
		return;
	}
	e.Veto(); //It's presitentin' time
}