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
				assert(row != nullptr && "Focused's parent is not BandRow or derived.");
				string stat = ToString(Success);
				int type = (int)focused->GetClientData();
				if (type == BandRow::Name)
					stat = row->Rename();
				else if(type == BandRow::Start|| type == BandRow::End)
					stat = row->UpdateFreq(type);

				if (stat.empty()) { //Success
					focused = nullptr;

					int type = (int)target->GetClientData();
					if (t == wxEVT_CLOSE_WINDOW || wxGetMouseState().LeftIsDown() && 
												  (type == BandRow::Name || type == BandRow::Start || type == BandRow::End || type == BandRow::StatBtn)) 
						return Event_Skip;
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