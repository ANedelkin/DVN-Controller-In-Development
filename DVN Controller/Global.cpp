#pragma once

#include "Global.h"

wxWindow* base = nullptr;
wxWindow* focused = nullptr;

vector<string> Split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter)) { tokens.push_back(token); }

    return tokens;
}

wxDEFINE_EVENT(EVT_UNSAVE, wxCommandEvent);
