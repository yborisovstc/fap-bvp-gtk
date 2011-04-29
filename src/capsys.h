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

class MCapSysObserver
{
    public:
	virtual void OnCompSelected(CAE_Object* aComp) = 0;
	virtual void OnSystSelected(const string& aName) = 0;
};

class CapCterm;
class MagSysObserver;
class CapSys: public CagLayout, public MCapCompObserver, public MCapCpPairRes, public MCapStateObserver,
    public MCapCpObserver
{
    public:
	CapSys(const string& aName, CAE_Object::Ctrl& aSys, MCapSysObserver* aObserver);
	virtual ~CapSys();
    private:
	virtual void OnExpose(GdkEventExpose* aEvent);
	virtual void OnSizeAllocate(GtkAllocation* aAllocation);
	virtual void OnSizeRequest(GtkRequisition* aRequisition);
	// From MCapCompObserver
	virtual void OnCompCpPairToggled(CapComp* aComp, CapCtermPair* aPair);
	virtual void OnCompNameClicked(CapComp* aComp);
	virtual void OnCompParentClicked(CapComp* aComp);
	// From MCapStateObserver
	virtual void OnStateCpPairToggled(CapState* aComp, CapCtermPair* aPair);
	// From MCapCpObserver
	virtual void OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair);
    private:
	CapComp* Comp(CagWidget* aWidget);
	void ActivateConn(CapCtermPair* aPair);
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
	MCapSysObserver* iObserver;
	CpPairObs iCpPairObs;
	// Size requested parameters
	GtkRequisition iInpReq;
};

#endif 
