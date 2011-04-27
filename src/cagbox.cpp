#include "cagbox.h"


CagBox::CagBox(const string& aName): CagContainer(GTK_TYPE_BOX, aName)
{
}

CagBox::CagBox(GType aType, const string& aName): CagContainer(aType, aName)
{
}

void CagBox::PackStart(CagWidget *child, gboolean expand, gboolean fill, guint padding)
{
    Add(child, EFalse);
    gtk_box_pack_start(GTK_BOX(iWidget), child->iWidget, expand, fill, padding);
}




CagHBox::CagHBox(const string& aName): CagBox(GTK_TYPE_HBOX, aName)
{
}


CagVBox::CagVBox(const string& aName): CagBox(GTK_TYPE_VBOX, aName)
{
}

