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
		if (t == wxEVT_LEFT_DOWN || t == wxEVT_RIGHT_DOWN) {
			if (focused && focused != e.GetEventObject()) {
				base->Refresh();

				BandRow* row = dynamic_cast<BandRow*>(focused->GetParent());
				Status stat = Success;

				switch ((int)focused->GetClientData())
				{
				case BandName:
					stat = row->Rename();
					break;
				case Start:
					stat = row->ChangeStart();
					break;
				case End:
					stat = row->ChangeEnd();
					break;
				default:
					break;
				}

				if (!stat) { //Success
					focused = nullptr;
					base->SetFocus();
					return Event_Processed;
				}
				else //Failure, like you
					focused->SetFocus();
				
				return Event_Processed;
			}
		}
		return Event_Skip;
	}
};

wxIMPLEMENT_APP(App);