#include "installDialog.h"


installDialog::installDialog(wxWindow* parent, wxString& title, wxArrayString& installedVersions)
	: wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400))
{
	for (int i = 0; i < installedVersions.GetCount(); i++)
	{
		this->installedVersions.Add(installedVersions[i]);
	}

	setupInterface();
}

auto installDialog::getAvailableVersions()
{
	//Formats a dictionary with the versions available to download using the output from nvm

	vector<wxArrayString> availableVersions;
	availableVersions.resize(4);
	
	for (int i = 0; i < 4; i++)
	{
		availableVersions[i].Alloc(20);
	}

	wxArrayString output;
	wxArrayString errors;
	wxExecute("nvm list available",output,errors);


	for (int i = 0; i < output.GetCount(); i++) {
		
		//Splits and sanitizes the line
		wxArrayString line;

		output[i].Replace(" ", "");
		wxStringTokenizer tokenizer(output[i], "|");
		while (tokenizer.HasMoreTokens()) {
			line.Add(tokenizer.GetNextToken());
		}


		//Ignores empty/separation lines
		if (output[i] == "" || output[i][1]=='-') {
			continue;
		}

		//Ignores non-numerical lines
		if(!output[i].Mid(1,1).IsNumber()) {
			continue;
		}


		//Adds versions to availableVersions
		for (size_t v = 0; v < 4; v++)
		{
			availableVersions[v].Add(line[v + 1]);
		}

	}

	return availableVersions;
}


void installDialog::renderAvailableVersions()
{
	wxScrolledWindow* versionsWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);

	auto versions = getAvailableVersions();
	const int collumSize = versions[0].GetCount();

	versionsGrid = new wxGridSizer(collumSize + 1, 4, 1, 1);

	wxStaticText* titles[4]{
		new wxStaticText(versionsWindow,wxID_ANY,"CURRENT"),
		new wxStaticText(versionsWindow,wxID_ANY,"LTS"),
		new wxStaticText(versionsWindow,wxID_ANY,"OLD STABLE"),
		new wxStaticText(versionsWindow,wxID_ANY,"OLD UNSTABLE")
	};

	for (int i = 0; i < 4; i++)
	{
		titles[i]->SetFont(wxFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD)));
		versionsGrid->Add(titles[i]);
	}

	buttons = new wxRadioButton* [collumSize * 4];

	for (int i = 0; i < collumSize; i++)
	{

		for (size_t j = 0; j < 4; j++)
		{
			const int index = (i * 4) + j;
			buttons[index] = new wxRadioButton(versionsWindow, 10000 + index, versions[j][i], wxDefaultPosition, wxDefaultSize);
			versionsGrid->Add(buttons[index], 1, wxEXPAND | wxUP, 3);
			buttons[index]->Bind(wxEVT_RADIOBUTTON, &installDialog::OnVersionSelected, this);

			int alreadyInstalled = installedVersions.Index(versions[j][i]);
			if (alreadyInstalled != wxNOT_FOUND) {
				installedVersions.RemoveAt(alreadyInstalled);

				buttons[index]->SetForegroundColour(wxColor(120,120,120));
			}
		}
	}

	wxSize buttonSize = buttons[1]->GetSize();
	versionsWindow->SetSizer(versionsGrid);
	versionsWindow->SetScrollbars(buttonSize.x, buttonSize.y, buttonSize.x * 4, buttonSize.y * 100);
	versionsGrid->Layout();
	mainSizer->Add(versionsWindow, 1, wxEXPAND | wxALL, 10);
}

void installDialog::setupInterface()
{
	mainSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText* title = new wxStaticText(this, wxID_ANY, "Select a version of Node.js");
	title->SetFont(wxFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD)));
	mainSizer->Add(title, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

	renderAvailableVersions();

	//Buttons setup

	wxBoxSizer* buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* cancelButton = new wxButton(this, wxID_ANY, "CANCEL");
	wxButton* installButton = new wxButton(this, wxID_ANY, "INSTALL");
	cancelButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &installDialog::OnCancelPressed, this);
	installButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &installDialog::OnInstallPressed, this);
	installButton->SetBackgroundColour(wxColor(140, 200, 75));

	buttonsSizer->Add(cancelButton,0, wxALL,10);
	buttonsSizer->AddStretchSpacer(1);
	buttonsSizer->Add(installButton,0 ,wxALL,10);
	buttonsSizer->Layout();

	mainSizer->Add(buttonsSizer,0.2,wxEXPAND );

	SetSizer(mainSizer);
	mainSizer->Layout();
	Centre();
}

void installDialog::OnVersionSelected(wxCommandEvent& event)
{
	selectedVersion = buttons[event.GetId() - 10000]->GetLabel();
	event.Skip();
}

void installDialog::OnCancelPressed(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
	event.Skip();
}

void installDialog::OnInstallPressed(wxCommandEvent& event)
{
	EndModal(wxID_OK);
	event.Skip();
}


installDialog::~installDialog()
{
	delete buttons;
}