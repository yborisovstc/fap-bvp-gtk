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
    if (iHistory.empty()) {
	iHistory.push_back(&(iSys->Object()));
	iHistItr = iHistory.end() - 1;
    }
    iWindow->SetSys(iSys);
}

void CagProxy::UnsetObj(CAE_Object::Ctrl* aObj)
{
    _FAP_ASSERT(iSys == aObj);
    iWindow->UnsetSys(iSys);
    iSys = NULL;
}

void CagProxy::TurnToComp(CAE_Object* aComp)
{
    // Move cursor to component
    iSys->Object().RemoveBaseViewProxy(this);
    aComp->SetBaseViewProxy(this);
}

void CagProxy::OnTurnToComp(CAE_Object* aComp)
{
    // New turn, update history
    iHistory.erase(iHistItr + 1, iHistory.end());
    iHistory.push_back(aComp);
    iHistItr = iHistory.end() - 1;
    TurnToComp(aComp);
}

void CagProxy::OnTurnToSyst(const string& aName)
{
    // TODO [YB] To use full name for parent
    CAE_EBase* elem = iSys->FindByName(aName.c_str());
    _FAP_ASSERT(elem != NULL);
    CAE_Object* syst = elem->GetFbObj(syst);
    _FAP_ASSERT(syst != NULL);
    OnTurnToComp(syst);
}


void CagProxy::OnCmd(TCmd aCmd)
{
    if (aCmd == ECmd_Up) {
	// Going UP to parent on cursor
	OnTurnToComp(iSys->Mangr());
    }
    else if (aCmd == ECmd_Back) {
	// Going back with history
	CAE_Object* syst = *(--iHistItr);
	TurnToComp(syst);
    }
}

TBool CagProxy::OnCmdUpdateRequest(TCmd aCmd)
{
    if (aCmd == ECmd_Back) {
	return (!iHistory.empty() && iHistItr != iHistory.begin());
    }
    else if (aCmd == ECmd_Up) {
	return (iSys != iRoot);
    }
}

