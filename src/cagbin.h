#ifndef __FAPBVP_GTK_BIN_H
#define __FAPBVP_GTK_BIN_H

#include "cagcontainer.h"

class CagBin: public CagContainer 
{
    public:
	CagBin(const string& aName);
	CagBin(GType aType, const string& aName);
	virtual ~CagBin();
    protected:
	CagBin(GtkWidget* aWidget, const string& aName, TBool aOwned = ETrue);
};

class CagEventBox: public CagBin 
{
    public:
	CagEventBox(const string& aName);
};

#endif 
