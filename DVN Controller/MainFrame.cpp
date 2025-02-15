#include "MainFrame.h"

MainFrame::MainFrame(const string& title) : wxFrame(nullptr, wxID_ANY, title) {
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

	notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &MainFrame::OnTabChanged, this);
}

void MainFrame::CreateToolBar()
{
	toolBar = new wxPanel(this, wxID_ANY);
	wxBoxSizer* toolBarSizer = new wxBoxSizer(wxHORIZONTAL);

	#define CTRL_HEIGHT 30

	newBtn = new wxButton(toolBar, wxID_ANY, "New");
	//newBtn->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS));
	addBtn = new wxButton(toolBar, wxID_ANY, "Add existing");
	//addBtn->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN));

	separator = new wxStaticLine(toolBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVERTICAL);

	selJammLabel = new wxStaticText(toolBar, wxID_ANY, "Select jammer: ");
	devComboBox = new wxComboBox(toolBar, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(150, -1)), 0, nullptr, wxCB_READONLY);
	selJammLabel->Hide();
	devComboBox->Hide();


	loadToBtn = new wxButton(toolBar, wxID_UP, "Load to jammer");
	//loadToBtn->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_UP));
	loadFromBtn = new wxButton(toolBar, wxID_DOWN, "Load from jammer");
	//loadFromBtn->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN));
	loadToBtn->Hide();
	loadFromBtn->Hide();

	aboutBtn = new wxButton(toolBar, wxID_ANY, "About");
	//aboutBtn->SetBitmap(wxArtProvider::GetBitmap(wxART_HELP));

	#define PADDING FromDIP(5)

	toolBarSizer->Add(newBtn, 0, wxEXPAND | wxALL, PADDING);
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

void MainFrame::OnTabChanged(wxNotebookEvent& e) {
	if (e.GetSelection() == 1) {
		separator->Show();
		selJammLabel->Show();
		devComboBox->Show();
		loadToBtn->Show();
		loadFromBtn->Show();
		Layout();
	}
	else {
		separator->Hide();
		selJammLabel->Hide();
		devComboBox->Hide();
		loadToBtn->Hide();
		loadFromBtn->Hide();
	}
}

void MainFrame::OnNew(wxMouseEvent& e) {
	switch (notebook->GetSelection())
	{
	//case 0:
	//	NewBand();
	//	break;
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

