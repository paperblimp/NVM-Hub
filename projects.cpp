#include "projects.h"


projects::projects(wxFrame* root, wxListbook* parent)
{
	this->root = root;
	this->parent = parent;
	this->panel = new wxPanel(parent);
	this->mainSizer = new wxBoxSizer(wxVERTICAL);

	setupTitle();
	setupProjects();

	this->panel->SetSizer(mainSizer);
	mainSizer->Layout();


}
void projects::setupTitle()
{
	wxBoxSizer* titleSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Projects");
	title->SetFont(wxFont(26, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	wxButton* addButton = new wxButton(panel, wxID_ANY, "ADD", wxDefaultPosition, wxSize(100, 40));
	addButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &projects::OnAddPressed, this);
	wxButton* newButton = new wxButton(panel, wxID_ANY, "NEW", wxDefaultPosition, wxSize(100, 40));
	newButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &projects::OnNewPressed, this);
	newButton->SetFocus();

	newButton->SetBackgroundColour(wxColor(140, 200, 75));

	titleSizer->Add(title, 1, wxRIGHT | wxLEFT  | wxALIGN_CENTER_VERTICAL, 10);
	titleSizer->Add(addButton, 0.5, wxALL, 10);
	titleSizer->Add(newButton, 0.5, wxALL, 10);
	titleSizer->Layout();

	mainSizer->Add(titleSizer, 0.5, wxEXPAND);
}

void projects::setupProjects()
{
	projectsList = new wxListCtrl(panel,wxID_ANY,wxDefaultPosition,wxDefaultSize, wxLC_REPORT | wxLC_HRULES );
	projectsList->Bind(wxEVT_LIST_ITEM_ACTIVATED, &projects::OnProjectsLeftClick, this);
	projectsList->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &projects::OnProjectsRightClick, this);
	projectsList->Bind(wxEVT_LIST_COL_CLICK, &projects::OnCollumClick, this);

	const int collumSize = 212;
	wxListItem col0;
	col0.SetId(0);
	col0.SetText("Project Name");
	col0.SetWidth(collumSize);
	wxListItem col1;
	col1.SetId(0);
	col1.SetText("Path");
	col1.SetWidth(collumSize+3);
	wxListItem col2;
	col2.SetId(1);
	col2.SetText("Node version");
	col2.SetWidth(collumSize);
	wxListItem col3;
	col3.SetId(2);
	col3.SetText("Last Modified");
	col3.SetWidth(collumSize);

	projectsList->InsertColumn(0,col0);
	projectsList->InsertColumn(1,col1);
	projectsList->InsertColumn(2,col2);
	projectsList->InsertColumn(3, col3);

	getProjects();
	callSort(4);
	loadProjects();

	mainSizer->Add(projectsList,1, wxLEFT | wxRIGHT | wxEXPAND,10);

}

void projects::getProjects()
{
	wxFileName projectsIndexPath(wxFileName::GetCwd(), "index.csv");
	if (!projectsIndexPath.Exists()) return;

	//Iterates through projects.csv, splits the data and loads each index as a collum
	wxTextFile projectsIndex(projectsIndexPath.GetFullPath());
	projectsIndex.Open();
	if (!projectsIndex.IsOpened()) return;

	projectsData.clear();


	for (int i = 0; i < projectsIndex.GetLineCount(); i++)
	{
		wxStringTokenizer line(projectsIndex[i], ",");
		if (line.CountTokens() != 4) continue;
		wxArrayString lineData;
		lineData.resize(4);
		for (int t = 0; t < 4; t++)
		{
			lineData[t] = line.GetNextToken();
		}
		projectsData.push_back(lineData);
	}
}

void projects::loadProjects()
{


	projectsList->DeleteAllItems();

	for (wxArrayString data : projectsData)
	{
		//Checks if the project folder still exists
		wxFileName projectFolder(data[1], "");
		if (not projectFolder.Exists()) continue;

		int index = projectsList->InsertItem(0, 1);
		for (int i = 0; i < 4; i++)
		{
			if (i == 3) {
				wxString message = getDateDiffMessage(data[i]);
				projectsList->SetItem(index, i, message);
				continue;
			}
			projectsList->SetItem(index, i, data[i]);
		}
	}

}

wxString projects::getDateDiffMessage(const wxString& rawDate)
{
	//Calculates the message of the Last Modified collum
	wxDateTime firstDay;
	firstDay.ParseISODate(rawDate);
	wxDateTime secondDay = wxDateTime::Now();
	wxTimeSpan time = secondDay - firstDay;
	int days = time.GetDays();

	wxString message;
	if (days == 0) message = "Today";
	else if (days == 1) message = "Yesterday";
	else message = wxString::Format("%i days ago", days);

	return message;
}

void projects::createProject(const wxString& name, const wxString& path, const wxString& version)
{
	//Creates the project folder and the .nvmhub file to storage the project data

	wxFileName projectFolder(path,"");
	projectFolder.AppendDir(name);
	wxMkDir(projectFolder.GetFullPath());

	wxFileName projectFile(projectFolder.GetFullPath(), ".nvmhub");
	wxTextFile projectData(projectFile.GetFullPath());
	projectData.Create();
	projectData.Open();
	
	if (projectData.IsOpened()) {
		projectData.AddLine(name);
		projectData.AddLine(version);
		projectData.Write();
		projectData.Close();
	}

}

void projects::deleteProject(const wxString& name)
{
	//Removes the line with data of this project from index.csv

	wxFileName indexPath(wxFileName::GetCwd(), "index.csv");
	wxTextFile indexFile(indexPath.GetFullPath());

	if (indexPath.Exists()) {
		indexFile.Open();
	}
	else return;


	if (indexFile.IsOpened()) {

		for (int i = 0; i < indexFile.GetLineCount(); i++)
		{
			if (indexFile[i].starts_with(name)) {
				indexFile.RemoveLine(i);
				indexFile.Write();
				indexFile.Close();
				break;
			}
		}

	}
}

void projects::indexProject(const wxString& name, const wxString& path, const wxString& version)
{
	//Creates a new .csv file to storage the data of the projects or appends to an existing one
	wxFileName indexPath(wxFileName::GetCwd(), "index.csv");
	wxTextFile indexFile(indexPath.GetFullPath());

	if (indexPath.Exists()) {
		indexFile.Open();
	}
	else {
		indexFile.Create();
		indexFile.Open();
	}


	if (indexFile.IsOpened()) {

		wxString date = wxDateTime::Now().FormatISODate();
		wxString lineData = wxString(name + "," + path + "," + version + "," + date);
		indexFile.AddLine(lineData);
		indexFile.Write();
		indexFile.Close();

	}
}

void projects::openProject(const wxString& path, const wxString& version)
{
	//Checks if the project version is installed
	wxArrayString installedVersions;
	installs::getInstalledVersions(installedVersions);
	if (installedVersions.Index(version) == wxNOT_FOUND) {
		wxString message("Version not installed. Please install it to open this project.");
		wxMessageDialog result(NULL, message, "Error", wxICON_ERROR | wxOK);
		result.ShowModal();
		return;
	}

	//Searchs for the text editor path
	wxFileName settingsPath(wxFileName::GetCwd(), "settings.data");
	if (!settingsPath.Exists()) {
		wxString message("No text editor selected.");
		wxMessageDialog result(NULL, message, "Error", wxICON_ERROR | wxOK);
		result.ShowModal();
		return;
	}

	wxTextFile settingsFile(settingsPath.GetFullPath());
	settingsFile.Open();

	wxString editorPath;
	for (int i = 0; i < settingsFile.GetLineCount(); i++)
	{
		if (settingsFile[i].BeforeFirst(':') == "EditorPath") {
			editorPath = settingsFile[i].AfterFirst(':');
		}
	}

	if (editorPath == "") {
		wxString message("No text editor selected.");
		wxMessageDialog result(NULL, message, "Error", wxICON_ERROR | wxOK);
		result.ShowModal();
		return;
	}

	//Moves the cwd to the project, set the nvm version, opens the text editor and moves back to the old cwd
	wxString oldPath = wxFileName::GetCwd();
	wxSetWorkingDirectory(path);
	wxExecute("nvm use " + version,wxEXEC_HIDE_CONSOLE);
	wxLogDebug(editorPath + " .");
	wxExecute(editorPath + " .", wxEXEC_HIDE_CONSOLE);
	wxSetWorkingDirectory(oldPath);
}

void projects::OnNewPressed(wxCommandEvent& event)
{
	//Creates a new project with data from the projectDialog
	wxString title("Create a new Node.js project");
	projectDialog* dialog = new projectDialog(root, title,projectsData);
	if (dialog->ShowModal() == wxID_OK) {
		dialog->projectName->GetValue().Replace(",", "");
		createProject(
			dialog->projectName->GetValue(),
			dialog->projectDir->GetPath(),
			dialog->selectedVersion);
		wxFileName projectDir(dialog->projectDir->GetPath(),"");
		projectDir.AppendDir(dialog->projectName->GetValue());
		indexProject(
			dialog->projectName->GetValue(),
			projectDir.GetPath(),
			dialog->selectedVersion);
	}

	dialog->Destroy();
	getProjects();
	currentSort = -1;
	callSort(4);
	loadProjects();
	event.Skip();
}

void projects::OnAddPressed(wxCommandEvent& event)
{
	//Opens a dialog that gets a .nvmhub file and read it's content in  order to load an existing project 
	wxString message = "Please choose a .nvmhub file.";
	wxString extensions = ".NVMHUB files (*.nvmhub)|*.nvmhub";
	wxFileDialog addDialog(root, message, "", "", extensions, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (addDialog.ShowModal() == wxID_OK) {
		wxFileName dataPath(addDialog.GetPath());
		wxTextFile dataFile(addDialog.GetPath());
		dataFile.Open();
		if (dataFile.IsOpened()) {
			wxString name = dataFile.GetLine(0);

			//Avoid duplicate name projects
			for (int i = 0; i < projectsData.size(); i++)
			{
				if (name == projectsData[i][0]) {
					wxString message = "There's already a project with this name.";
					wxMessageDialog result(NULL, message, wxT("Error"), wxICON_ERROR | wxOK);
					result.ShowModal();
					return;
				}
			}
			wxString version = dataFile.GetLine(1);
			indexProject(name, dataPath.GetPath(), version);
		}
	}
	getProjects();
	currentSort = -1;
	callSort(4);
	loadProjects();
	event.Skip();
}

void projects::OnProjectsLeftClick(wxListEvent& event)
{
	for (wxArrayString data : projectsData)
	{
		if (data[0] == event.GetLabel())
		{
			openProject(data[1], data[2]);
		}
	}

}

void projects::OnProjectsRightClick(wxListEvent& event)
{
	wxString message("Do you really want to delete this project? Files will not be deleted.");
	wxMessageDialog result(NULL, message, "Warn", wxYES_NO | wxYES_DEFAULT | wxICON_INFORMATION);
	if (result.ShowModal() == wxID_YES) {
		deleteProject(event.GetLabel());
		getProjects();
		currentSort = -1;
		callSort(4);
		loadProjects();
	}
}

void projects::OnCollumClick(wxListEvent& event)
{
	const int newSort = event.GetColumn() + 1;
	callSort(newSort);
}

void projects::callSort(const int newSort)
{
	//Checks the collum the user pressed in order to sort
	//If it's same of the previous sort, it inverts the sort, otherwise sorts asc
	
	switch (newSort)
	{
	case 1:
		if (newSort != currentSort) {
			currentSort = newSort;
			sort(projectsData.begin(), projectsData.end(), Sort::compareNameAsc);
		}
		else {
			currentSort = newSort * -1;
			if (currentSort < 0) sort(projectsData.begin(), projectsData.end(), Sort::compareNameDesc);
			else sort(projectsData.begin(), projectsData.end(), Sort::compareNameAsc);
		}
		break;
	case 2:
		if (abs(newSort) != currentSort) {
			currentSort = newSort;
			sort(projectsData.begin(), projectsData.end(), Sort::comparePathAsc);
		}
		else {
			currentSort = newSort * -1;
			if (currentSort < 0) sort(projectsData.begin(), projectsData.end(), Sort::comparePathDesc);
			else sort(projectsData.begin(), projectsData.end(), Sort::comparePathAsc);
		}
		break;
	case 3:
		if (abs(newSort) != currentSort) {
			currentSort = newSort;
			sort(projectsData.begin(), projectsData.end(), Sort::compareVersionAsc);
		}
		else {
			currentSort = newSort * -1;
			if (currentSort < 0) sort(projectsData.begin(), projectsData.end(), Sort::compareVersionDesc);
			else sort(projectsData.begin(), projectsData.end(), Sort::compareVersionAsc);
		}
		break;
	case 4:
		if (abs(newSort) != currentSort) {
			currentSort = newSort;
			sort(projectsData.begin(), projectsData.end(), Sort::compareDateAsc);
		}
		else {
			currentSort = newSort * -1;
			if (currentSort < 0) sort(projectsData.begin(), projectsData.end(), Sort::compareDateDesc);
			else sort(projectsData.begin(), projectsData.end(), Sort::compareDateAsc);
		}
		break;
	default:
		break;
	}
	loadProjects();
}


projects::~projects()
{

}