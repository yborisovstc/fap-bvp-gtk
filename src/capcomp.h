
#ifndef __FAPBVP_GTK_COMP_H
#define __FAPBVP_GTK_COMP_H

#include <fapbase.h>
#include "caglayout.h"
#include "capcp.h"
#include "cagbox.h"
#include "cagentry.h"

class MCompHeadObserver
{
    public:
	static inline const char* Type() { return "MCompHeadObserver";} ; 
	virtual void OnCompNameClicked() = 0;
	virtual void OnCompParentClicked() = 0;
};



class CapCompHead: public CagHBox, public MWidgetObs
{
    public:
	CapCompHead(const string& aName, CAE_Object& aComp);
	static inline const char* Type() { return "CapCompHead";} ; 
	void SetObserver(MCompHeadObserver* aObs);
	// From CAE_Base
	virtual void *DoGetObj(const char *aName);
    private:
	virtual void OnExpose(GdkEventExpose* aEvent);
	// From MWidgetObs
	virtual TBool OnWidgetButtonPress(CagWidget* aWidget, GdkEventButton* aEvent);
    private:
	CAE_Object& iComp;
	CagEntry* iName;
	CagELabel* iParent;
	MCompHeadObserver* iObs;
};

class CapComp; 
class CagToggleButton; 
class MCapCompObserver
{
    public:
	static inline const char* Type() { return "CapCompObserever";} ; 
	virtual void OnCompCpPairToggled(CapComp* aComp, CapCtermPair* aPair) = 0;
	virtual void OnCompNameClicked(CapComp* aComp) = 0;
	virtual void OnCompParentClicked(CapComp* aComp) = 0;
};


class CapCp;
class CapComp: public CagLayout, public MCapCpObserver, public MCapCpPairRes, public MCompHeadObserver
{
    public:
	CapComp(const string& aName, CAE_Object& aComp);
	virtual ~CapComp();
	const CAE_Object& Comp() const { return iComp;};
	const CAE_EBase* CompEBase() const { CAE_EBase* res = iComp.GetFbObj(res); return res;};
	int GetInpTermY(CAE_ConnPointBase* aCp);
	int GetOutpTermY(CAE_ConnPointBase* aCp);
	int GetBodyCenterX() const;
	void SetObs(MCapCompObserver* aObs);
	// From MCapCpObserver
	virtual void OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair);
	// From MCapCpPairRes
	virtual CapCtermPair* GetCpPair(CapCtermPair* aPair);
	// From MCompHeadObserver
	virtual void OnCompNameClicked();
	virtual void OnCompParentClicked();
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
    public:
	CAE_Object& iComp;
    private:
	CapCompHead* iHead;
	map<CAE_ConnPointBase*, CapCp*> iInps;
	map<CAE_ConnPointBase*, CapCp*> iOutps;
	GtkAllocation iBodyAlc;
	MCapCompObserver* iObs;
};

#endif
