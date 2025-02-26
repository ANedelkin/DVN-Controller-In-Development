#pragma once
#include "Global.h"

#include "SideNotebookPanel.h"
#include "BandRow.h"

class BandsPanel : public SideNotebookPanel
{
private:
	array<BandRow*, BANDS_COUNT> bandRows;
public:
	wxPanel* table;
	wxPanel* content;
	wxBoxSizer* tableSizer;

	BandsPanel(wxWindow* parent, wxPanel* mainPanel, Scenario* scenario);

	void ChangeSource(DVNFileData* source) override;
	void Init() override;
	void UnInit() override;
};