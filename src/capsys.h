#ifndef __FAPBVP_GTK_SYS_H
#define __FAPBVP_GTK_SYS_H

#include <fapbase.h>
#include "caglayout.h"

class CapSysHead: public CagLayout
{
    public:
	CapSysHead(const string& aName, CAE_Object::Ctrl& aSys);
	virtual ~CapSysHead();
    private:
	virtual void OnExpose(GdkEventExpose* aEvent);
	virtual TBool OnButtonPress(GdkEventButton* aEvent);
	virtual TBool OnButtonRelease(GdkEventButton* aEvent);
	virtual void OnSizeAllocate(GtkAllocation* aAllocation);
	virtual void OnSizeRequest(GtkRequisition* aRequisition);
	virtual void OnMotion(GdkEventMotion *aEvent);
	virtual void OnEnter(GdkEventCrossing *aEvent);
	virtual void OnLeave(GdkEventCrossing *aEvent);
	virtual void OnStateChanged(GtkStateType state);
    private:
	CAE_Object::Ctrl& iSys;
};

class MagSysObserver
{
    public:
	virtual void OnCompSelected(CAE_Object* aComp) = 0;
	virtual void OnHeadSelected() = 0;
};

class CapComp;
class CapSys: public CagLayout
{
    public:
	CapSys(const string& aName, CAE_Object::Ctrl& aSys, MagSysObserver* aObserver);
	virtual ~CapSys();
    private:
	virtual void OnExpose(GdkEventExpose* aEvent);
	virtual TBool OnButtonPress(GdkEventButton* aEvent);
	virtual TBool OnButtonRelease(GdkEventButton* aEvent);
	virtual void OnSizeAllocate(GtkAllocation* aAllocation);
	virtual void OnSizeRequest(GtkRequisition* aRequisition);
	virtual void OnMotion(GdkEventMotion *aEvent);
	virtual void OnEnter(GdkEventCrossing *aEvent);
	virtual void OnLeave(GdkEventCrossing *aEvent);
	virtual void OnStateChanged(GtkStateType state);
	virtual void OnChildStateChanged(CagWidget* aChild, GtkStateType aPrevState);
    private:
	CapComp* Comp(CagWidget* aWidget);
    private:
	CapSysHead* iHead; // Not owned
	CAE_Object::Ctrl& iSys;
	map<CAE_Object*, CapComp*> iComps;
	MagSysObserver* iObserver;
};

#endif 
