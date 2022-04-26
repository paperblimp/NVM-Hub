#pragma once

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/statline.h"
#include "wx/statbmp.h"
#include "wx/filename.h"

#include "projects.h"
#include "installs.h"
#include "settingsDialog.h"

class myFrame : public wxFrame
{
public:
	myFrame();
	~myFrame();
	void renderWindow();

	wxPanel* mainPanel = nullptr;
	wxListbook* book = nullptr;
	wxImageList* images = nullptr;

	projects* projectsTab = nullptr;
	installs* installsTab = nullptr;

	int pressCount = 0;

	void OnSettingsPressed(wxCommandEvent& event);


};
