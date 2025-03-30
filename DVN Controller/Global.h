#pragma once

#include "Includes.h"

//Styles:

#define CLOSEABLE   0b00000001
#define DELETABLE   0b00000010
#define LOADABLE    0b00000100
#define READ_ONLY   0b00001000
#define DIALOG      0b00010000


//Vars:

extern wxWindow* base;
extern wxWindow* focused;
extern char ctrlHeight;


//Errors:

enum Status {
    Success,
    BandUninitialized,
    StartValueOutOfBounds,
    EndValueOutOfBounds,
    StartValueHigherThanEndvalue,
    EndValueLowerThanStartValue,
    BandAtLastPlace,
    InvalidSymbols,
    NameWhitespace,
    JammerNotSelected,
    FileNonexistent,
    FileAlreadyOpen,
    ScenarioAlreadyExists,
    FreqNotPositiveNumber,
    NameTooLong,
    ErrorMessageTooLong,
};

extern map<Status, const char*> errorMessages;


//Funcs:

extern vector<string> Split(const string& str, char delimiter);
extern Status ValidateName(string& name);
extern int ErrorMessage(wxWindow* parent, Status stat, const char style = 0, ...);


//Tabs

enum Tabs { //Move to MainFrame
    Loads,
    Scenarios,
};


//BandRow column tags

enum InputType { //Change to defines
    ScenName,
    BandName,
    Start,
    End,
};


//Colours

#define DARK_GREEN wxColour(0, 127, 14)


//Events
wxDECLARE_EVENT(EVT_LOAD, wxCommandEvent);
wxDECLARE_EVENT(EVT_CLOSE_PAGE, wxCommandEvent);
wxDECLARE_EVENT(EVT_DELETE, wxCommandEvent);
wxDECLARE_EVENT(EVT_UNSAVE, wxCommandEvent);