#include "MainFrame.h"

MainFrame::MainFrame(const string& title) : wxFrame(nullptr, wxID_ANY, title) {
	SetMinSize(wxSize(1000, 710));

	base = this;

	toolBar = CreateToolBar(wxTB_HORIZONTAL | wxTB_TEXT | wxTB_HORZ_LAYOUT, wxID_ANY);
	toolBar->AddTool(wxID_NEW, wxT("New"), wxArtProvider::GetBitmap(wxART_PLUS));
	toolBar->AddTool(wxID_OPEN, wxT("Add existing"), wxArtProvider::GetBitmap(wxART_FILE_OPEN));
	toolBar->AddSeparator();
	toolBar->AddStretchableSpace();
	toolBar->AddTool(wxID_ABOUT, wxT("About"), wxArtProvider::GetBitmap(wxART_INFORMATION));
	toolBar->Realize();

	devCtrl = new wxControl(toolBar, DEV_COMBOBOX_ID);
	wxBoxSizer* devCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
	wxComboBox* devComboBox = new wxComboBox(devCtrl, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1), 0, nullptr, wxCB_READONLY);
	wxStaticText* label = new wxStaticText(devCtrl, wxID_ANY, "Select jammer: ");
	devCtrlSizer->Add(label, 0, wxALIGN_CENTER);
	devCtrlSizer->Add(devComboBox, 0);
	devCtrl->SetSizerAndFit(devCtrlSizer);
	devCtrl->Hide();

	notebook = new wxNotebook(this, wxID_ANY);
	scenariosPanel = new ScenariosPanel(notebook);
	loadsPanel = new wxPanel(notebook);
	notebook->AddPage(scenariosPanel, "Scenarios");
	notebook->AddPage(loadsPanel, "Loads");	
	
	notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &MainFrame::OnTabChanged, this);
	toolBar->Bind(wxEVT_TOOL, &MainFrame::OnNew, this, wxID_NEW);
}

void MainFrame::NewScenario()
{
	NameSetter* nameSetter = new NameSetter(this, "Enter scenario name", Scenario::ValidateName);
	nameSetter->ShowModal();
	if (nameSetter->ok) scenariosPanel->AddScenario(new Scenario(nameSetter->name));
}

void MainFrame::NewLoad()
{
}

void MainFrame::OnTabChanged(wxNotebookEvent& e) {
	if (e.GetSelection() == 2) {
		toolBar->InsertControl(2, devCtrl);
		toolBar->InsertTool(3, wxID_UP, wxT("Upload to jammer"), wxArtProvider::GetBitmap(wxART_GO_UP));
		toolBar->InsertTool(4, wxID_DOWN, wxT("Extract from jammer"), wxArtProvider::GetBitmap(wxART_GO_DOWN));
	}
	else {
		toolBar->RemoveTool(DEV_COMBOBOX_ID);
		devCtrl->Hide();
		toolBar->RemoveTool(wxID_UP);
		toolBar->RemoveTool(wxID_DOWN);
	}
	toolBar->Realize();
}

void MainFrame::OnNew(wxCommandEvent& e) {
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

