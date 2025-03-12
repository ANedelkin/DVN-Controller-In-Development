#pragma once
#include "Global.h"

#include "Icon.xpm"

#include "NameSetter.h"
#include "BandsPanel.h"
#include "SideNotebook.h"

#include "ScenariosPanel.h"
#include "LoadsPanel.h"

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
	wxStaticText* selJammLabel;
	wxComboBox* devComboBox;
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
	void OnClose(wxCloseEvent& e);
public:
	MainFrame(const string& title);
};