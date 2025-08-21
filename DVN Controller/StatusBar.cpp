#include "StatusBar.h"

StatusBar::StatusBar(wxFrame* parent)
{
	statusBar = parent->CreateStatusBar(2);

	int height = statusBar->GetClientSize().GetHeight();
	int widths[] = { height - statusBar->FromDIP(9), -1 };
	statusBar->SetFieldsCount(2, widths);

	icon = new wxStaticBitmap(statusBar, wxID_ANY, wxBitmapBundle::FromSVG(OKSVG, wxSize(16, 16)), statusBar->FromDIP(wxPoint(2, 1)), wxSize(height - statusBar->FromDIP(3), height));

	SetStatus("");
}

void StatusBar::SetStatus(const std::string& status)
{
	if (status.empty()) {
		icon->SetBitmap(OKBitmap);
		statusBar->SetStatusText("Everything is OK.", 1);
	}
	else {
		icon->SetBitmap(ErrorBitmap);
		statusBar->SetStatusText(status, 1);
	}
}
