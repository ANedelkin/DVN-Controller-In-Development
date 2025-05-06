#pragma once

#include "Includes.h"


//Random defines:

#define NAME_INPUT_LEN	200 //For LayoutConstants.h

//Vars:

extern wxWindow* base;
extern wxWindow* focused;


//Colours

#define DARK_GREEN wxColour(0, 127, 14) //For Colours.h


//Events
wxDECLARE_EVENT(EVT_UNSAVE, wxCommandEvent); //For Events.h
wxDECLARE_EVENT(EVT_SCROLL_TO, wxCommandEvent);