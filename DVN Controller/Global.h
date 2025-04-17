#pragma once

#include "Includes.h"


//Random defines:

#define NAME_MAX_LENGTH	27

#define NAME_INPUT_LEN	200
#define FREQ_INPUT_LEN	100


//Styles:

#define CLOSEABLE       0b00000001
#define DELETABLE       0b00000010
#define LOADABLE        0b00000100
#define READ_ONLY       0b00001000
#define CONTENT         0b00010000

#define DIALOG          0b00000001
#define SAVING_MANY     0b00000010

//Vars:

extern wxWindow* base;
extern wxWindow* focused;


//Funcs:

extern vector<string> Split(const string& str, char delimiter);


//Colours

#define DARK_GREEN wxColour(0, 127, 14)


//Events
wxDECLARE_EVENT(EVT_SCROLL_TO, wxCommandEvent);
wxDECLARE_EVENT(EVT_UNSAVE, wxCommandEvent);