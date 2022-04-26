#include "waitDialog.h"


waitDialog::waitDialog(wxWindow* parent, wxString& message)
	:wxDialog(parent, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0)
{
	wxStaticText* waitTitle = new wxStaticText(this, wxID_ANY, message);
	wxActivityIndicator* loading = new wxActivityIndicator(this);
	loading->Start();

	wxBoxSizer* loadingSizer = new wxBoxSizer(wxVERTICAL);
	loadingSizer->Add(waitTitle, 1, wxALL, 20);
	loadingSizer->Add(loading, 1, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxDOWN, 10);

	SetSizer(loadingSizer);
	loadingSizer->Fit(this);
	CenterOnScreen();
}

waitDialog::~waitDialog()
{

}
