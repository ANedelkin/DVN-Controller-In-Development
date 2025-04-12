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

	void Rename(SideMenuCtrl* page, const string& name, bool renameFile = true);
	void Close(SideMenuCtrl* page);
	bool Save(SideMenuCtrl* page, bool saveAs);

	void OnSelect(wxCommandEvent& e);
public:
	SideNotebook(wxWindow* parent, string sideMenuTxt, string(*pageNameValidator)(const string& name));
	
	void Select(char i);

	void OnUnsave(wxCommandEvent& e);
	void Unsave(bool created, SideMenuCtrl* target = nullptr);

	StatusCode NewPage(DVNFileData* data);
	StatusCode AddPage(SideMenuCtrl* page);
	vector<SideMenuCtrl*>& GetPages();

	void SaveCurrent(bool saveAs);
	SideMenuCtrl* GetCurrent();
	bool CheckForUnsaved();
};

