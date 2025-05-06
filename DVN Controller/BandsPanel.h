#pragma once
#include "Global.h"
#include "LayoutConstants.h"
#include "SideNotebookContent.h"
#include "BandRow.h"
#include "Events.h"

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

	BandsPanel(wxWindow* parent, Scenario* scenario, const bool readOnly = false);

	void SetSource(DVNFileData* source) override;
	void Init() override;
	void UnInit() override;
};