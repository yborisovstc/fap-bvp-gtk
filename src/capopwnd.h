#ifndef __FAPBVP_GTK_OPWND_H
#define __FAPBVP_GTK_OPWND_H

#include "cagwindow.h"
#include "cagtoolbar.h"
#include "cagbox.h"
#include "capsys.h"

class CapOpWndToolbar: public CagToolBar
{
    public:
	CapOpWndToolbar(const string& aName);
    private:
	CagToolButton* iBtnBack;
	CagToolButton* iBtnUp;
};

class MOpWndObserver
{
    public:
	enum TCmd
	{
	    ECmd_Up
	};
    public:
	virtual void OnTurnToComp(CAE_Object* aComp) = 0;
	virtual void OnTurnToSyst(const string& aName) = 0;
	virtual void OnCmd(TCmd aCmd) = 0;
};

class CapOpWnd: public CagWindow, public MCapSysObserver, public MagToolButtonObserver
{
    public:
	static inline const char* Type() { return "CapOpWnd";} ; 
	CapOpWnd(const string& aName);
	CapOpWnd(GtkWidget* aWidget, TBool aOwned);
	virtual ~CapOpWnd();
	void AddView(CagWidget* aView);
	void RemoveView(CagWidget* aView);
	void SetSys(CAE_Object::Ctrl* aObj);
	void UnsetSys(CAE_Object::Ctrl* aObj);
	void SetObserver(MOpWndObserver* aObs);
	// From CAE_Base
	virtual void *DoGetObj(const char *aName);
    protected:
	void Construct();
	// From MCapSysObserver	
	virtual void OnCompSelected(CAE_Object* aComp);
	virtual void OnSystSelected(const string& aName);
	// From MagToolButtonObserver
	virtual void OnClicked(CagToolButton* aBtn);
    private:
	CagVBox* iVbox;
	CapOpWndToolbar* iToolbar;
	CapSys* iSysWidget;
	MOpWndObserver* iObs;
};

#endif 
