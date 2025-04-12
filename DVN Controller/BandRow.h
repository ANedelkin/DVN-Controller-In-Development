#pragma once

#include "Global.h"
#include "Scenario.h"
#include "ColourfulButton.h"

class BandRow : public wxPanel
{
private:
    wxWindow* unfocused;
    bool toBeInited = true;
    const char style;

    Scenario* scenario;
    char bandNum;

    wxButton* background;

    wxStaticText* num;
    ColourfulBtn* statBtn;

    void InitForeground();
    void BindEventHandlers();
    void SetUpSizers();

    void MarkUnsaved();

    bool ProcessKey(int key);

    void OnResize(wxSizeEvent& e);

    void EmptyHandler(wxEvent& e);
    void OnKey(wxKeyEvent& e);
    void OnStatusChanged(wxCommandEvent& e);
    void OnFocus(wxFocusEvent& e);
public:
    wxTextCtrl* name;
    wxTextCtrl* startValue;
    wxTextCtrl* endValue;
    BandRow(wxWindow* parent, Scenario* scenario, const char bandNum, const  char style = 0);

    void ChangeScenario(Scenario* scenario);

    string Rename();
    string UpdateFreq(wxTextCtrl* ctrl);

    void Unfocus();
};