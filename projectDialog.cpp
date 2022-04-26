#include "projectDialog.h"


projectDialog::projectDialog(wxWindow* parent, wxString& title, vector<wxArrayString>& projectsData)
	: wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(100,100))
{

	this->mainSizer = new wxBoxSizer(wxVERTICAL);
	this->optionsSizer = new wxBoxSizer(wxHORIZONTAL);
	this->projectsData = projectsData;

	setupVersions();
	setupSettings();
	setupButtons();

	optionsSizer->Layout();



	this->SetSizer(mainSizer);
	mainSizer->Layout();
	mainSizer->Fit(this);
	this->Centre();
}


void projectDialog::setupVersions()
{
	images = new wxImageList(125, 140);

	wxFileName imagePath("Images", "nodejsIcon.png");
	wxImage image(imagePath.GetFullPath(), wxBITMAP_TYPE_PNG);
	image.Rescale(125, 140, wxIMAGE_QUALITY_HIGH);
	wxBitmap bitmap(image);
	iconIndex = images->Add(bitmap);

	wxBoxSizer* versionsSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* title = new wxStaticText(this, wxID_ANY, "Versions");
	title->SetFont((wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD)));
	title->SetOwnForegroundColour(*wxLIGHT_GREY);
	versionsSizer->Add(title, 0.1, wxALL, 10);

	versionsList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(360,360), wxLC_ICON);
	versionsList->AssignImageList(images, wxIMAGE_LIST_NORMAL);
	versionsList->Bind(wxEVT_LIST_ITEM_SELECTED, &projectDialog::OnVersionPressed, this);

	versionsList->SetFont((wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)));

	versionsSizer->Add(versionsList, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);
	versionsSizer->Layout();
	optionsSizer->Add(versionsSizer,0.5,wxALL | wxEXPAND, 10);

	installs::loadInstalledVersions(installedVersions, *versionsList, iconIndex);



}

void projectDialog::setupSettings()
{
	wxBoxSizer* settingsSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* title = new wxStaticText(this, wxID_ANY, "Settings");
	title->SetFont((wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD)));
	title->SetOwnForegroundColour(*wxLIGHT_GREY);
	settingsSizer->Add(title, 0.1);


	wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, "Project name*");
	nameLabel->SetFont((wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)));
	nameLabel->SetForegroundColour(wxColor(140, 200, 75));
	projectName = new wxTextCtrl(this, wxID_ANY);

	wxBoxSizer* nameSizer = new wxBoxSizer(wxVERTICAL);
	nameSizer->Add(nameLabel, 1, wxEXPAND);
	nameSizer->Add(projectName, 1, wxEXPAND);

	wxStaticText* dirLabel = new wxStaticText(this, wxID_ANY, "Location*");
	dirLabel->SetFont((wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)));
	dirLabel->SetForegroundColour(wxColor(140, 200, 75));
	wxString message("Please select the folder for your new project");
	projectDir = new wxDirPickerCtrl(this,wxID_ANY,"",message,wxDefaultPosition,wxDefaultSize, wxFLP_OPEN | wxFLP_FILE_MUST_EXIST | wxFLP_USE_TEXTCTRL);
	projectDir->SetMinSize(wxSize(400, 0));

	wxBoxSizer* dirSizer = new wxBoxSizer(wxVERTICAL);
	dirSizer->Add(dirLabel, 1,wxEXPAND);
	dirSizer->Add(projectDir, 1, wxEXPAND);

	settingsSizer->Add(nameSizer, 1, wxRIGHT | wxUP | wxDOWN| wxEXPAND, 20);
	settingsSizer->Add(dirSizer, 1,  wxRIGHT | wxUP | wxDOWN| wxEXPAND, 20);

	optionsSizer->Add(settingsSizer, 1, wxALL , 10);
	optionsSizer->Layout();

	mainSizer->Add(optionsSizer, 0.8,wxALL |  wxEXPAND,10);

}

void projectDialog::setupButtons()
{
	wxBoxSizer* buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* cancel = new wxButton(this, wxID_ANY, "CANCEL");
	wxButton* create = new wxButton(this, wxID_ANY, "CREATE");
	cancel->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &projectDialog::OnCancelPressed, this);
	create->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &projectDialog::OnCreatePressed, this);
	cancel->SetMinSize(wxSize(100, 40));
	create->SetMinSize(wxSize(100, 40));
	create->SetBackgroundColour(wxColor(140, 200, 75));

	buttonsSizer->Add(cancel,1,wxRIGHT,10);
	buttonsSizer->Add(create,1,wxRIGHT,25);
	buttonsSizer->Layout();

	mainSizer->Add(buttonsSizer, 0.2, wxALIGN_RIGHT | wxALL , 10);
}

void projectDialog::OnVersionPressed(wxListEvent& event)
{
	selectedVersion = event.GetLabel();
	event.Skip();
}

void projectDialog::OnCreatePressed(wxCommandEvent& event)
{
	//Error messages
	if (projectName->GetValue() == "") {
		wxString message = "Please type a name for your project.";
		wxMessageDialog result(NULL, message, wxT("Error"), wxICON_ERROR | wxOK);
		result.ShowModal();
		return;
	}
	else if (projectDir->GetPath() == "") {
		wxString message = "Please choose a path for your project.";
		wxMessageDialog result(NULL, message, wxT("Error"), wxICON_ERROR | wxOK);
		result.ShowModal();
		return;
	}
	else if (!wxFileName::DirExists(projectDir->GetPath()))
	{
		wxString message = "Please choose an existing directory.";
		wxMessageDialog result(NULL, message, wxT("Error"), wxICON_ERROR | wxOK);
		result.ShowModal();
		return;
	}
	else if (selectedVersion == "") {
		wxString message = "Please select a Node.js version for your project.";
		wxMessageDialog result(NULL, message, wxT("Error"), wxICON_ERROR | wxOK);
		result.ShowModal();
		return;
	}
	for (int i = 0; i < projectsData.size(); i++)
	{
		if (projectName->GetValue() == projectsData[i][0]) {
			wxString message = "Project name already in use.";
			wxMessageDialog result(NULL, message, wxT("Error"), wxICON_ERROR | wxOK);
			result.ShowModal();
			return;
		}
	}

	EndModal(wxID_OK);
	event.Skip();
}

void projectDialog::OnCancelPressed(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
	event.Skip();
}




projectDialog::~projectDialog()
{

}
