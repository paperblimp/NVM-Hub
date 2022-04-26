#include "settingsDialog.h"

settingsDialog::settingsDialog(wxWindow* parent, wxString& title)
	: wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{
	loadSettings();
	setupInterface();

	this->SetSizer(mainSizer);
	mainSizer->Layout();
	mainSizer->Fit(this);
	this->Centre();
}

void settingsDialog::setupInterface()
{
	mainSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText* title = new wxStaticText(this, wxID_ANY, "Settings");
	title->SetFont((wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD)));
	mainSizer->Add(title, 1, wxALIGN_CENTER_HORIZONTAL | wxUP , 10);

	setupSettings();
	setupButtons();
}

void settingsDialog::setupSettings()
{
	wxBoxSizer* textEditorSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText* textEditorLabel = new wxStaticText(this, wxID_ANY, "Text Editor Path: ");
	
	wxString oldEditorPath = settingsData["EditorPath"];
	wxString message("Please choose the file that initializes your text editor");
	textEditor = new wxFilePickerCtrl(this, wxID_ANY, oldEditorPath, message, "", 
		wxDefaultPosition, wxDefaultSize, wxFLP_OPEN | wxFLP_FILE_MUST_EXIST | wxFLP_USE_TEXTCTRL);
	textEditor->SetMinSize(wxSize(500, 50));
	textEditorSizer->Add(textEditorLabel, 0);
	textEditorSizer->Add(textEditor, 1);

	mainSizer->Add(textEditorSizer, 1, wxALL, 10);
}

void settingsDialog::setupButtons()
{
	wxBoxSizer* buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* cancel = new wxButton(this, wxID_ANY, "CANCEL");
	wxButton* save = new wxButton(this, wxID_ANY, "SAVE");
	cancel->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &settingsDialog::OnCancelPressed, this);
	save->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &settingsDialog::OnSavePressed, this);
	save->SetBackgroundColour(wxColor(140, 200, 75));

	buttonsSizer->Add(cancel, 0.3);
	buttonsSizer->AddStretchSpacer(1);
	buttonsSizer->Add(save, 0.3);
	buttonsSizer->Layout();

	mainSizer->Add(buttonsSizer, 0.2, wxALL | wxEXPAND, 10);
}

void settingsDialog::loadSettings()
{
	//Load settings from settings.data or create if not exists
	wxFileName settingsPath(wxFileName::GetCwd(), "settings.data");
	wxTextFile settingsFile(settingsPath.GetFullPath());

	if (settingsPath.Exists()) {
		settingsFile.Open();

		settingsData.clear();
		for (int i = 0; i < settingsFile.GetLineCount(); i++)
		{
			
			wxString key = settingsFile[i].BeforeFirst(':');
			wxString value = settingsFile[i].AfterFirst(':');
			settingsData[key] = value;

			
		}
	}
	else {
		settingsFile.Create();
		settingsFile.Open();

		wxString editorLine("EditorPath:");
		settingsFile.AddLine(editorLine);

		settingsFile.Write();
		settingsFile.Close();
	}

}

bool settingsDialog::saveSettings()
{
	//Error checking
	settingsData["EditorPath"] = textEditor->GetPath();
	wxFileName EditorPath(settingsData["EditorPath"]);
	if (!EditorPath.Exists()) {
		wxString message = "Text editor path does not exists.";
		wxMessageDialog result(NULL, message, wxT("Error"), wxICON_ERROR | wxOK);
		result.ShowModal();
		return false;
	}

	//Saves the settings in settings.data
	wxFileName settingsPath(wxFileName::GetCwd(), "settings.data");
	wxTextFile settingsFile(settingsPath.GetFullPath());

	if (settingsPath.Exists()) {
		settingsFile.Open();
		
		for (int i = 0; i < settingsFile.GetLineCount(); i++)
		{
			settingsFile.RemoveLine(i);
		}

		for(pair<wxString,wxString> data : settingsData)
		{
			settingsFile.AddLine(data.first + ":" + data.second);
		}

		settingsFile.Write();
		settingsFile.Close();
	}

	return true;
}

void settingsDialog::OnCancelPressed(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void settingsDialog::OnSavePressed(wxCommandEvent& event)
{
	if(saveSettings()) EndModal(wxID_OK);
}

settingsDialog::~settingsDialog()
{

}
