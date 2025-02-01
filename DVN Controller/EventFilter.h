#pragma once
#include "MainFrame.h"

class EventFilter : public wxEventFilter {
public:
	EventFilter() {
		wxEvtHandler::AddFilter(this);
	}
	virtual ~EventFilter()
	{
		wxEvtHandler::RemoveFilter(this);
	}
	virtual int FilterEvent(wxEvent& e) {
		const wxEventType t = e.GetEventType();
		if (t == wxMOUSE_BTN_ANY) {
			wxTextCtrl* ctrl = dynamic_cast<wxTextCtrl*>(base->FindFocus());
			switch ((int)(ctrl->GetClientData()))
			{
			case BandName:
				if (!ctrl->GetRect().Contains(wxGetMousePosition())) {

					BandRow* row = dynamic_cast<BandRow*>(ctrl->GetParent());
					row->Rename();
				}
				break;
			default:
				break;
			}
		}
	}
};