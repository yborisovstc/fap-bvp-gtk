#ifndef __FAPBVP_GTK_CP_H
#define __FAPBVP_GTK_CP_H

#include <fapbase.h>
#include "caglayout.h"
#include "caglabel.h"
#include "capcterm.h"

class CagToggleButton;
class CapCp;
class MCapCpObserver
{
    public:
	static inline const char* Type() { return "CapCpObserever";} ; 
	virtual void OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair) = 0;
};

class CapCterm;
class CapCp: public CagLayout, public MCapCtermObserver, public MCapCpPairRes
{
    public:
	CapCp(const string& aName, CAE_ConnPointBase& aCp, TBool aLeft, TBool aLineSep = EFalse);
	virtual ~CapCp();
	int GetLabelWidth() const;
	void SetObs(MCapCpObserver* aObs);
	// From MCapCpPairRes
	virtual CapCtermPair* GetCpPair(CapCtermPair* aPair);
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
	// From MCapCpObserver
	virtual void OnCpPairToggled(CapCtermPair* aPair);
    private:
	CAE_ConnPointBase& iCp;
	CagLabel* iLabel;
	CapCterm* iTerm; // Connections terminator
	TBool iLeft;
	TBool iLineSep; // Needs to draw line separator
	MCapCpObserver* iCpObs;
};

#endif 
