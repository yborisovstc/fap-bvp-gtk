
#include "capopwnd.h"

CapOpWndToolbar::CapOpWndToolbar(const string& aName): CagToolBar(aName)
{
    // Button "Back"
    iBtnBack = new CagToolButton("BntBack", GTK_STOCK_GO_BACK);
    Insert(iBtnBack, 0);
    iBtnBack->Show();
}




CapOpWnd::CapOpWnd(const string& aName): CagWindow(aName)
{
    Construct();
}

CapOpWnd::CapOpWnd(GtkWidget* aWidget, TBool aOwned): CagWindow(aWidget, aOwned)
{
    Construct();
}

CapOpWnd::~CapOpWnd()
{
}

void CapOpWnd::Construct()
{
    iVbox = new CagVBox("Vbox");
    Add(iVbox);
    iVbox->Show();
    // Toolbar
    iToolbar = new CapOpWndToolbar("Toolbar");
//    Add(iToolbar);
    iVbox->PackStart(iToolbar, false, false, 1);
    iToolbar->Show();
}


void CapOpWnd::AddView(CagWidget* aView)
{
    iVbox->PackStart(aView, false, false, 1);
}

