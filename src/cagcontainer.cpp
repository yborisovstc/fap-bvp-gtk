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

void CagContainer::Add(CagWidget* aChild, TBool aAddToGtkCont)
{
    _FAP_ASSERT(iChilds.count(aChild->Name()) == 0);
    iChilds[aChild->Name()] = aChild;
    iChildsWd[aChild->iWidget] = aChild;
    if (aAddToGtkCont)
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

CagWidget* CagContainer::GetWidget(GtkWidget* aGtkWidget, CagWidget* aRequester)
{
    CagWidget* res = NULL;
    if (iWidget == aGtkWidget) {
	res = this;
    }
    else if (iChildsWd.count(aGtkWidget) != 0) {
	res = iChildsWd[aGtkWidget];
    }
    else {
	for (map<GtkWidget*, CagWidget*>::iterator it = iChildsWd.begin(); it != iChildsWd.end() && res == NULL; it++) {
	    if (it->second != aRequester) {
		res = it->second->GetWidget(aGtkWidget, this);
	    }
	}
	if (res == NULL && iParent != NULL && iParent != aRequester) {
	    res = iParent->GetWidget(aGtkWidget, this);	
	}
    }
    return res;
}

void CagContainer::SetBorderWidth(int aWidth)
{
    gtk_container_set_border_width(GTK_CONTAINER(iWidget), aWidth);
}
