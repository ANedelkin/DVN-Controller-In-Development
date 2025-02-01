#pragma once

#include "Includes.h"

//Vars:

extern wxWindow* base;
extern wxWindow* focused;
extern char ctrlHeight;


//Errors:

enum Status {
    Success,
    StartValueOutOfBounds,
    EndValueOutOfBounds,
    StartValueHigherThanEndvalue,
    EndValueLowerThanStartValue,
    BandAtLastPlace,
    InvalidName,
    InvalidSymbols,
    NameWhitespace,
};

extern map<int, string> errorMessages;


//Funcs:

extern vector<string> Split(const string& str, char delimiter);
extern Status ValidateName(string& name);

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
