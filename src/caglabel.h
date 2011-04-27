#ifndef __FAPBVP_GTK_LABEL_H
#define __FAPBVP_GTK_LABEL_H

#include "cagmisc.h"
#include "cagbin.h"

// Standard lable. Note that it is wrapper for GtkLabel that is no-window GTK widget
// So the widget doesn't generate events
class CagLabel: public CagMisc 
{
    public:
	CagLabel(const string& aName);
	virtual ~CagLabel();
	void SetText(const string& aText);
};

// Label with events support
class CagELabel: public CagEventBox 
{
    public:
	CagELabel(const string& aName);
	void SetText(const string& aText);
    private:
	GtkLabel* Label();
};


#endif 
