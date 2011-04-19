#ifndef __FAPBVP_GTK_WINDOW_H
#define __FAPBVP_GTK_WINDOW_H

#include "cagbin.h"

class CagWindow: public CagBin
{
    public:
	enum TWndType {
	    EWndt_Toplevel,
	    EWndt_Popup
	};
    public:
	CagWindow(const string& aName);
	CagWindow(GtkWidget* aWidget, TBool aOwned = ETrue);
	virtual ~CagWindow();
    private:
	static void Init();
    protected:
	GtkWidget* iWidget;
};

#endif 
