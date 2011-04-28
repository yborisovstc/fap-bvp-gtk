#ifndef __FAPBVP_GTK_PROXY_H
#define __FAPBVP_GTK_PROXY_H

#include <fapbase.h>
#include <fapfact.h>
#include <gtk/gtk.h>
#include "capopwnd.h"


class CapSys;
class CapOpWnd;
class CagProxy: public MAE_Opv, public MOpWndObserver
{
    public:
	CagProxy(GtkWidget* aWnd);
	virtual ~CagProxy();
	// From MAE_Opv
	virtual void Destroy();
	virtual void SetRoot(CAE_Object::Ctrl* aObj);
	virtual void SetObj(CAE_Object::Ctrl* aObj);
	virtual void UnsetObj(CAE_Object::Ctrl* aObj);
	// From MObjectProvider
	virtual void OnTurnToComp(CAE_Object* aComp);
	virtual void OnTurnToSyst(const string& aName);
	// From MOpWndObserver
	virtual void OnCmd(TCmd aCmd);
    private:
	CAE_Object::Ctrl* iRoot;
	CAE_Object::Ctrl* iSys;
	CapOpWnd* iWindow;
};

#endif 
