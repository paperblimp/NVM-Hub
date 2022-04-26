#pragma once

#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/filepicker.h"
#include "wx/filename.h"

#include "installs.h"

#include <vector>
using std::vector;

class projectDialog : public wxDialog
{
public:
	projectDialog(wxWindow* parent, wxString& title,vector<wxArrayString>& projectsData);
	~projectDialog();

	wxWindow* parent;
	wxBoxSizer* mainSizer = nullptr;
	wxBoxSizer* optionsSizer = nullptr;

	wxTextCtrl* projectName = nullptr;
	wxDirPickerCtrl* projectDir = nullptr;
	wxString selectedVersion = "";

	vector<wxArrayString> projectsData;
	wxArrayString installedVersions;
	wxListCtrl* versionsList;
	wxImageList* images;
	long int iconIndex;


	void setupVersions();
	void setupSettings();
	void setupButtons();

	void OnCancelPressed(wxCommandEvent& event);
	void OnCreatePressed(wxCommandEvent& event);
	void OnVersionPressed(wxListEvent& event);
};

