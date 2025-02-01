#pragma once
#include "Global.h"
#include "BandRow.h"

class BandsPanel : public wxPanel
{
private:
	Scenario* scenario;
public:
	wxScrolled<wxPanel>* table;
	wxBoxSizer* tableSizer;

	BandsPanel(wxWindow* parent, Scenario* scenario);
};