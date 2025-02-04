#pragma once

#include "Global.h"

wxWindow* base = nullptr;
wxWindow* focused = nullptr;
char ctrlHeight = 0;

map<int, string> errorMessages = {
    {StartValueOutOfBounds , "Start value outside bounds!"},
    {EndValueOutOfBounds, "End value outside bounds!"},
    {StartValueHigherThanEndvalue, "Start value higher than end value!"},
    {EndValueLowerThanStartValue, "End value lower than start value!"},
    {BandAtLastPlace, "The range between the start and end values of the last band of this range cannot include the value {}!"},
    {InvalidName, "The name cannot contain the symbol \"|\"!"},
    {InvalidSymbols, "The name cannot contain the symbol \"{}\"!"},
    {NameWhitespace, "The name cannot be empty!"},
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
