#pragma once

#include "Global.h"
#include "Scenario.h"
#include "BandPreset.h"
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

    Status TryParse(const wxString& str, int* result);

    void OnResize(wxSizeEvent& e);

    void EmptyHandler(wxEvent& e);
    void OnNameEnter(wxKeyEvent& e);
    void OnStartEnter(wxKeyEvent& e);
    void OnEndEnter(wxKeyEvent& e);
    void OnStatusChanged(wxCommandEvent& e);
    void OnFocus(wxFocusEvent& e);
public:
    wxTextCtrl* name;
    wxTextCtrl* startValue;
    wxTextCtrl* endValue;
    BandRow(wxWindow* parent, Scenario* scenario, const char bandNum, const  char style = 0);

    void ChangeScenario(Scenario* scenario);

    Status Rename();
    //Status ChangeStart();
    //Status ChangeEnd();
    Status UpdateFreq(wxTextCtrl* ctrl);

    void Unfocus();
};