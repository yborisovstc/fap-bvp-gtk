#include "cagwindow.h"
#include <map>

static map<CagWindow::TWndType, GtkWindowType> KWndtypeToGtk;

void CagWindow::Init()
{
    if (KWndtypeToGtk.empty()) {
	KWndtypeToGtk[EWndt_Popup] = GTK_WINDOW_POPUP;
	KWndtypeToGtk[EWndt_Toplevel] = GTK_WINDOW_TOPLEVEL;
    };
}

CagWindow::CagWindow(const string& aName): 
    CagBin((Init(), gtk_window_new(KWndtypeToGtk[EWndt_Toplevel])), aName)
{
}

CagWindow::CagWindow(GtkWidget* aWidget, TBool aOwned): CagBin(aWidget, "", aOwned)
{
}

CagWindow::~CagWindow()
{
}
