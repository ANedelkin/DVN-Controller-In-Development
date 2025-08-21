#pragma once
#include <wx/statusbr.h>
#include <wx/bmpbndl.h>
#include <wx/frame.h>
#include <wx/statbmp.h>

#include <string>

#include "Icons.h"

class StatusBar
{
private:
	wxBitmapBundle OKBitmap = wxBitmapBundle::FromSVG(OKSVG, wxSize(16, 16));
	wxBitmapBundle ErrorBitmap = wxBitmapBundle::FromSVG(ErrorSVG, wxSize(16, 16));
	
	wxStatusBar* statusBar = nullptr;
	wxStaticBitmap* icon = nullptr;
public:
	StatusBar() {};
	StatusBar(wxFrame* parent);

	void SetStatus(const std::string& status);
};

