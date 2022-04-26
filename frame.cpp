#include "frame.h"


myFrame::myFrame() : wxFrame(nullptr, wxID_ANY, "Hub", wxPoint(300, 300), wxSize(1000, 500))
{
	this->SetMinSize(wxSize(1000, 500));
	this->renderWindow();
	this->SetDoubleBuffered(true);
	this->Centre();

	wxFileName iconPath("Images","NodejsIcon3.png");
	SetIcon(wxIcon(iconPath.GetFullPath(),wxBitmapType::wxBITMAP_TYPE_PNG));

}


void myFrame::renderWindow()
{

	mainPanel = new wxPanel(this);
	///Top area


	wxPanel* topPanel = new wxPanel(mainPanel);
	topPanel->SetBackgroundColour(*wxWHITE);

	wxFileName titleImagePath("Images", "NodejsIcon2.png");
	wxImage titleImage(titleImagePath.GetFullPath(), wxBITMAP_TYPE_PNG);
	titleImage.Rescale(93, 25, wxIMAGE_QUALITY_HIGH);
	wxStaticBitmap* title = new wxStaticBitmap(topPanel, wxID_ANY, wxBitmap(titleImage));
	
	wxButton* configButton = new wxButton(topPanel, wxID_ANY,"",wxDefaultPosition,wxSize(40,40));
	wxFileName buttonImagePath("Images", "Settings.png");
	wxImage buttonImage(buttonImagePath.GetFullPath(), wxBITMAP_TYPE_PNG);
	buttonImage.Rescale(20, 20, wxIMAGE_QUALITY_HIGH);
	configButton->SetBitmap(wxBitmap(buttonImage));
	configButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &myFrame::OnSettingsPressed, this);

	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);

	topSizer->Add(title,0,  wxALL | wxALIGN_CENTER_VERTICAL,15);
	topSizer->AddStretchSpacer(1);
	topSizer->Add(configButton,0, wxRIGHT | wxALIGN_CENTER_VERTICAL,20);
	topPanel->SetSizer(topSizer);

	///Main area
	images = new wxImageList(40, 40);

	wxFileName projectsFile("Images", "Projects.png");
	wxFileName installsFile("Images", "Installs.png");
	wxImage projectsImage(projectsFile.GetFullPath(), wxBITMAP_TYPE_PNG);
	wxImage installsImage(installsFile.GetFullPath(), wxBITMAP_TYPE_PNG);
	projectsImage.Rescale(40, 40, wxIMAGE_QUALITY_HIGH);
	installsImage.Rescale(40, 40, wxIMAGE_QUALITY_HIGH);
	const int projectsIndex = images->Add(wxBitmap(projectsImage));
	const int installsIndex = images->Add(wxBitmap(installsImage));

	book = new wxListbook(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_LEFT);
	book->UseBackgroundColour();
	book->SetImageList(images);


	projectsTab = new projects(this,book);
	installsTab = new installs(this,book);

	book->AddPage(projectsTab->panel,wxT("Projects"),true, projectsIndex);
	book->AddPage(installsTab->panel,wxT("Installs"),false, installsIndex);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticLine* separator = new wxStaticLine(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(1000,1), wxHORIZONTAL);

	mainSizer->Add(topPanel,0.15,wxEXPAND);
	mainSizer->Add(separator,0, wxDOWN | wxEXPAND,5);
	mainSizer->Add(book,1,wxALL | wxEXPAND,10);

	mainPanel->SetSizer(mainSizer);
	mainSizer->SetSizeHints(mainPanel);
	mainSizer->Layout();


}

void myFrame::OnSettingsPressed(wxCommandEvent& event)
{
	wxString title("Settings");
	settingsDialog settings(this, title);
	settings.ShowModal();
}

myFrame::~myFrame()
{
	delete projectsTab;
	delete installsTab;
	delete images;
	this->Close();
	this->Destroy();
}