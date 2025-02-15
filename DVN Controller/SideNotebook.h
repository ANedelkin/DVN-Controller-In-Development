#pragma once
#include "Global.h"
#include "DVNFileData.h"

#include "SideMenuCtrl.h"
#include "SideNotebookPanel.h"

class SideNotebook : public SideNotebookPanel
{
private:
	string folder;
	string extension;

	vector<SideMenuCtrl*> pages;

	wxBoxSizer* mainSizer;
	wxStaticBox* pagesBox;
	wxStaticBoxSizer* pagesBoxSizer;
	wxPanel* pagesList;
	wxBoxSizer* pagesSizer;
	SideMenuCtrl* cur = nullptr;

	SideNotebookPanel* content = nullptr;

	void ChangeSelection(SideMenuCtrl* scenCtrl);

	void OnSelect(wxMouseEvent& e);

	void OnDelete(wxCommandEvent& e);
public:
	SideNotebook(wxWindow* parent, string sideMenuTxt, DVNFileData* data = nullptr);

	void SetContent(SideNotebookPanel* content);

	void AddPage(DVNFileData* data, bool subMenu);

	void ChangePage(DVNFileData* data);

	void ChangeSource(DVNFileData* data) override;
	void Init() override;
	void UnInit() override;
};

