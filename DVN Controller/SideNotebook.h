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
	void OnUnsave(wxCommandEvent& e);

	void Remove(SideMenuCtrl* win);
	void RemoveAll();
public:
	SideNotebook(wxWindow* parent, wxPanel* mainPanel, string sideMenuTxt, DVNFileData* data = nullptr);

	void SetContent(SideNotebookPanel* content);

	void AddPage(DVNFileData* data, bool subMenu);

	void ChangePage(DVNFileData* data);

	void ChangeSource(DVNFileData* source) override;
	void SaveCurrent();
	void Init() override;
	void UnInit() override;
};

