#pragma once

#include "wx/wx.h"
#include "frame.h"

class main : public wxApp
{
public:
	virtual bool OnInit();
public:

	myFrame* frame = nullptr;
};



