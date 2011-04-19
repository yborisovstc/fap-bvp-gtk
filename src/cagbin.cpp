#include "cagbin.h"


CagBin::CagBin(const string& aName): CagContainer(GTK_TYPE_BIN, aName)
{
}

CagBin::CagBin(GtkWidget* aWidget, const string& aName, TBool aOwned): CagContainer(aWidget, aName, aOwned)
{
}

CagBin::~CagBin()
{
}
