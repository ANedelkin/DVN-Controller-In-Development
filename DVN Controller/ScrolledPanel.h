#pragma once

#include <wx/window.h>
#include <wx/scrolwin.h>
#include <wx/event.h>

#include "Events.h"

class ScrolledPanel : public wxScrolledWindow {
private:
	void OnKey(wxKeyEvent& e) {
        if (e.GetKeyCode() == WXK_PAGEUP)
            ScrollLines(FromDIP(-5));
        else if (e.GetKeyCode() == WXK_PAGEDOWN)
            ScrollLines(FromDIP(5));
        else e.Skip();
	}
	void OnScrollTo(wxCommandEvent& e) {
        wxWindow* target = (wxWindow*)e.GetEventObject();
        wxPoint pos = target->GetPosition();
        int viewStart = GetViewStart().y;
        int x, pxPerUnit;
        GetScrollPixelsPerUnit(&x, &pxPerUnit);
        int y = pos.y / pxPerUnit;

        if (y < viewStart)
            Scroll(0, y);
        else {
            int targetSize = target->GetSize().y / pxPerUnit;
            wxSize swSize = GetClientSize();
            int curEnd = viewStart + swSize.y / pxPerUnit - targetSize;
            if (y > curEnd) {
                int x, scrollRate;
                GetScrollPixelsPerUnit(&x, &scrollRate);
                int viewStartY;
                GetViewStart(&x, &viewStartY);
                int viewStartPy = viewStartY * scrollRate;
                int targetY = viewStartPy;
                if (pos.y < viewStartPy) {
                    targetY = pos.y;
                }
                else if (pos.y + target->GetSize().y > viewStartPy + swSize.y) {
                    targetY = pos.y + target->GetSize().y - swSize.y;
                }
                int targetUnitY = targetY / scrollRate;
                Scroll(0, targetUnitY + 1);
            }
        }
	}
public:
	ScrolledPanel(wxWindow* parent) : wxScrolledWindow(parent) {
        SetScrollRate(0, FromDIP(5));
        Bind(EVT_SCROLL_TO, &ScrolledPanel::OnScrollTo, this);
        Bind(wxEVT_CHAR_HOOK, &ScrolledPanel::OnKey, this);
	}
};