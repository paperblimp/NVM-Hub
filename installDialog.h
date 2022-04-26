#pragma once

#include "wx/wx.h"
#include "wx/radiobut.h"
#include "wx/grid.h"
#include <wx/tokenzr.h>
#include <wx/dynarray.h>

#include <vector>

using std::vector;

class installDialog : public wxDialog
{
public:
	installDialog(wxWindow* parent, wxString& title, wxArrayString& installedVersions);
	~installDialog();

	wxBoxSizer* mainSizer = nullptr;
	wxGridSizer* versionsGrid = nullptr;
	wxRadioButton** buttons;

	void setupInterface();

	auto getAvailableVersions();
	void renderAvailableVersions();

	wxArrayString installedVersions;
	wxString selectedVersion = "";

	void OnVersionSelected(wxCommandEvent& event);
	void OnCancelPressed(wxCommandEvent& event);
	void OnInstallPressed(wxCommandEvent& event);


};

