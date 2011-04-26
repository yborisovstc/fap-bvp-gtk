
#ifndef __FAPBVP_GTK_STATE_H
#define __FAPBVP_GTK_STATE_H

#include <fapbase.h>
#include "caglayout.h"
#include "capcp.h"

class CapStateHead: public CagLayout
{
    public:
	CapStateHead(const string& aName, CAE_StateBase& aState);
    private:
	virtual void OnExpose(GdkEventExpose* aEvent);
	virtual void OnSizeAllocate(GtkAllocation* aAllocation);
	virtual void OnSizeRequest(GtkRequisition* aRequisition);
    private:
	CAE_StateBase& iState;
	CagLabel* iName;
};

class CapState; 
class MCapStateObserver
{
    public:
	static inline const char* Type() { return "MCapStateObserver";} ; 
	virtual void OnStateCpPairToggled(CapState* aComp, CapCtermPair* aPair) = 0;
};

class CapCp;
class CagTextView;
class CapState: public CagLayout, public MCapCpObserver, public MCapCpPairRes
{
    public:
	CapState(const string& aName, CAE_StateBase& aState);
	virtual ~CapState();
	const CAE_StateBase& State() const { return iState;};
	void SetObs(MCapStateObserver* aObs);
	int GetBodyCenterX() const;
	// From MCapCpObserver
	virtual void OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair);
	// From MCapCpPairRes
	virtual CapCtermPair* GetCpPair(CapCtermPair* aPair);
    private:
	virtual void OnExpose(GdkEventExpose* aEvent);
	virtual void OnSizeAllocate(GtkAllocation* aAllocation);
	virtual void OnSizeRequest(GtkRequisition* aRequisition);
    public:
	CAE_StateBase& iState;
    private:
	CapStateHead* iHead;
	map<CAE_ConnPointBase*, CapCp*> iInps;
	map<CAE_ConnPointBase*, CapCp*> iOutps;
	CagTextView* iTrans; // Transition
	GtkAllocation iBodyAlc;
	MCapStateObserver* iObs;
};

#endif
