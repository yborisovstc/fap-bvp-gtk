#include "cagbin.h"


CagBin::CagBin(const string& aName): CagContainer(GTK_TYPE_BIN, aName)
{
}

CagBin::CagBin(GType aType, const string& aName): CagContainer(aType, aName)
{
}

CagBin::CagBin(GtkWidget* aWidget, const string& aName, TBool aOwned): CagContainer(aWidget, aName, aOwned)
{
}

CagBin::~CagBin()
{
}


CagEventBox::CagEventBox(const string& aName): CagBin(GTK_TYPE_EVENT_BOX, aName)
{
}


CagItem::CagItem(const string& aName): CagBin(GTK_TYPE_ITEM, aName)
{
}

CagAlignment::CagAlignment(const string& aName, gfloat xalign, gfloat yalign, gfloat xscale, gfloat yscale): CagBin(GTK_TYPE_ALIGNMENT, aName) 
{
    gtk_alignment_set(GTK_ALIGNMENT(iWidget), xalign, yalign, xscale, yscale);
}

