#pragma once
#include "Global.h"
#include "DVNFileData.h"

#include "SideMenuCtrl.h"
#include "SideNotebookContent.h"
#include "SaveDialog.h"

class ScenSelectDialog;

class SideNotebook : public wxPanel
{
protected:
	wxMenu* contextMenu;
	vector<SideMenuCtrl*> pages;
	string(*pageNameValidator)(const string& name);

	wxBoxSizer* mainSizer;

	wxScrolledWindow* scrollWrapper;
	wxStaticBox* pagesBox;
	wxStaticBoxSizer* pagesBoxSizer;
	wxPanel* pagesList;
	wxBoxSizer* pagesSizer;
	SideMenuCtrl* cur = nullptr;

	SideNotebookContent* content = nullptr;

	void UpdateContent();
	void SetContent(SideNotebookContent* content);
	void ChangeSelection(SideMenuCtrl* scenCtrl);
	StatusCode NewPage(DVNFileData* data);

	void Duplicate(SideMenuCtrl* page);
	bool Rename(SideMenuCtrl* page, bool renameFile = true);
	void Close(SideMenuCtrl* page);
	virtual bool Save(SideMenuCtrl* page);

	void OnSelect(wxCommandEvent& e);
public:
	SideNotebook(wxWindow* parent, string sideMenuTxt, string(*pageNameValidator)(const string& name));
	
	void Select(char i);

	void OnUnsave(wxCommandEvent& e);
	void Unsave(bool created, SideMenuCtrl* target = nullptr);

	vector<SideMenuCtrl*>& GetPages();

	void SaveCurrent();
	SideMenuCtrl* GetCurrent();
	bool CheckForUnsaved();
};

