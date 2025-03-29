#include "Includes.h"
#include "MainFrame.h"

class App : public wxApp {
public:
    bool OnInit() {
        MainFrame* mainFrame = new MainFrame("Title");
        mainFrame->Show();
        return true;
    }
    int FilterEvent(wxEvent& e) {
		const wxEventType t = e.GetEventType();
		if (t == wxEVT_LEFT_DOWN || t == wxEVT_RIGHT_DOWN || t == wxEVT_CLOSE_WINDOW) {
			wxWindow* target = dynamic_cast<wxWindow*>(e.GetEventObject());
			if (focused && (focused != target || t == wxEVT_CLOSE_WINDOW && base && base == target)) {
				base->Refresh();

				BandRow* row = dynamic_cast<BandRow*>(focused->GetParent());
				Status stat = Success;

				switch ((int)focused->GetClientData())
				{
				case BandName:
					stat = row->Rename();
					break;
				case Start:
					stat = row->UpdateFreq(row->startValue);
					break;
				case End:
					stat = row->UpdateFreq(row->endValue);
					break;
				default:
					break;
				}

				if (!stat) { //Success
					focused = nullptr;

					int type = (int)(target->GetClientData());
					if (t == wxEVT_CLOSE_WINDOW || wxGetMouseState().LeftIsDown() && (type == BandName || type == Start || type == End)) return Event_Skip;
					row->Unfocus();
				}
				else //Failure, like me
					focused->SetFocus();
				
				return Event_Processed;
			}
		}
		return Event_Skip;
	}
};

wxIMPLEMENT_APP(App);