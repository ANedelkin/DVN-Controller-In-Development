#include "ScenariosPanel.h"

ScenariosPanel::ScenariosPanel(wxWindow* parent) : wxPanel(parent)
{
	curScen = nullptr;

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	scenList = new wxStaticBox(this, wxID_ANY, "Scenarios");
	scenSizer = new wxStaticBoxSizer(wxVERTICAL, scenList, "Scenarios");
	scenSizer->AddSpacer(38);
	scenList->SetSizerAndFit(scenSizer);
	scenList->SetMinSize(wxSize(210, -1));

	mainSizer->Add(scenList, 0, wxEXPAND);

	bandsBox = new wxStaticBox(this, wxID_ANY, "Bands");
	bandsSizer = new wxStaticBoxSizer(wxVERTICAL, bandsBox, "Bands");
	
	bandsPanel = new BandsPanel(bandsBox, placeHolder);
	bandsSizer->Add(bandsPanel);
	bandsBox->SetSizerAndFit(bandsSizer);

	bandsPanel->Hide();

	mainSizer->Add(bandsBox, 1, wxEXPAND | wxLEFT, 5);

	this->SetSizerAndFit(mainSizer);

	Bind(wxEVT_COMMAND_MENU_SELECTED, &ScenariosPanel::OnScenDelete, this);
}

void ScenariosPanel::AddScenario(Scenario* scen)
{
	SideMenuCtrl* scenCtrl = new SideMenuCtrl(scenList, this, scen);
	scenarios.push_back(scenCtrl);
	scenSizer->Add(scenCtrl, 0, wxEXPAND | wxBOTTOM, 10);
	ChangeSelection(scenCtrl);
	Refresh();
	Layout();
	scenCtrl->Bind(wxEVT_LEFT_UP, &ScenariosPanel::OnScenSelect, this);
}

void ScenariosPanel::ChangeSelection(SideMenuCtrl* scenCtrl)
{
	scenCtrl->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	if (curScen) curScen->SetBackgroundColour(wxColour(255, 255, 255));
	curScen = scenCtrl;

	bandsPanel->ChangeScenario(dynamic_cast<Scenario*>(curScen->data));
	if (!bandsPanel->IsShown()) bandsPanel->Show();
}

void ScenariosPanel::OnScenSelect(wxMouseEvent& e)
{
	SideMenuCtrl* scenCtrl = dynamic_cast<SideMenuCtrl*>(e.GetEventObject());
	if (scenCtrl != curScen) ChangeSelection(scenCtrl);
	e.Skip();
}

void ScenariosPanel::OnScenDelete(wxCommandEvent& e)
{
	wxMessageDialog dialog(base, "If you delete the scenario you won't be able to get it back!", "Are you sure about that?", wxYES_NO | wxICON_EXCLAMATION);
	if (dialog.ShowModal() == wxID_YES) {
		int i = find(scenarios.begin(), scenarios.end(), e.GetEventObject()) - scenarios.begin();
		scenarios.erase(scenarios.begin() + i);
		if (curScen == e.GetEventObject()) {
			if (scenarios.size() > 0) {
				if (i < scenarios.size()) ChangeSelection(scenarios[i]);
				else ChangeSelection(scenarios[i - 1]);
			}
			else {
				bandsPanel->Hide();
				curScen = nullptr;
			}
		}
		scenSizer->Remove(1 + i);
		Layout();
		dynamic_cast<wxWindowBase*>(e.GetEventObject())->Destroy();
	}
}
