#pragma once

#include "Global.h"
#include "Scenario.h"
#include "SideNotebook.h"
#include "BandsPanel.h"
#include "ScenariosPanel.h"

class ScenariosFrame : public wxDialog
{
private:
	bool isForSelecting;

	wxPanel* emptyPanel;
	wxStaticText* emptyText;

	wxButton* newBtn;
	wxButton* addBtn;
	wxButton* saveBtn;
	wxPanel* toolBar;

	SideNotebook* scenariosPanel;

	void CreateToolBar();

	void OnNew(wxMouseEvent& e);
	void OnSave(wxMouseEvent& e);
	void OnAdd(wxMouseEvent& e);
public:
	Scenario* scenario;
	ScenariosFrame(wxWindow* parent, bool isForSelecting);
};

