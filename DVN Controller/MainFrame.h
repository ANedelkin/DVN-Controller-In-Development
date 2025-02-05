#pragma once
#include "Global.h"
#include "NameSetter.h"
#include "BandsPanel.h"
#include "ScenariosPanel.h"

class MainFrame : public wxFrame
{
private:
	wxToolBar* toolBar;
	wxNotebook* notebook;
	ScenariosPanel* scenariosPanel;
	wxPanel* loadsPanel;

	wxPanel* emptyPanel;
	wxStaticText* emptyText;

	wxControl* devCtrl;
	#define DEV_COMBOBOX_ID wxID_HIGHEST + 1

	void NewBand();
	void NewScenario();
	void NewLoad();

	void OnTabChanged(wxNotebookEvent& e);
	void OnNew(wxCommandEvent& e);
public:
	BandsPanel* bandsPanel;
	MainFrame(const string& title);
};