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
	virtual TBool OnDragDrop(GdkDragContext *drag_context, gint x, gint y, guint time);
	virtual void OnDragDataReceived(GdkDragContext *drag_context, gint x, gint y, GtkSelectionData *data, guint info, guint time);
	virtual TBool OnButtonPress(GdkEventButton* aEvent);
	// From MCapCompObserver
	virtual void OnCompCpPairToggled(CapComp* aComp, CapCtermPair* aPair);
	virtual void OnCompNameClicked(CapComp* aComp);
	virtual void OnCompParentClicked(CapComp* aComp);
	virtual void OnCompNameChanged(CapComp* aComp, const string& aName);
	// From MCapStateObserver
	virtual void OnStateCpPairToggled(CapState* aState, CapCtermPair* aPair);
	virtual void OnStateNameChanged(CapState* aState, const string& aName);
	virtual void OnStateDeleteRequested(CapState* aState);
	virtual void OnStateAddingInput(CapState* aState);
	virtual void OnStateInpRenamed(CapState* aState, CapCp* aCp, const string& aName);
	virtual void OnStateTransUpdated(CapState* aState, const string& aTrans);
	// From MCapCpObserver
	virtual void OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair);
	virtual void OnLabelRenamed(CapCp* aCp, const string& aName);
    protected:
	void Construct();
    private:
	CapComp* Comp(CagWidget* aWidget);
	void ActivateConn(CapCtermPair* aPair);
	void AddComponent();
	void AddState();
	void AddStateInp(CapState* aState);
	void DeleteState(CapState* aState);
	void ChangeCompName(CapComp* aComp, const string& aName);
	void ChangeStateName(CapState* aState, const string& aName);
	void ChangeStateTrans(CapState* aState, const string& aTrans);
	void RenameStateInp(CapState* aState, CapCp* aCp, const string& aName);
	void Refresh();
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
    public:
	CapSysHead* iHead; // Not owned
    private:
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
