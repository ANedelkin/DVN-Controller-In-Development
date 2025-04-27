#pragma once
#include "Global.h"

#include "SideNotebookContent.h"
#include "BandRow.h"

class BandsPanel : public SideNotebookContent
{
private:
	array<BandRow*, GetBandsCount()> bandRows;
	wxScrolled<wxPanel>* scrollWrapper;

	void OnScrollTo(wxCommandEvent& e);
public:
	wxPanel* table;
	wxPanel* content;
	wxBoxSizer* tableSizer;

	BandsPanel(wxWindow* parent, Scenario* scenario, const char style = 0);

	void SetSource(DVNFileData* source) override;
	void Init() override;
	void UnInit() override;
};