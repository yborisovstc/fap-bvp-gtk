#ifndef __FAPBVP_GTK_LAYOUT_H
#define __FAPBVP_GTK_LAYOUT_H

#include "cagcontainer.h"



class CagLayout: public CagContainer
{
    public:
	CagLayout(const string& aName);
	virtual ~CagLayout();
	GdkWindow* BinWnd();
};

#endif 
