#pragma once
#include "Global.h"
#include "DVNFileData.h"

#include "SideMenuCtrl.h"
#include "SideNotebookPanel.h"

#include "SaveDialog.h"

class SideNotebook : public SideNotebookPanel
{
private:
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

	void Remove(SideMenuCtrl* win);

	bool Save(SideMenuCtrl* page, bool saveAs);
public:
	void Select(char i);

	void OnUnsave(wxCommandEvent& e);
	void Unsave(bool created);

	SideNotebook(wxWindow* parent, string sideMenuTxt, DVNFileData* data = nullptr);

	void SetContent(SideNotebookPanel* content);

	Status AddPage(DVNFileData* data, bool subMenu);

	void ChangePage(DVNFileData* data);

	void ChangeSource(DVNFileData* source) override;
	void SaveCurrent(bool saveAs);
	SideMenuCtrl* GetCurrent();
	bool CheckForUnsaved();
	void Init() override;
	void UnInit() override;
};

