#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <array>

#include "SideNotebookContent.h"
#include "BandRow.h"
#include "ScrolledPanel.h"

class BandsPanel : public SideNotebookContent
{
private:
	wxWindow* bandsListEnd;

	array<BandRow*, GetBandsCount()> bandRows;
	ScrolledPanel* scrollWrapper;

	void OnScrollTo(wxCommandEvent& e);
	void OnBandListEndReached(wxFocusEvent& e);
public:
	wxPanel* table;
	wxPanel* content;
	wxBoxSizer* tableSizer;

	BandsPanel(wxWindow* parent, Scenario* scenario, const bool readOnly = false);

	void SetSource(DVNFileData* source) override;
	void Init() override;
	void UnInit() override;
};