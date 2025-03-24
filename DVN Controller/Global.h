#pragma once

#include "Includes.h"

//Defines:

#define CLOSEABLE   0b0001
#define DELETABLE   0b0010
#define LOADABLE    0b0100
#define READ_ONLY   0b1000


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
    InvalidName,
    InvalidSymbols,
    NameWhitespace,
    NameAlreadyExists,
    JammerNotSelected,
    FileNonexistent,
    FileAlreadyOpen,
    FreqNotPositiveNumber,
};

extern map<Status, const char*> errorMessages;


//Funcs:

extern vector<string> Split(const string& str, char delimiter);
extern Status ValidateName(string& name);
extern int ErrorMessage(wxWindow* parent, Status stat, const char* param = "");


//Tabs

enum Tabs {
    Scenarios,
    Loads,
};


//BandRow column tags

enum InputType {
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