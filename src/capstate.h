
#ifndef __FAPBVP_GTK_STATE_H
#define __FAPBVP_GTK_STATE_H

#include <fapbase.h>
#include "caglayout.h"
#include "capcp.h"
#include "cagbox.h"
#include "cagmenu.h"
#include "capmiscwid.h"

class CapStatePopupMenu: public CagMenu
{
    public:
	CapStatePopupMenu(const string& aName);
};

class MStateHeadObserver
{
    public:
	static inline const char* Type() { return "MStateHeadObserver";} ; 
	virtual void OnStateNameChanged(const string& aName) = 0;
};

class CapStateHead: public CagHBox, public MapEopEntryObserver
{
    public:
	CapStateHead(const string& aName, CAE_StateBase& aState);
	void SetObserver(MStateHeadObserver* aObs);
    private:
	// From MapEopEntryObserver
	virtual void OnUpdateCompleted();
    private:
	CAE_StateBase& iState;
	CapEopEntry* iName;
	CagELabel* iType;
	MStateHeadObserver* iObs;
};

class CapState; 
class MCapStateObserver
{
    public:
	static inline const char* Type() { return "MCapStateObserver";} ; 
	virtual void OnStateCpPairToggled(CapState* aState, CapCtermPair* aPair) = 0;
	virtual void OnStateNameChanged(CapState* aState, const string& aName) = 0;
	virtual void OnStateDeleteRequested(CapState* aState) = 0;
};

class CapCp;
class CagTextView;
class CapState: public CagLayout, public MCapCpObserver, public MCapCpPairRes, public MStateHeadObserver, 
    public MagMenuShellObs
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
	// From MStateHeadObserver
	virtual void OnStateNameChanged(const string& aName);
	// From MagMenuShellObs 
	virtual void OnItemActivated(CagMenuShell* aMenuShell, CagMenuItem* aItem);
    private:
	virtual void OnExpose(GdkEventExpose* aEvent);
	virtual void OnSizeAllocate(GtkAllocation* aAllocation);
	virtual void OnSizeRequest(GtkRequisition* aRequisition);
	virtual TBool OnButtonPress(GdkEventButton* aEvent);
    private:
    public:
	CAE_StateBase& iState;
    private:
	CapStateHead* iHead;
	map<CAE_ConnPointBase*, CapCp*> iInps;
	map<CAE_ConnPointBase*, CapCp*> iOutps;
	CagTextView* iTrans; // Transition
	GtkAllocation iBodyAlc;
	MCapStateObserver* iObs;
	CapStatePopupMenu* iPopupMenu;
};
#endif
