
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

void CagPaned::Pack1(CagWidget* aChild, TBool aResize, TBool aCanShrink)
{
    gtk_paned_pack1(GTK_PANED(iWidget), aChild->iWidget, aResize, aCanShrink);
}

void CagPaned::Pack2(CagWidget* aChild, TBool aResize, TBool aCanShrink)
{
    gtk_paned_pack2(GTK_PANED(iWidget), aChild->iWidget, aResize, aCanShrink);
}


CagVPaned::CagVPaned(const string& aName): CagPaned(GTK_TYPE_VPANED, aName)
{
}

