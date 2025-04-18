#pragma once
#include "Global.h"

#include "Icon.xpm"

#include "Load.h"
#include "Scenario.h"
#include "NameSetter.h"
#include "BandsPanel.h"
#include "SideNotebook.h"
#include "ScenariosPanel.h"
#include "LoadsPanel.h"
#include "JammersWindow.h"

class MainFrame : public wxFrame
{
private:
	enum Tabs {
		Loads,
		Scenarios,
	};
	wxPanel* mainPanel;
	wxNotebook* notebook;
	ScenariosPanel* scenariosPanel;
	LoadsPanel* loadsPanel;

	wxPanel* emptyPanel;
	wxStaticText* emptyText;

	wxButton* newBtn;
	wxButton* openBtn;
	wxButton* saveBtn;
	wxButton* saveAsBtn;
	wxButton* addBtn;
	wxPanel* toolBar;
	wxStaticLine* separator;
	wxButton* loadToBtn;
	wxButton* loadFromBtn;
	wxButton* aboutBtn;

	void CreateToolBar();

	void LoadScenarios();
	void UpdateScenarios();

	void NewScenario();
	void NewLoad();

	void OnTabChanged(wxNotebookEvent& e);
	void OnNew(wxCommandEvent& e);
	void OnOpen(wxCommandEvent& e);
	void OnSave(wxCommandEvent& e);
	void OnSaveAs(wxCommandEvent& e);
	void OnLoadFromJmr(wxCommandEvent& e);
	void OnLoadToJmr(wxCommandEvent& e);
	void OnClose(wxCloseEvent& e);
public:
	MainFrame(const string& title);
};