#pragma once

#include "Global.h"

wxWindow* base = nullptr;
wxWindow* focused = nullptr;
char ctrlHeight = 0;

map<Status, const char*> errorMessages = {
    {StartValueOutOfBounds , "Start value outside bounds!"},
    {EndValueOutOfBounds, "End value outside bounds!"},
    {StartValueHigherThanEndvalue, "Start value higher than end value!"},
    {EndValueLowerThanStartValue, "End value lower than start value!"},
    {BandAtLastPlace, "The range between the start and end values of the last band of this range cannot include the value %s!"},
    {InvalidName, "The name cannot contain the symbol \"|\"!"},
    {InvalidSymbols, "The name cannot contain the symbol \"%s\"!"},
    {NameWhitespace, "The name cannot be empty!"},
    {BandUninitialized, "Start and end frequencies have to be set in order to turn band on!"},
    {NameAlreadyExists, "A scenario with this name already exists!"},
    {JammerNotSelected, "You haven't selected a jammer!"},
    {FileNonexistent, "The file \"%s\" does not exist!"},
    {FileAlreadyOpen, "The file \"%s\" is already open!"}
};

vector<string> Split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter)) { tokens.push_back(token); }

    return tokens;
}

Status ValidateName(string& name) {
    if (name.find('|') != string::npos) return InvalidName;
    return Success;
}

int ErrorMessage(wxWindow* parent, Status stat, const char* param)
{
    char buffer[256];
    string msg;
    if (param != "")  {
        sprintf(buffer, errorMessages[stat], param);
        msg = buffer;
    }
    else msg = errorMessages[stat];
    return wxMessageDialog(parent, msg, "Error", wxOK | wxICON_ERROR).ShowModal();
}

wxDEFINE_EVENT(EVT_CLOSE_PAGE, wxCommandEvent);
wxDEFINE_EVENT(EVT_DELETE, wxCommandEvent);
wxDEFINE_EVENT(EVT_UNSAVE, wxCommandEvent);
