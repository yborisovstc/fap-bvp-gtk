#include "cagproxy.h"
#include "cagprov.h"


CagProvider::CagProvider(CagWindow* aWnd): CAE_ProviderBase("cagprov"), iWnd(aWnd)
{
}

CagProvider::~CagProvider()
{
}

MAE_Opv* CagProvider::CreateViewProxy()
{
    return new CagProxy(iWnd);
}
