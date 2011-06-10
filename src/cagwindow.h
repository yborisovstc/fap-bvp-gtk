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
	CagWindow(GType aType, const string& aName);
	virtual ~CagWindow();
	void SetTitle(const string& aTitle);
	void Fullscrean();
	void Maximize();
    private:
	static void Init();
};

class CagScrolledWindow: public CagBin
{
    public:
	CagScrolledWindow(const string& aName);
	void AddWithViewport(CagWidget* aChild);
};


#endif 
