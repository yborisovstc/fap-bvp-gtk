#ifndef __FAPBVP_GTK_BOX_H
#define __FAPBVP_GTK_BOX_H

#include "cagcontainer.h"

class CagBox: public CagContainer 
{
    public:
	CagBox(const string& aName);
	void PackStart(CagWidget *child, gboolean expand, gboolean fill, guint padding);
    protected:
	CagBox(GType aType, const string& aName);
};


class CagHBox: public CagBox 
{
    public:
	CagHBox(const string& aName);
};

class CagVBox: public CagBox 
{
    public:
	CagVBox(const string& aName);
};


#endif 
