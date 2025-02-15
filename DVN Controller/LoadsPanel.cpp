#include "LoadsPanel.h"

//LoadsPanel::LoadsPanel(wxWindow* parent)
//{
//	curLoad = nullptr;
//
//	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
//
//	loadsList = new wxStaticBox(this, wxID_ANY, "Loads");
//	loadsSizer = new wxStaticBoxSizer(wxVERTICAL, loadsList, "Loads");
//	loadsSizer->AddSpacer(38);
//	loadsList->SetSizerAndFit(loadsSizer);
//	loadsList->SetMinSize(wxSize(210, -1));
//
//	mainSizer->Add(loadsList, 0, wxEXPAND);
//
//	//wxPanel* scenBox = new wxPanel(this);
//	//
//	//scenPanel = new ScenariosPanel(scenBox, placeHolder);
//	//
//	//mainSizer->Add(scenBox, 1, wxEXPAND | wxLEFT, 5);
//
//	this->SetSizerAndFit(mainSizer);
//
//	Bind(wxEVT_COMMAND_MENU_SELECTED, &LoadsPanel::OnLoadDelete, this);
//}
//
//void LoadsPanel::AddLoad(Load* load)
//{
//	SideMenuCtrl* loadCtrl = new SideMenuCtrl(loadsList, this, load);
//	loads.push_back(loadCtrl);
//	loadsSizer->Add(loadCtrl, 0, wxEXPAND | wxBOTTOM, 10);
//	ChangeSelection(loadCtrl);
//	Refresh();
//	Layout();
//	loadCtrl->Bind(wxEVT_LEFT_UP, &LoadsPanel::OnLoadSelect, this);
//}
//
//
//void LoadsPanel::ChangeSelection(SideMenuCtrl* loadCtrl)
//{
//	loadCtrl->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
//	if (curLoad) curLoad->SetBackgroundColour(wxColour(255, 255, 255));
//	curLoad = loadCtrl;
//
//	scenPanel->ChangeLoad(dynamic_cast<Load*>(curLoad->source));
//	if (!scenPanel->IsShown()) scenPanel->Show();
//}
//
//void LoadsPanel::OnLoadSelect(wxMouseEvent& e)
//{
//	SideMenuCtrl* loadCtrl = dynamic_cast<SideMenuCtrl*>(e.GetEventObject());
//	if (loadCtrl != curLoad) ChangeSelection(loadCtrl);
//	e.Skip();
//}
//
//void LoadsPanel::OnLoadDelete(wxCommandEvent& e)
//{
//	wxMessageDialog dialog(base, "If you delete the scenario you won't be able to get it back!", "Are you sure about that?", wxYES_NO | wxICON_EXCLAMATION);
//	if (dialog.ShowModal() == wxID_YES) {
//		int i = find(loads.begin(), loads.end(), e.GetEventObject()) - loads.begin();
//		loads.erase(loads.begin() + i);
//		if (curLoad == e.GetEventObject()) {
//			if (loads.size() > 0) {
//				if (i < loads.size()) ChangeSelection(loads[i]);
//				else ChangeSelection(loads[i - 1]);
//			}
//			else {
//				scenPanel->Hide();
//				curLoad = nullptr;
//			}
//		}
//		loadsSizer->Remove(1 + i);
//		Layout();
//		dynamic_cast<wxWindowBase*>(e.GetEventObject())->Destroy();
//	}
//}