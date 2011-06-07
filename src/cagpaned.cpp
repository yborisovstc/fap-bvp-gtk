
#include "cagpaned.h"


CagPaned::CagPaned(GType aType, const string& aName): CagContainer(aType, aName)
{
}

void CagPaned::Add1(CagWidget* aChild)
{
    gtk_paned_add1(GTK_PANED(iWidget), aChild->iWidget);
    Add(aChild);
}

void CagPaned::Add2(CagWidget* aChild)
{
    gtk_paned_add2(GTK_PANED(iWidget), aChild->iWidget);
    Add(aChild);
}


CagVPaned::CagVPaned(const string& aName): CagPaned(GTK_TYPE_VPANED, aName)
{
}

