#include "main.h"

wxIMPLEMENT_APP(main);


bool main::OnInit()
{
	wxInitAllImageHandlers();
	frame = new myFrame();
	frame->Show();



	return true;
}
