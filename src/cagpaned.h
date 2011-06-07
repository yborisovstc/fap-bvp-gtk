#ifndef __FAPBVP_GTK_PANED_H
#define __FAPBVP_GTK_PANED_H

#include "cagcontainer.h"

class CagPaned: public CagContainer 
{
    public:
	CagPaned(GType aType, const string& aName);
    public:
	void Add1(CagWidget* aChild);
	void Add2(CagWidget* aChild);
};

class CagVPaned: public CagPaned
{
    public:
	CagVPaned(const string& aName);
};


#endif 
