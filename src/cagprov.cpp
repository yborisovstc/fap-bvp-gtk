#include "cagproxy.h"
#include "cagprov.h"


CagProvider::CagProvider(GtkWidget* aWnd): iWnd(aWnd)
{
}

CagProvider::~CagProvider()
{
}

MAE_Opv* CagProvider::CreateViewProxy()
{
    return new CagProxy(iWnd);
}