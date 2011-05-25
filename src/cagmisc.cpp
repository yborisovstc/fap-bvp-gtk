
#include "cagmisc.h"


CagMisc::CagMisc(GType aType, const string& aName): CagWidget(aType, aName)
{
}

CagMisc::~CagMisc()
{
}

void CagMisc::SetPadding(TInt aX, TInt aY)
{
    gtk_misc_set_padding(GTK_MISC(iWidget), aX, aY);
}

