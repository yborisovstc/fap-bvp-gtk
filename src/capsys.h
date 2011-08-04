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
	virtual void OnCompDeleteRequested(CapComp* aComp);
	virtual void OnCompCpRenamed(CapComp* aComp, CapCp* aCp, const string& aName, TBool aIsOutp);
	virtual void OnCompCpAddPairRequested(CapComp* aComp, CapCp* aCp, const string& aPairName);
	virtual void OnCompCpDelPairRequested(CapComp* aComp, CapCp* aCp, CapCtermPair* aPair);
	virtual void OnCompChangeQuietRequested(CapComp* aComp, TBool aQuiet);
	// From MCapStateObserver
	virtual void OnStateCpPairToggled(CapState* aState, CapCtermPair* aPair);
	virtual void OnStateNameChanged(CapState* aState, const string& aName);
	virtual void OnStateTypeChanged(CapState* aState, const string& aTypeName);
	virtual void OnStateLogspecChanged(CapState* aState, map<TLeBase, TInt>& aLogSpec);
	virtual void OnStateDeleteRequested(CapState* aState);
	virtual void OnStateAddingInput(CapState* aState);
	virtual void OnStateInpRenamed(CapState* aState, CapCp* aCp, const string& aName);
	virtual void OnStateTransUpdated(CapState* aState, const string& aTrans);
	virtual void OnStateInitUpdated(CapState* aState, const string& aInit);
	virtual void OnStateCpAddPairRequested(CapState* aState, CapCp* aCp, const string& aPairName);
	virtual void OnStateCpDelPairRequested(CapState* aState, CapCp* aCp, CapCtermPair* aPair);
	virtual void OnStateNodeDelRequested(CapState* aState, CAE_NBase* aNode);
	// From MCapCpObserver
	virtual void OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair);
	virtual void OnLabelRenamed(CapCp* aCp, const string& aName);
	virtual void OnCpAddPairRequested(CapCp* aCp, const string& aPairName);
	virtual void OnCpDelPairRequested(CapCp* aCp, CapCtermPair* aPair);
	virtual void OnCpDelRequested(CapCp* aCp);
	// From MWidgetObs
	virtual TBool OnWidgetFocusOut(CagWidget* aWidget, GdkEventFocus* aEvent);
    protected:
	void Construct();
    private:
	CapComp* Comp(CagWidget* aWidget);
	void ActivateConn(CapCtermPair* aPair);
	void AddComponent(const string& aName, const string& aType);
	void AddState(const string& aName, const string& aType);
	void AddStateInp(CapState* aState);
	void AddTrans();
	void AddInp();
	void AddOutp();
	void ChangeNodeAttr(CAE_NBase* aNode, TNodeAttr aAttr, const string& aVal);
	void ChangeNodeContent(CAE_NBase* aNode, const string& aVal);
	void RenameNode(CAE_NBase* aNode, const string& aName);
	void DeleteNode(CAE_NBase* aNode);
	void AddCpPair(CapCp* aCp, const string& aPairName, TBool aInp);
	void DelCpPair_v1(CAE_EBase* aCpOwner, TBool aIsInp, CapCp* aCp, CapCtermPair* aPair);
	void Refresh();
	string GetRandomNum() const;
	void GetCompTypesAvailable(vector<string>& aList) const;
	void GetStateTypesAvailable(vector<string>& aList) const;
	void AddCompTypesFromLocModPath(const string& aDirUri, const string& aPath, vector<string>& aList) const;
	void AddCompTypesFromModPaths(vector<string>& aList) const;
	static int FilterModulesDirEntries(const struct dirent *aEntry);
	void MoveComp(const string& aName, gint aX, gint aY);
	void FindCompByPosY(string& aCompName, gint aY);
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
	vector<CapComp*> iComps; // Components
	map<CAE_ConnPointBase*, CapCp*> iOutputs;
	map<CAE_ConnPointBase*, CapCp*> iInputs;
	CapTran* iTrans; // Transition
	MCapSysObserver* iObserver;
	CpPairObs iCpPairObs;
	// Size requested parameters
	GtkRequisition iInpReq;
	static vector<string> iModulesPaths;
};

#endif 
