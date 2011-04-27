#ifndef __FAPBVP_GTK_TOOLBAR_H
#define __FAPBVP_GTK_TOOLBAR_H

#include "cagcontainer.h"
#include "cagbin.h"

class CagToolItem;
class CagToolBar: public CagContainer
{
    public:
	CagToolBar(const string& aName);
	void Insert(CagToolItem* aItem, int aPos);
};

class CagToolItem: public CagBin
{
    public:
	CagToolItem(const string& aName);
    protected:
	CagToolItem(GType aType, const string& aName);
};


class CagToolButton: public CagToolItem
{
    public:
	CagToolButton(const string& aName);
	CagToolButton(const string& aName, const string& aStockId);
	void SetStockId(const string& aStockId);
    protected:
	CagToolButton(GType aType, const string& aName);
};


#endif
