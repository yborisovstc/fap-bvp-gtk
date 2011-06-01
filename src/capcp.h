#ifndef __FAPBVP_GTK_CP_H
#define __FAPBVP_GTK_CP_H

#include <fapbase.h>
#include "caglayout.h"
#include "caglabel.h"
#include "capcterm.h"
#include "capmiscwid.h"

class CagToggleButton;
class CapCp;
class MCapCpObserver
{
    public:
	static inline const char* Type() { return "CapCpObserever";} ; 
	virtual void OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair) = 0;
	virtual void OnLabelRenamed(CapCp* aCp, const string& aName) = 0;
	virtual void OnCpAddPairRequested(CapCp* aCp, const string& aPairName) = 0;
	virtual void OnCpDelPairRequested(CapCp* aCp, CapCtermPair* aPair) = 0;
};

class CapCterm;
class CapCp: public CagLayout, public MCapCtermObserver, public MCapCpPairRes, public MapEopEntryObserver
{
    public:
	CapCp(const string& aName, CAE_ConnPointBase& aCp, TBool aExt, TBool aLeft, TBool aLineSep = EFalse, TBool aNoLabel = EFalse);
	virtual ~CapCp();
	int GetLabelWidth() const;
	int GetTermWidth();
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
	// From MCapCtermObserver
	virtual void OnCpTermPairToggled(CapCtermPair* aPair);
	virtual void OnCpTermAddPairRequested(CapCterm* aCpTerm, const string& aPairName);
	virtual void OnCpTermDelPairRequested(CapCterm* aCpTerm, CapCtermPair* aPair);
	// From MapEopEntryObserver
	virtual void OnUpdateCompleted(CapEopEntry* aEntry);
    public:
	CAE_ConnPointBase& iCp;
    private:
	CapEopEntry* iLabel;
	CapCterm* iTerm; // Connections terminator
	TBool iLeft;
	TBool iLineSep; // Needs to draw line separator
	MCapCpObserver* iCpObs;
	TBool iNoLabel; // No label displayed (case of state output)
	TBool iExt; // The role - extender
};

#endif 
