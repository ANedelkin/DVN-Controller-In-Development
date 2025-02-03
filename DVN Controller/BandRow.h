#pragma once

#include "Global.h"
#include "Scenario.h"
#include "BandPreset.h"

class BandRow : public wxWindow
{
private:
    bool toBeInited = true;

    Scenario* scenario;
    char bandNum;

    wxButton* background;

    void InitForeground();

    void OnResize(wxSizeEvent& e);

    void OnNameEnter(wxCommandEvent& e);
    void OnStartEnter(wxCommandEvent& e);
    void OnEndEnter(wxCommandEvent& e);
    void OnFocus(wxFocusEvent& e);
public:
    wxStaticText* num;
    wxTextCtrl* name;
    wxTextCtrl* startValue;
    wxTextCtrl* endValue;
    BandRow(wxWindow* parent, Scenario* scenario, char bandNum);

    void ChangeScenario(Scenario* scenario);

    Status Rename();
    Status ChangeStartValue();
};