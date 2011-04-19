#include "cagproxy.h"
#include "cagwindow.h"
#include "capsys.h"



CagProxy::CagProxy(GtkWidget* aWnd): iSysWidget(NULL)
{
    iWindow = new CagWindow(aWnd, EFalse);
}

CagProxy::~CagProxy()
{
}

void CagProxy::Destroy()
{
    delete this;
}

void CagProxy::SetObj(CAE_Object::Ctrl* aObj)
{
    iSys = aObj;
    _FAP_ASSERT(iSysWidget == NULL);
    iSysWidget = new CapSys("System", *iSys, this);
    iWindow->Add(iSysWidget);
    iSysWidget->Show();
}

void CagProxy::OnCompSelected(CAE_Object* aComp)
{
    // Move cursor to component
    iWindow->Remove(iSysWidget);
    delete iSysWidget;
    iSysWidget = NULL;
    iSys->Object().MoveBaseViewProxy(this, aComp);
}

void CagProxy::OnHeadSelected()
{
    // Move cursor to upper level
    iWindow->Remove(iSysWidget);
    delete iSysWidget;
    iSysWidget = NULL;
    iSys->Object().MoveBaseViewProxyToMan(this);
}





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
