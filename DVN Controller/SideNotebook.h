#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <filesystem>

#include "Global.h"
#include "DVNFileData.h"

#include "SideMenuCtrl.h"
#include "SideNotebookContent.h"
#include "SaveDialog.h"

using namespace filesystem;

//Styles:

#define CLOSEABLE       0b00000001
#define DELETABLE       0b00000010
#define LOADABLE        0b00000100
#define READ_ONLY       0b00001000
#define CONTENT         0b00010000

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
	virtual StatusCode NewPage(DVNFileData* data);

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
	virtual bool CheckForUnsaved();
};

