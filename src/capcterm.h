#ifndef __FAPBVP_GTK_CPLUG_H
#define __FAPBVP_GTK_CPLUG_H

#include <fapbase.h>
#include "caglayout.h"
#include "cagbutton.h"
#include "capbutton.h"
#include "capdect.h"

// Pair in Connection terminator
class CapCtermPair: public CagButton
{
    public:
	CapCtermPair(const string& aName, CAE_ConnPointBase& aCp);
	virtual ~CapCtermPair();
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
	CAE_ConnPointBase& iCp;
	string iLabel;
};

// Connection terminator
class CapCterm: public CagLayout, public MDectObserver
{
    public:
	CapCterm(const string& aName, CAE_ConnPointBase& aCp, TBool aLeft);
	virtual ~CapCterm();
	TBool IsLeft() const { return iLeft;};
	const CAE_ConnPointBase& Cp() const { return iCp;};
	int GetTermConnY() const;
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
	// From MDectObserver
	virtual void OnDetLevelChanged(int aLevel);
    private:
	CAE_ConnPointBase& iCp;
	CapDect* iContr; // Controller
	CagButton* iInfo; // Info. Shown when multiple pairs, and  minimized
	map<CAE_ConnPointBase*, CapCtermPair*> iPairs; // Connection pairs
	TBool iLeft; // Connected from left (i.e to comp output)
};

#endif 
