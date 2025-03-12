#pragma once

#include "Global.h"
#include "Scenario.h"
#include "BandPreset.h"

class BandRow : public wxPanel
{
private:
    wxWindow* unfocused;
    bool toBeInited = true;

    Scenario* scenario;
    char bandNum;

    wxButton* background;

    wxButton* statBtn;

    void InitForeground();
    void BindEventHandlers();
    void SetUpSizers();

    void MarkUnsaved();

    void OnResize(wxSizeEvent& e);

    void OnNameEnter(wxKeyEvent& e);
    void OnStartEnter(wxKeyEvent& e);
    void OnEndEnter(wxKeyEvent& e);
    void OnStatusChanged(wxMouseEvent& e);
    void OnFocus(wxFocusEvent& e);
public:
    wxStaticText* num;
    wxTextCtrl* name;
    wxTextCtrl* startValue;
    wxTextCtrl* endValue;
    BandRow(wxWindow* parent, Scenario* scenario, char bandNum);

    void ChangeScenario(Scenario* scenario);

    Status Rename();
    Status ChangeStart();
    Status ChangeEnd();

    void Unfocus();
};