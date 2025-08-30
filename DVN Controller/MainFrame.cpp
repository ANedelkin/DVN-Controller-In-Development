#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, string(JAMMER_NAME) + " Controller") {
	wxIcon icon(ICON);
	SetIcon(icon);

	wxSize size = FromDIP(wxSize(1090, 710));

	SetMinSize(size);
	SetSize(size);

	base = this;

	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	SetUpToolBars();
	statusBar = StatusBar(this);

	notebook = new wxNotebook(mainPanel, wxID_ANY);
	notebook->Bind(wxEVT_SET_FOCUS, [this](wxFocusEvent& e) {
										if (notebook->GetSelection() == Loads)
											loadsPanel->SetFocus();
										else
											scenariosPanel->SetFocus();
									});

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

void MainFrame::SetUpToolBars()
{
	scenariosToolBar = CreateToolBar(wxTB_FLAT | wxTB_NODIVIDER | wxTB_HORZ_TEXT | wxTB_NO_TOOLTIPS);

	scenariosToolBar->AddTool(wxID_NEW, "New", wxBitmapBundle::FromSVG(newSVG, TOOLBAR_ICON_SIZE));
	scenariosToolBar->AddTool(wxID_ADD, "Add Existing", wxBitmapBundle::FromSVG(openSVG, TOOLBAR_ICON_SIZE));
	scenariosToolBar->AddTool(wxID_SAVE, "Save", wxBitmapBundle::FromSVG(saveSVG, TOOLBAR_ICON_SIZE));
	scenariosToolBar->AddStretchableSpace();
	scenariosToolBar->AddTool(wxID_ABOUT, "About", wxBitmapBundle::FromSVG(aboutSVG, TOOLBAR_ICON_SIZE));

	Bind(wxEVT_TOOL, &MainFrame::OnNew, this, wxID_NEW);
	Bind(wxEVT_TOOL, &MainFrame::OnAdd, this, wxID_ADD);
	Bind(wxEVT_TOOL, &MainFrame::OnSave, this, wxID_SAVE);
	Bind(wxEVT_TOOL, &MainFrame::OnAbout, this, wxID_ABOUT);

	scenariosToolBar->Realize();
	SetToolBar(nullptr);

	loadsToolBar = CreateToolBar(wxTB_FLAT | wxTB_NODIVIDER | wxTB_HORZ_TEXT | wxTB_NO_TOOLTIPS);

	loadsToolBar->AddTool(wxID_NEW, "New", wxBitmapBundle::FromSVG(newSVG, TOOLBAR_ICON_SIZE));
	loadsToolBar->AddTool(wxID_OPEN, "Open", wxBitmapBundle::FromSVG(openSVG, TOOLBAR_ICON_SIZE));
	loadsToolBar->AddTool(wxID_SAVE, "Save", wxBitmapBundle::FromSVG(saveSVG, TOOLBAR_ICON_SIZE));
	loadsToolBar->AddTool(wxID_SAVEAS, "Save As", wxBitmapBundle::FromSVG(saveAsSVG, TOOLBAR_ICON_SIZE));
	loadsToolBar->AddSeparator();
	loadsToolBar->AddTool(wxID_UP, "Load To Jammer", wxBitmapBundle::FromSVG(upSVG, TOOLBAR_ICON_SIZE));
	loadsToolBar->AddTool(wxID_DOWN, "Load From Jammer", wxBitmapBundle::FromSVG(downSVG, TOOLBAR_ICON_SIZE));
	loadsToolBar->AddStretchableSpace();
	loadsToolBar->AddTool(wxID_ABOUT, "About", wxBitmapBundle::FromSVG(aboutSVG, TOOLBAR_ICON_SIZE));

	Bind(wxEVT_TOOL, &MainFrame::OnOpen, this, wxID_OPEN);
	Bind(wxEVT_TOOL, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
	Bind(wxEVT_TOOL, &MainFrame::OnLoadToJmr, this, wxID_UP);
	Bind(wxEVT_TOOL, &MainFrame::OnLoadFromJmr, this, wxID_DOWN);
	Bind(wxEVT_TOOL, &MainFrame::OnAbout, this, wxID_ABOUT);
	
	loadsToolBar->Show();
	SetToolBar(loadsToolBar);
	scenariosToolBar->Hide();
	loadsToolBar->Realize();

	wxAcceleratorEntry entries[8];
	entries[0].Set(wxACCEL_CTRL, (int)'N', wxID_NEW);
	entries[1].Set(wxACCEL_CTRL, (int)'O', wxID_OPEN);
	entries[2].Set(wxACCEL_CTRL, (int)'A', wxID_ADD);
	entries[3].Set(wxACCEL_CTRL, (int)'S', wxID_SAVE);
	entries[4].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'S', wxID_SAVEAS);
	entries[5].Set(wxACCEL_CTRL, (int)'T', wxID_UP);
	entries[6].Set(wxACCEL_CTRL, (int)'F', wxID_DOWN);
	entries[7].Set(wxACCEL_CTRL, (int)'I', wxID_ABOUT);

	wxAcceleratorTable accelTable(8, entries);
	SetAcceleratorTable(accelTable);
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
		scenariosPanel->MarkPagesValidity();
	}
	Thaw();
}

void MainFrame::UpdateScenarios()
{
	vector<Scenario*> scenarios = Scenario::LoadScenarios();
	vector<SideMenuCtrl*> pages = scenariosPanel->GetPages();

	Freeze();
	for (char i = 0; i < scenarios.size(); i++)
	{
		if (scenarios[i]) {
			bool f = true;
			for (char j = 0; j < pages.size() && f; j++)
			{
				if (scenarios[i]->GetPath() == pages[j]->GetSource()->GetPath()) {
					if (pages[j]->GetSource()->upToDate)
						*(Scenario*)pages[j]->GetSource() = *scenarios[i];
					delete scenarios[i];
					f = false;
				}
			}
			if (f) scenariosPanel->NewPage(scenarios[i]);
		}
	}
	scenariosPanel->MarkPagesValidity();
	scenariosPanel->Select(0);
	Thaw();
}

void MainFrame::NewScenario()
{
	NameSetter nameSetter(this, "Enter scenario name", Scenario::ValidateNameUnique);
	nameSetter.ShowModal();
	if (nameSetter.ok && !scenariosPanel->NewPage(new Scenario(nameSetter.name))) {
		scenariosPanel->SaveCurrent();
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
		loadsToolBar->Show();
		SetToolBar(loadsToolBar);
		scenariosToolBar->Hide();
		loadsToolBar->Realize();
	}
	else {
		scenariosToolBar->Show();
		SetToolBar(scenariosToolBar);
		loadsToolBar->Hide();
		scenariosToolBar->Realize();
		if (scenariosPanel->GetPages().size())
			UpdateScenarios();
		else
			LoadScenarios();
	}
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
	if (notebook->GetSelection() != Loads) return;
	wxFileDialog dialog(this, "Select Load/s", "", "", "Load files (*.jld)|*.jld", wxFD_MULTIPLE);
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
				if (!CheckModel(data)) {
					ShowError(this, ToString(InvalidJammer, name.c_str(), JAMMER_NAME));
					continue;
				}
				Load* load = Load::ToLoad(name, fn.GetPath().ToStdString(), data);
				if (load->ok)
					loadsPanel->NewPage(load);
				else
					ShowError(this, ToString(InvalidFileStructure, name.c_str()));
			}
			else ShowError(this, ToString(FileNonexistent, name.c_str()));
		}
	}
	Thaw();
}

void MainFrame::OnAdd(wxCommandEvent& e)
{
	if (notebook->GetSelection() != Scenarios) return;
	wxFileDialog dialog(this, "Select Scenario/s", "", "", "Scenario files (*.jsc)|*.jsc", wxFD_MULTIPLE);
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
				if (!CheckModel(data)) {
					ShowError(this, ToString(InvalidJammer, name.c_str(), JAMMER_NAME));
					continue;
				}
				Scenario* scenario = new Scenario(Scenario::ToScenario(name, data, true));
				if (scenario->ok) {
					scenariosPanel->NewPage(scenario);
					scenariosPanel->SaveCurrent();
				}
				else
					ShowError(this, ToString(InvalidFileStructure, name.c_str()));
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
	if (notebook->GetSelection() != Loads) return;
	loadsPanel->SaveCurrentAs();
}

void MainFrame::OnLoadFromJmr(wxCommandEvent& e)
{
	if (notebook->GetSelection() != Loads) return;
	JammersWindow jammersWindow(this);
	if (jammersWindow.ShowModal() == wxID_CANCEL) return;
	wxProgressDialog progressDialog("Getting load", wxString::Format("Scenario: %d/%d", 0, SCENARIOS_COUNT), SCENARIOS_COUNT, this);
	Load* load = new Load();
	vector<tuple<char, char>> brokenBands;

	if (JammersManager::GetLoad(jammersWindow.GetSerNum(), load, &brokenBands, [&progressDialog](int progress, wxString msg)
		{ return progressDialog.Update(progress, msg); })) {

		if (brokenBands.size())
			ShowError(this, ToString(InvalidBands));

		loadsPanel->NewPage(load);
		loadsPanel->Unsave(true);
		return;
	}
	else {
		ShowError(this, ToString(ConnectionError));
		delete load;
	}

}

void MainFrame::OnLoadToJmr(wxCommandEvent& e)
{
	if (notebook->GetSelection() != Loads) return;
	if (!loadsPanel->GetCurrent()) return;
	JammersWindow jammersWindow(this);
	if (jammersWindow.ShowModal() == wxID_CANCEL) return;
	wxProgressDialog progressDialog("Sending load", wxString::Format("Scenario: %d/%d", 0, SCENARIOS_COUNT), SCENARIOS_COUNT, this);
	JammersManager::SendLoad(jammersWindow.GetSerNum(), (Load*)loadsPanel->GetCurrent()->GetSource(), [&progressDialog](int progress, wxString msg) 
							 { return progressDialog.Update(progress, msg); } );
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