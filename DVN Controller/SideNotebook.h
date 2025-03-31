#pragma once
#include "Global.h"
#include "DVNFileData.h"

#include "SideMenuCtrl.h"
#include "SideNotebookPanel.h"
#include "SaveDialog.h"

class ScenSelectDialog;

class SideNotebook : public SideNotebookPanel
{
protected:
	wxMenu* contextMenu;
	vector<SideMenuCtrl*> pages;

	wxBoxSizer* mainSizer;

	wxScrolledWindow* scrollWrapper;
	wxStaticBox* pagesBox;
	wxStaticBoxSizer* pagesBoxSizer;
	wxPanel* pagesList;
	wxBoxSizer* pagesSizer;
	SideMenuCtrl* cur = nullptr;

	SideNotebookPanel* content = nullptr;

	void SetContent(SideNotebookPanel* content);
	void ChangeSelection(SideMenuCtrl* scenCtrl);

	void OnSelect(wxCommandEvent& e);

	void Close(SideMenuCtrl* win);
	bool Save(SideMenuCtrl* page, bool saveAs);
public:
	void Select(char i);

	void OnUnsave(wxCommandEvent& e);
	void Unsave(bool created, SideMenuCtrl* target = nullptr);

	SideNotebook(wxWindow* parent, string sideMenuTxt, DVNFileData* data = nullptr);


	Status NewPage(DVNFileData* data);
	Status AddPage(SideMenuCtrl* page);
	vector<SideMenuCtrl*> GetPages();

	void SetSource(DVNFileData* source) override;
	void SaveCurrent(bool saveAs);
	SideMenuCtrl* GetCurrent();
	bool CheckForUnsaved();
	void Init() override;
	void UnInit() override;
};

