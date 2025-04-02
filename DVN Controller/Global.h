#pragma once

#include "Includes.h"


//Random defines:

#define NAME_MAX_LENGTH 27

#define NAME_INPUT_LEN 200
#define FREQ_INPUT_LEN 100


//Styles:

#define CLOSEABLE       0b00000001
#define DELETABLE       0b00000010
#define LOADABLE        0b00000100
#define READ_ONLY       0b00001000

#define DIALOG          0b00010000
#define SAVING_MANY     0b00100000

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
extern int ErrorMessage(wxWindow* parent, Status stat, const char style = 0, ...);
extern bool HasNonStdChars(const string& str);
extern Status ValidateNameBasic(const string& name);


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