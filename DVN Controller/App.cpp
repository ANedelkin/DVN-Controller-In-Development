#include "MainFrame.h"

class App : public wxApp {
public:
    bool OnInit() {
        MainFrame* mainFrame = new MainFrame();
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
					row->Rename();
				else if(type == BandRow::Start|| type == BandRow::End)
					row->UpdateFreqs(type);

				focused = nullptr;

				type = (int)target->GetClientData();
				if (t == wxEVT_CLOSE_WINDOW || wxGetMouseState().LeftIsDown() && 
											  (type == BandRow::Name || type == BandRow::Start || type == BandRow::End || type == BandRow::StatBtn)) 
					return Event_Skip;
				row->Unfocus();
				
				return Event_Processed;
			}
		}
		return Event_Skip;
	}
};

wxIMPLEMENT_APP(App);