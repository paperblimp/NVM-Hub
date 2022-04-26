#pragma once

#include "wx/wx.h"
#include "wx/listbook.h"
#include "wx/listctrl.h"
#include "wx/imaglist.h"
#include "wx/icon.h"
#include "wx/activityindicator.h"
#include "wx/filename.h"

#include "waitDialog.h"


class installs
{
public:
	installs(wxFrame* root, wxListbook* parent);
	~installs();

	wxFrame* root = nullptr;
	wxListbook* parent = nullptr;
	wxPanel* panel = nullptr;
	wxBoxSizer* mainSizer = nullptr;

	wxImageList* images = nullptr;
	long int iconIndex;


	wxListCtrl* installsList = nullptr;
	wxArrayString installedVersions;

	void setupTitle();
	void setupInstalls();
	static void getInstalledVersions(wxArrayString& installedVersions);
	static void loadInstalledVersions(wxArrayString& installList, wxListCtrl& wxListCtrl, const int iconIndex);
	
	void installVersion(wxString& version);
	void uninstallVersion(wxString& version);
	void OnAddPressed(wxCommandEvent& event);
	void OnVersionPressed(wxListEvent& event);
};

