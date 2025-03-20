#include "JammersWindow.h"


JammersWindow::JammersWindow(wxWindow* parent) : wxDialog(parent, wxID_ANY, "Select jammer")
{
	wxPanel* mainPanel = new wxPanel(this);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	list = new wxListBox(mainPanel, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(150, 150)));
	select = new wxButton(mainPanel, wxID_OK);
	wxButton* refresh = new wxButton(mainPanel, wxID_ANY, "Refresh");
	wxButton* cancel = new wxButton(mainPanel, wxID_CANCEL);

	mainSizer->Add(list, 0, wxALL, FromDIP(10));
	mainSizer->Add(select, 0, wxEXPAND | wxLEFT | wxRIGHT, FromDIP(10));
	mainSizer->Add(refresh, 0, wxEXPAND | wxALL, FromDIP(10));
	mainSizer->Add(cancel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));

	Bind(wxEVT_KEY_DOWN, &JammersWindow::OnEnterPressed, this);  
	select->Bind(wxEVT_BUTTON, &JammersWindow::OnSelectClicked, this);
	refresh->Bind(wxEVT_BUTTON, &JammersWindow::OnRefreshClicked, this);

	select->SetDefault();

	mainPanel->SetSizerAndFit(mainSizer);
	Fit();

	LoadJammers();
}

char* JammersWindow::GetSerNum()
{
	return serNum;
}



void JammersWindow::OnEnterPressed(wxKeyEvent& e)
{
	if(e.GetKeyCode() == WXK_RETURN) Select();
	e.Skip();
}

void JammersWindow::OnSelectClicked(wxCommandEvent& e)
{
	Select();
}

void JammersWindow::OnRefreshClicked(wxCommandEvent& e)
{
	LoadJammers();
}

void JammersWindow::LoadJammers()
{
	list->Clear();
	DWORD devsCount;
	FT_CreateDeviceInfoList(&devsCount);
	_ft_device_list_info_node* devices = new _ft_device_list_info_node[devsCount];
	FT_GetDeviceInfoList(devices, &devsCount);
	for (DWORD i = 0; i < devsCount; i++) {
		if (devices[i].Type != FT_DEVICE_UNKNOWN && strcmp(devices[i].Description, "BJ130P") == 0) {
			char str[16] = "BJ130P ";
			strcat(str, devices[i].SerialNumber);
			this->SetFocus();
			list->AppendString(str);
		}
	}
	for (char i = 0; i < 3; i++) list->AppendString(to_string(i) + "xxxxxxx");
	if (list->GetCount() == 0) select->Disable();
	else {
		if (list->GetCount() == 1) list->SetSelection(0);
		select->Enable();
	}
}

void JammersWindow::Select()
{
	int selection = list->GetSelection();
	if (selection == wxNOT_FOUND) ErrorMessage(this, JammerNotSelected);
	else {
		wxString jammerName = list->GetString(selection);
		int len = jammerName.Length();
		const wxString temp = jammerName.SubString(len - 8, len);
		strcpy(serNum, temp);
		Close();
	}
}
