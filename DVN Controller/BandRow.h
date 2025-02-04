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

    wxButton* onOffBtn;

    void InitForeground();
    void BindEventHandlers();
    void SetUpSizers();

    void OnResize(wxSizeEvent& e);

    void OnNameEnter(wxCommandEvent& e);
    void OnFreqEnter(wxCommandEvent& e);
    void OnFocus(wxFocusEvent& e);
public:
    wxStaticText* num;
    wxTextCtrl* name;
    wxTextCtrl* startValue;
    wxTextCtrl* endValue;
    BandRow(wxWindow* parent, Scenario* scenario, char bandNum);

    void ChangeScenario(Scenario* scenario);

    Status Rename();
    Status ChangeFreqs();
};