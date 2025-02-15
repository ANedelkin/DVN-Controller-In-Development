#pragma once
#include "Global.h"

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
	wxButton* addBtn;
	#define DEV_COMBOBOX_ID wxID_HIGHEST + 1
	wxPanel* toolBar;
	wxStaticLine* separator;
	wxStaticText* selJammLabel;
	wxComboBox* devComboBox;
	wxButton* loadToBtn;
	wxButton* loadFromBtn;
	wxButton* aboutBtn;

	void CreateToolBar();

	void NewBand();
	void NewScenario();
	void NewLoad();

	void OnTabChanged(wxNotebookEvent& e);
	void OnNew(wxMouseEvent& e);
public:
	BandsPanel* bandsPanel;
	MainFrame(const string& title);
};