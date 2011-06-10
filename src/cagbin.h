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

class CagItem: public CagBin
{
    public:
	CagItem(const string& aName);
};

class CagAlignment: public CagBin
{
    public: 
	CagAlignment(const string& aName, gfloat xalign, gfloat yalign, gfloat xscale, gfloat yscale);
};


#endif 
