#ifndef __FAPBVP_GTK_MISC_H
#define __FAPBVP_GTK_MISC_H

#include "widgets.h"

class CagMisc: public CagWidget 
{
    public:
	CagMisc(GType aType, const string& aName);
	virtual ~CagMisc();
    public:
	void SetPadding(TInt aX, TInt aY);
};

#endif 
