#include "Includes.h"
#include "MainFrame.h"

class App : public wxApp {
private:
	int RenameBand() {
		BandRow* row = dynamic_cast<BandRow*>(focused->GetParent());
		if (!row->Rename()) {
			focused = nullptr;
			base->SetFocus();
			return Event_Processed;
		}

		focused->SetFocus();
		return Event_Processed;
	}
	int ChangeStart() {
		BandRow* row = dynamic_cast<BandRow*>(focused->GetParent());
		if (!row->ChangeStart()) {
			focused = nullptr;
			base->SetFocus();
			return Event_Processed;
		}
		focused->SetFocus();
		return Event_Processed;
	}
	int ChangeEnd() {
		BandRow* row = dynamic_cast<BandRow*>(focused->GetParent());
		if (!row->ChangeEnd()) {
			focused = nullptr;
			base->SetFocus();
			return Event_Processed;
		}
		focused->SetFocus();
		return Event_Processed;
	}
public:
    bool OnInit() {
        MainFrame* mainFrame = new MainFrame("Title");
        mainFrame->Show();
        return true;
    }
    int FilterEvent(wxEvent& e) {
		const wxEventType t = e.GetEventType();
		if (t == wxEVT_LEFT_DOWN || t == wxEVT_RIGHT_DOWN) {
			if (focused && focused != e.GetEventObject()) {
				switch ((int)focused->GetClientData())
				{
				case BandName:
					return RenameBand();
					break;
				case Start:
					return ChangeStart();
					break;
				case End:
					return ChangeEnd();
					break;
				default:
					break;
				}
			}
		}
		return Event_Skip;
	}
};

wxIMPLEMENT_APP(App);