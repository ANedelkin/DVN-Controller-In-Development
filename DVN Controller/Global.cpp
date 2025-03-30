#pragma once

#include "Global.h"

wxWindow* base = nullptr;
wxWindow* focused = nullptr;
char ctrlHeight = 0;

map<Status, const char*> errorMessages = {
    {StartValueOutOfBounds , "This frequency can't be below %d!"},
    {EndValueOutOfBounds, "This frequency can't be over %d!"},
    {StartValueHigherThanEndvalue, "Start value higher than end value!"},
    {EndValueLowerThanStartValue, "End value lower than start value!"},
    {BandAtLastPlace, "The range between the start and end values of the last band of this range cannot include the value %d!"},
    {InvalidSymbols, "The name contains invalid symbol/s!"},
    {NameWhitespace, "The name cannot be empty!"},
    {BandUninitialized, "Start and end frequencies have to be set in order to turn band on!"},
    {JammerNotSelected, "You haven't selected a jammer!"},
    {FileNonexistent, "The file \"%s\" does not exist!"},
    {FileAlreadyOpen, "The file \"%s\" is already open as \"%s\"!"},
    {ScenarioAlreadyExists, "A scenario with the name \"%s\" already exists!"},
    {FreqNotPositiveNumber, "The frequency has to be a positive whole number!"},
    {NameTooLong, "The name can't be longer than %d symbols!"},
    {ErrorMessageTooLong, "The error message the program tried to generate was too long!"},
};

vector<string> Split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter)) { tokens.push_back(token); }

    return tokens;
}

Status ValidateName(string& name) { //Unused
    if (name.find('|') != string::npos) return InvalidSymbols;
    if (name.length() > NAME_MAX_LENGTH) return NameTooLong;
    return Success;
}

int ErrorMessage(wxWindow* parent, Status stat, const char style, ...)
{
    char buffer[256];
    string msg;

    va_list args;
    va_start(args, style);

    if (vsnprintf(buffer, sizeof(buffer), errorMessages[stat], args) > sizeof(buffer) - 1)
        return ErrorMessage(parent, ErrorMessageTooLong);

    msg = buffer;

    if (style & DIALOG) {
        wxMessageDialog frame(parent, msg, "Error", wxOK | wxCANCEL | wxICON_ERROR);
        frame.SetOKCancelLabels("Enter new value", "Keep old value");
        return frame.ShowModal();
    }

    return wxMessageDialog(parent, msg, "Error", wxOK | wxICON_ERROR).ShowModal();
}

wxDEFINE_EVENT(EVT_LOAD, wxCommandEvent);
wxDEFINE_EVENT(EVT_CLOSE_PAGE, wxCommandEvent);
wxDEFINE_EVENT(EVT_DELETE, wxCommandEvent);
wxDEFINE_EVENT(EVT_UNSAVE, wxCommandEvent);
