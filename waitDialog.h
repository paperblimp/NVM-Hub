#pragma once

#include "wx/wx.h"
#include "wx/activityindicator.h"

class waitDialog : public wxDialog
{
public:
	waitDialog(wxWindow* parent, wxString& message);
	~waitDialog();

	wxActivityIndicator* loading = nullptr;
};

