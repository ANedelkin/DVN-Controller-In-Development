//#pragma once
//
//#include <wx/dialog.h>
//#include <wx/panel.h>
//#include <wx/gauge.h>
//#include <wx/stattext.h>
//#include <wx/sizer.h>
//#include "JammersManager.h"
//#include <thread>
//#include "Log.h"
//
//class LoadTransferProgressFrame : public wxDialog
//{
//public:
//    LoadTransferProgressFrame(wxWindow* parent, Load* load, std::string serNum)
//        : wxDialog(parent, wxID_ANY, "Sending load", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxSYSTEM_MENU),
//        load(load),
//        serNum(serNum),
//		curScen(1)
//    {
//		wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
//        wxPanel* panel = new wxPanel(this, wxID_ANY);
//        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
//
//        scenText = new wxStaticText(panel, wxID_ANY, "");
//
//        gauge = new wxGauge(panel, wxID_ANY, SCENARIOS_COUNT, wxDefaultPosition, wxSize(250, 20));
//
//        sizer->Add(scenText, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
//        sizer->Add(gauge, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 10);
//
//        panel->SetSizer(sizer);
//		mainSizer->Add(panel, 1, wxEXPAND);
//		SetSizerAndFit(mainSizer);
//    }
//
//    int ShowModal() override {
//        curScen = 1;
//        gauge->SetValue(1);
//
//        scenText->SetLabel(wxString::Format("Scenario: %d/%d", curScen, SCENARIOS_COUNT));
//
//        thread([=]() {
//            bool result = JammersManager::SendLoad(serNum, load, this);
//            CallAfter([=]() {
//                EndModal(result ? wxID_OK : wxID_CANCEL);
//            });
//        }).detach();
//
//        return wxDialog::ShowModal();
//    }
//
//    void Increment() {
//        CallAfter([=]() {
//            //Log(scenText->GetLabel().ToStdString() + " " + to_string(gauge->GetValue()));
//
//            curScen++;
//
//            gauge->SetValue(curScen);
//            
//            if(curScen <= SCENARIOS_COUNT)
//                scenText->SetLabel(wxString::Format("Scenario: %d/%d", curScen, SCENARIOS_COUNT));
//
//            //Log(scenText->GetLabel().ToStdString() + " " + to_string(gauge->GetValue()));
//        });
//    }
//
//private:
//    char curScen;
//    Load* load;
//    string serNum;
//    wxGauge* gauge;
//    wxStaticText* scenText;
//};
