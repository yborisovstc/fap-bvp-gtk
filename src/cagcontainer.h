#ifndef __FAPBVP_GTK_CONTAINER_H
#define __FAPBVP_GTK_CONTAINER_H

#include "widgets.h"
#include <map>

class CagContainer: public CagWidget
{
    public:
	CagContainer(const string& aName);
	CagContainer(GType aType, const string& aName);
	virtual ~CagContainer();
	void Add(CagWidget* aChild);
	void Remove(CagWidget* aChild);
	// From MWidgetRes
	virtual CagWidget* GetWidget(GtkWidget* aGtkWidget, CagWidget* aRequester = NULL);
    protected:
	CagContainer(GtkWidget* aWidget, const string& aName, TBool aOwned = ETrue);
    protected:
	map<string, CagWidget*> iChilds;
	map<GtkWidget*, CagWidget*> iChildsWd;
};

#endif 
