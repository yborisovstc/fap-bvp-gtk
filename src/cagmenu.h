#ifndef __FAPBVP_GTK_MENU_H
#define __FAPBVP_GTK_MENU_H

#include "cagcontainer.h"

class CagMenuShell: public CagContainer
{
    public:
	CagMenuShell(const string& aName);
	void Append(CagWidget* aChild);
    protected:
	CagMenuShell(GType aType, const string& aName);
};

class CagMenuBar: public CagMenuShell
{
    public:
	CagMenuBar(const string& aName);
};

#endif
