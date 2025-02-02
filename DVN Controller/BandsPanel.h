#pragma once
#include "Global.h"
#include "BandRow.h"

class BandsPanel : public wxPanel
{
private:
	Scenario* scenario;

	array<BandRow*, BANDS_COUNT> bandRows;
public:
	wxPanel* content;
	wxBoxSizer* tableSizer;

	BandsPanel(wxWindow* parent, Scenario* scenario);

	void ChangeScenario(Scenario* scenario);
};