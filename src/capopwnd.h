#ifndef __FAPBVP_GTK_OPWND_H
#define __FAPBVP_GTK_OPWND_H

#include "cagwindow.h"
#include "cagtoolbar.h"
#include "cagbox.h"

class CapOpWndToolbar: public CagToolBar
{
    public:
	CapOpWndToolbar(const string& aName);
    private:
	CagToolButton* iBtnBack;
};


class CapOpWnd: public CagWindow
{
    public:
	CapOpWnd(const string& aName);
	CapOpWnd(GtkWidget* aWidget, TBool aOwned);
	virtual ~CapOpWnd();
	void AddView(CagWidget* aView);
    protected:
	void Construct();
    private:
	CagVBox* iVbox;
	CapOpWndToolbar* iToolbar;
};

#endif 
