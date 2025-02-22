#pragma once
#include "Global.h"

#include "NameSetter.h"
#include "BandsPanel.h"
#include "SideNotebook.h"
#include  "ScenariosFrame.h"

#include "ScenariosPanel.h"
#include "LoadsPanel.h"

class MainFrame : public wxFrame
{
private:
	wxNotebook* notebook;
	SideNotebook* scenariosPanel;
	SideNotebook* loadsPanel;

	wxButton* newBtn;
	wxButton* openBtn;
	wxButton* saveBtn;
	wxButton* scenBtn;
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

	void OnNew(wxMouseEvent& e);
	void OnSave(wxMouseEvent& e);
	void OnScenarios(wxMouseEvent& e);
public:
	BandsPanel* bandsPanel;
	MainFrame(const string& title);
};