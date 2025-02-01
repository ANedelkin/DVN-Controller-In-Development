#pragma once
#include "Global.h"
#include "Scenario.h"

class ScenCtrl : public wxPanel {
private:
	wxBoxSizer* sizer;
	wxButton* def;
	wxButton* selected;
	wxTextCtrl* renameCtrl;

	void OnEnter(wxCommandEvent& e) {
		if (!Rename()) base->SetFocus();
	}
	void OnFocus(wxFocusEvent& e) {
		focused = this;
		e.Skip();
	}
public:
	enum Modes {
		Default,
		Selected,
		Renaming,
	};

	Modes mode = Renaming;

	Scenario* scenario;

	ScenCtrl(wxWindow* parent, Scenario* scenario) : wxPanel(parent) {
		this->scenario = scenario;
		
		sizer = new wxBoxSizer(wxHORIZONTAL);
		def = new wxButton(this, wxID_ANY, scenario->name);
		def->Hide();
		selected = new wxButton(this, wxID_ANY, scenario->name);
		selected->SetBackgroundColour(wxColour(255, 255, 255));
		selected->Hide();
		renameCtrl = new wxTextCtrl(this, wxID_ANY, scenario->name, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		
		renameCtrl->Bind(wxEVT_TEXT_ENTER, &ScenCtrl::OnEnter, this);
		renameCtrl->Bind(wxEVT_SET_FOCUS, &ScenCtrl::OnFocus, this);

		renameCtrl->SetClientData((void*)ScenName);

		sizer->Add(def, 1);
		sizer->Add(selected, 1);
		sizer->Add(renameCtrl, 1);
		this->SetSizerAndFit(sizer);
	}

	Status Rename() {
		string newName = renameCtrl->GetLineText(0).ToStdString();
		Status stat = ValidateName(newName);
		if (stat) {
			wxMessageDialog dialog(base, errorMessages[stat], "Error", wxOK | wxICON_ERROR);
			dialog.ShowModal();
		}
		else {
			scenario->name = newName;
			SetMode(Selected);
			Layout();
			focused = nullptr;
		}
		return stat;
	}

	void SetMode(int mode) {
		switch (mode)
		{
		case Renaming:
			def->Hide();
			selected->Hide();
			renameCtrl->Show();
			break;
		case Default:
			def->Show();
			selected->Hide();
			renameCtrl->Hide();
			break;
		case Selected:
			Select();
			break;
		default:
			break;
		}
		Layout();
	}

	void Select() {
		def->Hide();
		selected->Show();
		renameCtrl->Hide();

		for (wxWindow* sibling : (GetParent()->GetChildren()))
		{
			ScenCtrl* scen = dynamic_cast<ScenCtrl*>(sibling);
			if (scen->mode == Selected) {
				scen->SetMode(Default);
				break;
			}
		}
	}
};