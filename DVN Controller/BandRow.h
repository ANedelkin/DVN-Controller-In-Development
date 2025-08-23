#pragma once

#include <wx/wx.h>
#include <wx/window.h>

#include "Global.h"
#include "Scenario.h"
#include "ColourfulButton.h"
#include "LayoutConstants.h"
#include "Colours.h"
#include "Events.h"

#define FREQ_INPUT_LEN 100

class BandRow : public wxPanel
{
private:
    wxWindow* unfocused;
    bool toBeInited = true;
    const bool readOnly;

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
    void OnText(wxCommandEvent& e);
    void OnTextCtrlFocus(wxFocusEvent& e);
    void OnUnfocus(wxFocusEvent& e);
    void OnFocus(wxFocusEvent& e);
    void ScrollTo();
public:
    wxTextCtrl* name;
    wxTextCtrl* startValue;
    wxTextCtrl* endValue;

    enum {
        Name = 1,
        Start,
        End,
        StatBtn,
    };

    BandRow(wxWindow* parent, Scenario* scenario, const char bandNum, const bool readOnly = false);

    void ChangeScenario(Scenario* scenario);

    void Rename();
    void UpdateFreqs(int freqToChange);
    void UpdateFreq(int freqToChange);

    void CheckIfValid(wxTextCtrl* ctrl);

    void Unfocus();
};