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
