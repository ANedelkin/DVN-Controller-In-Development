#pragma once
#include "Global.h"

#include "Icon.xpm"

#include "Load.h"
#include "Scenario.h"
#include "NameSetter.h"
#include "BandsPanel.h"
#include "SideNotebook.h"
#include "JammersWindow.h"

class MainFrame : public wxFrame
{
private:
	wxPanel* mainPanel;
	wxNotebook* notebook;
	SideNotebook* scenariosPanel;
	SideNotebook* loadsPanel;

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

	void NewScenario();
	void NewLoad();

	void OnTabChanged(wxNotebookEvent& e);
	void OnNew(wxMouseEvent& e);
	void OnOpen(wxMouseEvent& e);
	void OnSave(wxMouseEvent& e);
	void OnSaveAs(wxMouseEvent& e);
	void OnLoadFromJmr(wxMouseEvent& e);
	void OnLoadToJmr(wxMouseEvent& e);
	void OnClose(wxCloseEvent& e);
public:
	MainFrame(const string& title);
};