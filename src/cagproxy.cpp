#include "cagproxy.h"
#include "cagwindow.h"
#include "capsys.h"
#include "capopwnd.h"



CagProxy::CagProxy(GtkWidget* aWnd): iSys(NULL), iRoot(NULL)
{
    iWindow = new CapOpWnd(aWnd, EFalse);
    iWindow->SetObserver(this);
    // Load resource file
    gtk_rc_parse("cag_proxy.rc");
}

CagProxy::~CagProxy()
{
}

void CagProxy::Destroy()
{
    delete this;
}

void CagProxy::SetRoot(CAE_Object::Ctrl* aObj)
{
    _FAP_ASSERT(iRoot == NULL);
    iRoot = aObj;
}

void CagProxy::SetObj(CAE_Object::Ctrl* aObj)
{
    _FAP_ASSERT(iSys == NULL);
    iSys = aObj;
    iWindow->SetSys(iSys);
}

void CagProxy::UnsetObj(CAE_Object::Ctrl* aObj)
{
    _FAP_ASSERT(iSys == aObj);
    iWindow->UnsetSys(iSys);
    iSys = NULL;
}

void CagProxy::OnTurnToComp(CAE_Object* aComp)
{
    // Move cursor to component
    iSys->Object().RemoveBaseViewProxy(this);
    aComp->SetBaseViewProxy(this);
}

void CagProxy::OnTurnToSyst(const string& aName)
{
}


void CagProxy::OnCmd(TCmd aCmd)
{
}

