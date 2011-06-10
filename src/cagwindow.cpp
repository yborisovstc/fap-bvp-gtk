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

CagWindow::CagWindow(GType aType, const string& aName): CagBin(aType, aName)
{
}

CagWindow::~CagWindow()
{
}

void CagWindow::SetTitle(const string& aTitle)
{
    gtk_window_set_title(GTK_WINDOW(iWidget), aTitle.c_str());
}

void CagWindow::Fullscrean()
{
    gtk_window_fullscreen(GTK_WINDOW(iWidget));
}

void CagWindow::Maximize()
{
    gtk_window_maximize(GTK_WINDOW(iWidget));
}




CagScrolledWindow::CagScrolledWindow(const string& aName): CagBin(GTK_TYPE_SCROLLED_WINDOW, aName)
{
}

void CagScrolledWindow::AddWithViewport(CagWidget* aChild)
{
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(iWidget), aChild->iWidget);
}
