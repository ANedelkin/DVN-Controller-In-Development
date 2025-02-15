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
	//wxToolBar* toolBar;
	wxNotebook* notebook;
	SideNotebook* scenariosPanel;
	SideNotebook* loadsPanel;

	wxPanel* emptyPanel;
	wxStaticText* emptyText;

	wxButton* newBtn;
	wxButton* saveBtn;
	wxButton* addBtn;
	wxPanel* toolBar;
	wxStaticLine* separator;
	wxStaticText* selJammLabel;
	wxComboBox* devComboBox;
	wxButton* loadToBtn;
	wxButton* loadFromBtn;
	wxButton* aboutBtn;

	void CreateToolBar();

	//void NewBand();
	void NewScenario();
	void NewLoad();

	void OnTabChanged(wxNotebookEvent& e);
	void OnNew(wxMouseEvent& e);
	void OnSave(wxMouseEvent& e);
public:
	BandsPanel* bandsPanel;
	MainFrame(const string& title);
};