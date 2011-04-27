#ifndef __FAPBVP_GTK_SYS_H
#define __FAPBVP_GTK_SYS_H

#include <fapbase.h>
#include "caglayout.h"
#include "cagbutton.h"
#include "capcp.h"
#include "capcomp.h"
#include "capstate.h"

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

class CapCterm;
class MagSysObserver;
class CapSys: public CagLayout, public MCapCompObserver, public MCapCpPairRes, public MCapStateObserver
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
	// From MCapCompObserver
	virtual void OnCompCpPairToggled(CapComp* aComp, CapCtermPair* aPair);
	virtual void OnCompNameClicked(CapComp* aComp);
	// From MCapStateObserver
	virtual void OnStateCpPairToggled(CapState* aComp, CapCtermPair* aPair);
    private:
	CapComp* Comp(CagWidget* aWidget);
	// From MCapCpPairRes
	virtual CapCtermPair* GetCpPair(CapCtermPair* aPair);
    private:
	class CpPairObs: public MCagToggleButtonObs
    {
	public:
	    CpPairObs(CapSys& aSupw): iSupw(aSupw) {};
	    virtual void OnToggled(CagToggleButton* aBtn);
	private:
	    CapSys& iSupw;
    };
	friend class CpPairObs;
    private:
	CapSysHead* iHead; // Not owned
	CAE_Object::Ctrl& iSys;
	map<CAE_StateBase*, CapState*> iStates; 
	map<CAE_Object*, CapComp*> iComps; // Components
	map<CAE_ConnPointBase*, CapCp*> iOutputs;
	map<CAE_ConnPointBase*, CapCp*> iInputs;
	MagSysObserver* iObserver;
	CpPairObs iCpPairObs;
	// Size requested parameters
	GtkRequisition iInpReq;
};

#endif 
