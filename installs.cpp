#include "installs.h"
#include "installDialog.h"


///////////////////////////////////////////Interface Setup///////////////////////////////////////////

installs::installs(wxFrame* root, wxListbook* parent)
{
	this->root = root;
	this->parent = parent;
	this->panel = new wxPanel(parent);
	this->mainSizer = new wxBoxSizer(wxVERTICAL);

	setupTitle();
	setupInstalls();


	this->panel->SetSizer(mainSizer);
	mainSizer->Layout();
}

void installs::setupTitle()
{
	wxBoxSizer* titleSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Installs");
	title->SetFont(wxFont(26, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	wxButton* addButton = new wxButton(panel, wxID_ANY, "ADD", wxDefaultPosition, wxSize(100, 40));
	addButton->SetBackgroundColour(wxColor(140, 200, 75));
	addButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &installs::OnAddPressed, this);

	titleSizer->Add(title, 1, wxRIGHT | wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	titleSizer->Add(addButton, 0.5, wxALL, 10);
	titleSizer->Layout();

	mainSizer->Add(titleSizer, 0.5, wxEXPAND);
}

void installs::setupInstalls()
{
	images = new wxImageList(125,140);

	wxFileName imagePath("Images", "NodejsIcon.png");
	wxImage image(imagePath.GetFullPath(), wxBITMAP_TYPE_PNG);
	image.Rescale(125, 140,wxIMAGE_QUALITY_HIGH);
	wxBitmap bitmap(image);
	iconIndex = images->Add(bitmap);

	installsList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON);
	installsList->Bind(wxEVT_LIST_ITEM_ACTIVATED, &installs::OnVersionPressed, this);
	installsList->AssignImageList(images, wxIMAGE_LIST_NORMAL);

	installsList->SetFont((wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)));

	mainSizer->Add(installsList, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

	loadInstalledVersions(installedVersions, *installsList, iconIndex);

}

void installs::getInstalledVersions(wxArrayString& installedVersions)
{
	installedVersions.Clear();

	wxArrayString output;
	wxArrayString errors;
	wxExecute("nvm list", output, errors);

	//Iterate trought the output sanitizing the strings and detecting installed versions
	int cVersion = -1;
	for (int i = 0; i < output.GetCount(); i++) {

		output[i].Replace(" ", "");
		if (output[i] == "") continue;

		const int asteriscPos = output[i].Find('*');
		if ((cVersion == -1) && (asteriscPos != wxNOT_FOUND)) {
			const int bracketPos = output[i].Find('(');
			output[i] = output[i].Mid(asteriscPos + 1, bracketPos - 1);
			cVersion = i;
		}
		installedVersions.Add(output[i]);

	}
}

void installs::loadInstalledVersions(wxArrayString &installedVersions,wxListCtrl & installsList,const int iconIndex)
{
	//Calls the function to load the version from nvm list, then render output graphically
	installs::getInstalledVersions(installedVersions);
	installsList.DeleteAllItems();

	for(int i = 0; i < installedVersions.GetCount(); i++)
	{
		installsList.InsertItem(i, installedVersions[i], iconIndex);
	}

}

void installs::installVersion(wxString& version)
{


	//Parses error cases
	if (version == "") {
		wxMessageDialog result(NULL,"No version selected", "Error", wxICON_ERROR | wxOK);
		result.ShowModal();
		OnAddPressed(*new wxCommandEvent());
		return;
	}
	else if (installedVersions.Index(version) != wxNOT_FOUND) {
		wxMessageDialog result(NULL, "Version already installed", "Error", wxICON_ERROR | wxOK);
		result.ShowModal();
		OnAddPressed(*new wxCommandEvent());
		return;
	}

	//Freezes the screen during the download
	wxString waitMessage = "Please wait while Node.js " + version + " is being installed";
	waitDialog* dialog = new waitDialog(root, waitMessage);
	dialog->Show();

	wxArrayString output;
	wxArrayString errors;
	wxExecute("nvm install " + version, output, errors);
	
	dialog->Close();
	dialog->Destroy();

	wxString finalMessage = "Node.js " + version + " was sucessfully installed";
	wxMessageDialog result(NULL, finalMessage, "Result", wxICON_INFORMATION | wxOK);
	result.ShowModal();

	loadInstalledVersions(installedVersions,*installsList,iconIndex);
}

void installs::OnAddPressed(wxCommandEvent& event)
{
	wxString title("Add Node.js Version");
	installDialog* dialog = new installDialog(root, title ,installedVersions);
	if (dialog->ShowModal() == wxID_OK){
		installVersion(dialog->selectedVersion);
	}

	dialog->Destroy();
	event.Skip();
}

void installs::uninstallVersion(wxString& version)
{
	wxString waitMessage = "Please wait while Node.js " + version + " is being uninstalled";
	waitDialog* dialog = new waitDialog(root, waitMessage);
	dialog->Show();

	wxArrayString output;
	wxArrayString errors;

	wxExecute("nvm uninstall " + version, output, errors);

	dialog->Close();
	dialog->Destroy();

	wxString finalMessage = "Node.js " + version + " was sucessfully uninstalled";
	wxMessageDialog result(NULL, finalMessage, wxT("Result"), wxICON_INFORMATION | wxOK);
	result.ShowModal();

	loadInstalledVersions(installedVersions,*installsList,iconIndex);
}

void installs::OnVersionPressed(wxListEvent& event)
{
	wxString version = event.GetLabel();
	wxString message = "Would you like to uninstall Node.js version " + version + "?";
	wxMessageDialog* uninstallDialog = new wxMessageDialog(root, message, "Uninstall", wxYES_NO | wxYES_DEFAULT | wxICON_INFORMATION);

	if (uninstallDialog->ShowModal() == wxID_YES)
	{
		uninstallVersion(version);
	}


	uninstallDialog->Destroy();
	event.Skip();
}


installs::~installs()
{

}