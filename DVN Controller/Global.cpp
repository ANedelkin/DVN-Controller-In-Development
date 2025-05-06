#pragma once

#include "Global.h"

wxWindow* base = nullptr;
wxWindow* focused = nullptr;

wxDEFINE_EVENT(EVT_UNSAVE, wxCommandEvent);
wxDEFINE_EVENT(EVT_SCROLL_TO, wxCommandEvent);
