#ifndef __FAPBVP_GTK_PROXY_H
#define __FAPBVP_GTK_PROXY_H

#include <fapbase.h>
#include <fapfact.h>
#include <gtk/gtk.h>
#include "capopwnd.h"
#include "mainwndiface.h"


class CapSys;
class CapOpWnd;
class CagWindow;
class CagProxy: public MAE_Opv, public MOpWndObserver
{
    public:
	CagProxy(CagWindow* aWnd);
	virtual ~CagProxy();
	void TurnToComp(CAE_Object* aComp);
	// From MAE_Opv
	virtual void Destroy();
	virtual void SetRoot(CAE_Object::Ctrl* aObj);
	virtual void SetObj(CAE_Object::Ctrl* aObj);
	virtual void UnsetObj(CAE_Object::Ctrl* aObj);
	virtual void UnsetRoot(CAE_Object* aObj);
	// From MOpWndObserver
	virtual void OnTurnToComp(CAE_Object* aComp);
	virtual void OnTurnToSyst(const string& aName);
	virtual void OnCmd(TCmd aCmd);
	virtual TBool OnCmdUpdateRequest(TCmd aCmd);
    private:
	CAE_Object::Ctrl* iRoot;
	CAE_Object::Ctrl* iSys;
	CapOpWnd* iWindow;
	vector<CAE_Object*> iHistory;
	vector<CAE_Object*>::iterator iHistItr;
	MOpMainWnd* iMainWnd;
};

#endif 
