#include "cagcontainer.h"


CagContainer::CagContainer(const string& aName): CagWidget(GTK_TYPE_CONTAINER, aName)
{
}

CagContainer::CagContainer(GType aType, const string& aName): CagWidget(aType, aName)
{
}

CagContainer::CagContainer(GtkWidget* aWidget, const string& aName, TBool aOwned): CagWidget(aWidget, aName, aOwned)
{
}

CagContainer::~CagContainer()
{
}

void CagContainer::Add(CagWidget* aChild)
{
    _FAP_ASSERT(iChilds.count(aChild->Name()) == 0);
    iChilds[aChild->Name()] = aChild;
    gtk_container_add(GTK_CONTAINER(iWidget), aChild->iWidget);
    aChild->SetParent(this);
}

void CagContainer::Remove(CagWidget* aChild)
{
    _FAP_ASSERT(iChilds.count(aChild->Name()) > 0);
    iChilds.erase(aChild->Name());
    gtk_container_remove(GTK_CONTAINER(iWidget), aChild->iWidget);
    aChild->ResetParent(this);
}
