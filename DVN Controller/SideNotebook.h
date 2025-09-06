#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <filesystem>

#include "Global.h"
#include "DVNFileData.h"

#include "SideMenuCtrl.h"
#include "SideNotebookContent.h"
#include "SaveDialog.h"
#include "ScrolledPanel.h"

using namespace filesystem;

//Flags:

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

	ScrolledPanel* scrollWrapper;
	wxStaticBox* pagesBox;
	wxStaticBoxSizer* pagesBoxSizer;
	wxPanel* pagesList;
	wxBoxSizer* pagesSizer;
	SideMenuCtrl* cur = nullptr;

	SideNotebookContent* content = nullptr;

	void UpdateContent();
	void SetContent(SideNotebookContent* content);
	virtual void ChangeSelection(SideMenuCtrl* scenCtrl);
	virtual StatusCode NewPage(DVNFileData* data);

	bool Duplicate(SideMenuCtrl* page);
	bool Rename(SideMenuCtrl* page, bool renameFile = true);
	virtual bool Save(SideMenuCtrl* page);

	virtual void OnStatusUpdate(wxCommandEvent& e);
	void OnSelect(wxCommandEvent& e);
	void OnPagesBoxTabbed(wxKeyEvent& e);
	void OnPagesBoxFocused(wxFocusEvent& e);
public:
	SideNotebook(wxWindow* parent, string sideMenuTxt, string(*pageNameValidator)(const string& name));

	void Select(char i);
	void Close(SideMenuCtrl* page);

	void OnUnsave(wxCommandEvent& e);
	void Unsave(bool created, SideMenuCtrl* target = nullptr);

	vector<SideMenuCtrl*>& GetPages();

	void SaveCurrent();
	SideMenuCtrl* GetCurrent();
	virtual bool CheckForUnsaved();
};

