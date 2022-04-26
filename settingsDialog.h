#pragma once
#include "wx/wx.h"
#include "wx/filepicker.h"
#include "wx/textfile.h"
#include "wx/tokenzr.h"
#include "wx/filename.h"

#include <utility>
#include <unordered_map>
using std::unordered_map;
using std::pair;

class settingsDialog : public wxDialog
{
public:
	settingsDialog(wxWindow* parent, wxString& title);
	~settingsDialog();

	void setupInterface();
	void setupSettings();
	void setupButtons();

	wxBoxSizer* mainSizer = nullptr;
	wxFilePickerCtrl* textEditor = nullptr;

	unordered_map<wxString, wxString> settingsData;
	void loadSettings();
	bool saveSettings();

	void OnCancelPressed(wxCommandEvent& event);
	void OnSavePressed(wxCommandEvent& event);

};

