#ifndef __FAPBVP_GTK_SYS_H
#define __FAPBVP_GTK_SYS_H

#include <fapbase.h>
#include "caglayout.h"
#include "cagbutton.h"
#include "capcp.h"
#include "capcomp.h"
#include "capstate.h"

class CapTranHead: public CagHBox
{
    public:
	CapTranHead(const string& aName);
    private:
	CagLabel* iLabel;
};

class CapTran: public CagLayout
{
    public:
	CapTran(const string& aName, const string& aTranData);
    private:
	virtual void OnExpose(GdkEventExpose* aEvent);
	virtual void OnSizeAllocate(GtkAllocation* aAllocation);
	virtual void OnSizeRequest(GtkRequisition* aRequisition);
    public:
	CagTextView* iTrans; // Transition
    private:
	CapTranHead* iHead;
	const string& iTranData;
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
    public MCapCpObserver, public MWidgetObs
{
    public:
	static inline const char* Type() { return "CapSys";} ; 
	CapSys(const string& aName, CAE_Object::Ctrl& aSys, MCapSysObserver* aObserver);
	virtual ~CapSys();
	// From CAE_Base
	virtual void *DoGetObj(const char *aName);
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
	virtual void OnCompCpRenamed(CapComp* aComp, CapCp* aCp, const string& aName, TBool aIsOutp);
	virtual void OnCompCpAddPairRequested(CapComp* aComp, CapCp* aCp, const string& aPairName);
	virtual void OnCompCpDelPairRequested(CapComp* aComp, CapCp* aCp, const string& aPairName);
	// From MCapStateObserver
	virtual void OnStateCpPairToggled(CapState* aState, CapCtermPair* aPair);
	virtual void OnStateNameChanged(CapState* aState, const string& aName);
	virtual void OnStateTypeChanged(CapState* aState, const string& aTypeName);
	virtual void OnStateLogspecChanged(CapState* aState, map<TInt, TInt>& aLogSpec);
	virtual void OnStateDeleteRequested(CapState* aState);
	virtual void OnStateAddingInput(CapState* aState);
	virtual void OnStateInpRenamed(CapState* aState, CapCp* aCp, const string& aName);
	virtual void OnStateTransUpdated(CapState* aState, const string& aTrans);
	virtual void OnStateInitUpdated(CapState* aState, const string& aInit);
	virtual void OnStateCpAddPairRequested(CapState* aState, CapCp* aCp, const string& aPairName);
	virtual void OnStateCpDelPairRequested(CapState* aState, CapCp* aCp, const string& aPairName);
	// From MCapCpObserver
	virtual void OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair);
	virtual void OnLabelRenamed(CapCp* aCp, const string& aName);
	virtual void OnCpAddPairRequested(CapCp* aCp, const string& aPairName);
	virtual void OnCpDelPairRequested(CapCp* aCp, CapCtermPair* aPair);
	// From MWidgetObs
	virtual TBool OnWidgetFocusOut(CagWidget* aWidget, GdkEventFocus* aEvent);
    protected:
	void Construct();
    private:
	CapComp* Comp(CagWidget* aWidget);
	void ActivateConn(CapCtermPair* aPair);
	void AddComponent();
	void AddState();
	void AddStateInp(CapState* aState);
	void AddTrans();
	void AddInp();
	void AddOutp();
	void DeleteState(CapState* aState);
	void ChangeCompName(CapComp* aComp, const string& aName);
	void ChangeTrans(const string& aTrans);
	void ChangeStateName(CapState* aState, const string& aName);
	void ChangeStateType(CapState* aState, const string& aTypeName);
	void ChangeStateTrans(CapState* aState, const string& aTrans);
	void ChangeStateInit(CapState* aState, const string& aInit);
	void RenameStateInp(CapState* aState, CapCp* aCp, const string& aName);
	void RenameInpOutp(CapCp* aCp, const string& aName);
	void AddStateCpPair(CapState* aState, CapCp* aCp, const string& aPairName);
	void DelStateCpPair(CapState* aState, CapCp* aCp, const string& aPairName);
	void RenameCompCp(CapComp* aState, CapCp* aCp, const string& aName, TBool aIsOutp);
	void AddCompCpPair(CapComp* aComp, CapCp* aCp, const string& aPairName);
	void DelCpPair(string aMansFullName, TBool aIsInp, CapCp* aCp, const string& aPairName);
	void Refresh();
	string GetRandomNum() const;
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
	CAE_Object::Ctrl& iSys;
	map<CAE_StateBase*, CapState*> iStates; 
	map<CAE_Object*, CapComp*> iComps; // Components
	map<CAE_ConnPointBase*, CapCp*> iOutputs;
	map<CAE_ConnPointBase*, CapCp*> iInputs;
	CapTran* iTrans; // Transition
	MCapSysObserver* iObserver;
	CpPairObs iCpPairObs;
	// Size requested parameters
	GtkRequisition iInpReq;
};

#endif 
