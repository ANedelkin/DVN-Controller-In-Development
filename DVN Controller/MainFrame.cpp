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

	SetUpToolBar();

	notebook = new wxNotebook(mainPanel, wxID_ANY);

	scenariosPanel = new ScenariosPanel(notebook, DELETABLE);
	scenariosPanel->Bind(EVT_UNSAVE, &SideNotebook::OnUnsave, scenariosPanel);

	loadsPanel = new LoadsPanel(notebook);
	loadsPanel->Bind(EVT_UNSAVE, &SideNotebook::OnUnsave, loadsPanel);

	notebook->AddPage(loadsPanel, "Loads");	
	notebook->AddPage(scenariosPanel, "Scenarios");
	
	//mainSizer->Add(toolBar, 0, wxEXPAND);
	mainSizer->Add(notebook, 0, wxEXPAND);

	mainPanel->SetSizer(mainSizer);

	topSizer->Add(mainPanel, 1, wxEXPAND);
	SetSizer(topSizer);

	notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &MainFrame::OnTabChanged, this);
	Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

void MainFrame::SetUpToolBar()
{
	toolBar = CreateToolBar(wxTB_FLAT | wxTB_NODIVIDER | wxTB_HORZ_TEXT | wxTB_NO_TOOLTIPS);
	
	newTool = new wxToolBarToolBase(toolBar, wxID_NEW, "New", wxArtProvider::GetBitmapBundle(wxART_NEW));
	openTool = new wxToolBarToolBase(toolBar, wxID_OPEN, "Open", wxArtProvider::GetBitmapBundle(wxART_FILE_OPEN));
	saveTool = new wxToolBarToolBase(toolBar, wxID_SAVE, "Save", wxArtProvider::GetBitmapBundle(wxART_FILE_SAVE));
	saveAsTool = new wxToolBarToolBase(toolBar, wxID_SAVEAS, "Save As", wxArtProvider::GetBitmapBundle(wxART_FILE_SAVE_AS));
	addTool = new wxToolBarToolBase(toolBar, wxID_ADD, "Add Existing", wxArtProvider::GetBitmapBundle(wxART_FILE_OPEN));
	loadToTool = new wxToolBarToolBase(toolBar, wxID_UP, "Load To Jammer", wxArtProvider::GetBitmapBundle(wxART_GO_UP));
	loadFromTool = new wxToolBarToolBase(toolBar, wxID_DOWN, "Load From Jammer", wxArtProvider::GetBitmapBundle(wxART_GO_DOWN));
	aboutTool = new wxToolBarToolBase(toolBar, wxID_UP, "Load To Jammer", wxArtProvider::GetBitmapBundle(wxART_INFORMATION, wxART_TOOLBAR));

	toolBar->AddTool(newTool);
	toolBar->AddTool(openTool);
	toolBar->AddTool(saveTool);
	toolBar->AddTool(saveAsTool);
	toolBar->AddSeparator();
	toolBar->AddTool(loadToTool);
	toolBar->AddTool(loadFromTool);
	toolBar->AddStretchableSpace();
	toolBar->AddTool(aboutTool);

	toolBar->Bind(wxEVT_TOOL, &MainFrame::OnNew, this, wxID_NEW);
	toolBar->Bind(wxEVT_TOOL, &MainFrame::OnOpen, this, wxID_OPEN);
	toolBar->Bind(wxEVT_TOOL, &MainFrame::OnSave, this, wxID_SAVE);
	toolBar->Bind(wxEVT_TOOL, &MainFrame::OnAdd, this, wxID_ADD);
	toolBar->Bind(wxEVT_TOOL, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
	toolBar->Bind(wxEVT_TOOL, &MainFrame::OnLoadToJmr, this, wxID_UP);
	toolBar->Bind(wxEVT_TOOL, &MainFrame::OnLoadFromJmr, this, wxID_DOWN);
	toolBar->Bind(wxEVT_TOOL, &MainFrame::OnAbout, this, wxID_ABOUT);

	toolBar->Realize();
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
		toolBar->RemoveTool(wxID_ADD);
		toolBar->InsertTool(1, openTool);
		toolBar->InsertTool(3, saveAsTool);
		toolBar->InsertSeparator(4);
		toolBar->InsertTool(5, loadToTool);
		toolBar->InsertTool(6, loadFromTool);
	}
	else {
		toolBar->DeleteToolByPos(4);
		toolBar->InsertTool(2, addTool);
		toolBar->RemoveTool(wxID_OPEN);
		toolBar->RemoveTool(wxID_SAVEAS);
		toolBar->RemoveTool(wxID_UP);
		toolBar->RemoveTool(wxID_DOWN);
		if (scenariosPanel->GetPages().size())
			UpdateScenarios();
		else
			LoadScenarios();
	}
	toolBar->Realize();
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