
#include "wx/wx.h"
#include "wx/tokenzr.h"

#include <string>
#include <vector>
#include <cwchar>

using std::vector;
using std::string;

class Sort
{

public:
	static bool compareNameDesc(wxArrayString& p1, wxArrayString& p2)
	{
		return (p1[0].Lower() < p2[0].Lower());
	}
	static bool compareNameAsc(wxArrayString& p1, wxArrayString& p2)
	{
		return (p1[0].Lower() > p2[0].Lower());
	}

	static bool comparePathDesc(wxArrayString& p1, wxArrayString& p2)
	{
		return (p1[1].Lower() < p2[1].Lower());
	}
	static bool comparePathAsc(wxArrayString& p1, wxArrayString& p2)
	{
		return (p1[1].Lower() > p2[1].Lower());
	}

	static bool compareVersionDesc(wxArrayString& p1, wxArrayString& p2)
	{
		return (p1[2].Lower() < p2[2].Lower());
	}
	static bool compareVersionAsc(wxArrayString& p1, wxArrayString& p2)
	{
		return (p1[2].Lower() > p2[2].Lower());
	}
	static bool compareDateDesc(wxArrayString& p1, wxArrayString& p2)
	{
		return (p1[3].Lower() > p2[3].Lower());
	}
	static bool compareDateAsc(wxArrayString& p1, wxArrayString& p2)
	{
		return (p1[3].Lower() < p2[3].Lower());
	}

};
