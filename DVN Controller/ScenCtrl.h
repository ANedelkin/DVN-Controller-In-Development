#pragma once
#include "Global.h"
#include "Scenario.h"

class ScenCtrl : public wxButton {
public:
	Scenario* scenario;
	ScenCtrl(wxWindow* parent, Scenario* scenario) : wxButton(parent, wxID_ANY, scenario->GetName(), wxDefaultPosition, wxSize(-1, 43)) {
		this->scenario = scenario;
		SetBackgroundColour(wxColour(255, 255, 255));
	}
};