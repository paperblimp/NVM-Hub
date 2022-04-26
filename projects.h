#pragma once

#include "wx/wx.h"
#include "wx/listbook.h"
#include "wx/listctrl.h"
#include "wx/tokenzr.h"
#include "wx/textfile.h"
#include "wx/filename.h"
#include "wx/filefn.h"
#include "wx/filedlg.h"

#include "installs.h"
#include "projectDialog.h"
#include "Sort.h"

#include <algorithm>
#include <utility>
#include <vector>
using std::vector;
using std::sort;

class projects 
{
public:
	projects(wxFrame* root, wxListbook* parent);
	~projects();

	wxFrame* root = nullptr;
	wxListbook* parent = nullptr;
	wxPanel* panel = nullptr;
	wxBoxSizer* mainSizer = nullptr;
	wxListCtrl* projectsList = nullptr;
	vector<wxArrayString> projectsData;

	void setupTitle();
	void setupProjects();

	void OnAddPressed(wxCommandEvent& event);
	void OnNewPressed(wxCommandEvent& event);
	void OnProjectsLeftClick(wxListEvent& event);
	void OnProjectsRightClick(wxListEvent& event);
	void OnCollumClick(wxListEvent& event);

	void getProjects();
	void loadProjects();
	void indexProject(const wxString& name, const wxString& path, const wxString& version);
	void createProject(const wxString& name, const wxString& path, const wxString& version);
	void deleteProject(const wxString& name);
	void openProject(const wxString& path, const wxString& version);


	int currentSort = -1;
	void callSort(const int newSort);
	wxString getDateDiffMessage(const wxString& rawDate);

};

