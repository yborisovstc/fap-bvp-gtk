#ifndef __FAPBVP_GTK_CTERM_H
#define __FAPBVP_GTK_CTERM_H

#include <fapbase.h>
#include "caglayout.h"
#include "cagbutton.h"
#include "capbutton.h"
#include "capmiscwid.h"
#include "capdect.h"

// Pair in Connection terminator
class CapCtermPair: public CagToggleButton
{
    public:
	static inline const char* Type() { return "CapCtermPair";} ; 
	CapCtermPair(const string& aName, CAE_ConnPointBase& aCp, TBool aLocalCtx = EFalse);
	virtual ~CapCtermPair();
	const CAE_ConnPointBase* Pair() const {return &iCp;};
	CAE_ConnPointBase* Pair() {return &iCp;};
	CAE_ConnPointBase* Cp();
	string GetFullName();
	// From CAE_Base
	virtual void *DoGetObj(const char *aName);
    private:
	CAE_ConnPointBase& iCp;
	string iLabel;
	TBool iLocalCtx;
};

class CapCterm;
class MCapCtermObserver
{
    public:
	static inline const char* Type() { return "CapCtermObserver";} ; 
	virtual void OnCpTermPairToggled(CapCtermPair* aPair) = 0;
	virtual void OnCpTermAddPairRequested(CapCterm* aCpTerm, const string& aPairName) = 0;
	virtual void OnCpTermDelPairRequested(CapCterm* aCpTerm, CapCtermPair* aPair) = 0;
};

// Connpoint pair resolver ifale
class MCapCpPairRes
{
    public:
	static inline const char* Type() { return "MCapCpPairRes";} ; 
	virtual CapCtermPair* GetCpPair(CapCtermPair* aPair) = 0;
};

// Connection terminator
class CapCterm: public CagLayout, public MDectObserver, public MCagToggleButtonObs, public MCapCpPairRes,
    public MagMenuShellObs, public MWidgetObs
{
    friend class CapCp;
    public:
	CapCterm(const string& aName, CAE_ConnPointBase& aCp, TBool aExt, TBool aLeft);
	virtual ~CapCterm();
	TBool IsLeft() const { return iLeft;};
	const CAE_ConnPointBase* Cp() const { return &iCp;};
	CAE_ConnPointBase* Cp() { return &iCp;};
	void SetObs(MCapCtermObserver* aObs);
	void SetItemHeightHint(int aHeight);
	static inline const char* Type() { return "CapCterm";} ; 
	// From MCapCpPairRes
	virtual CapCtermPair* GetCpPair(CapCtermPair* aPair);
    private:
	virtual void OnExpose(GdkEventExpose* aEvent);
	virtual TBool OnButtonRelease(GdkEventButton* aEvent);
	virtual void OnSizeAllocate(GtkAllocation* aAllocation);
	virtual void OnSizeRequest(GtkRequisition* aRequisition);
	virtual void OnMotion(GdkEventMotion *aEvent);
	virtual void OnEnter(GdkEventCrossing *aEvent);
	virtual void OnLeave(GdkEventCrossing *aEvent);
	virtual void OnStateChanged(GtkStateType state);
	virtual void OnDragDataReceived(GdkDragContext *drag_context, gint x, gint y, GtkSelectionData *data, guint info, guint time);
	// From CAE_Base
	virtual void *DoGetObj(const char *aName);
	// From MWidgetObs
	virtual TBool OnWidgetButtonPress(CagWidget* aWidget, GdkEventButton* aEvent);
	// From MDectObserver
	virtual void OnDetLevelChanged(int aLevel);
	// From MCagToggleButtonObs
	virtual void OnToggled(CagToggleButton* aBtn);
	// From MagMenuShellObs 
	virtual void OnItemActivated(CagMenuShell* aMenuShell, CagMenuItem* aItem);
    private:
	CAE_ConnPointBase& iCp;
	CapDect* iContr; // Controller
	CagButton* iInfo; // Info. Shown when multiple pairs, and  minimized
	map<CAE_ConnPointBase*, CapCtermPair*> iPairs; // Connection pairs
	TBool iLeft; // Connected from left (i.e to comp output)
	MCapCtermObserver* iTermObs;
	TBool iExt; // Role is "extentder"
	TInt iDetLevel; 
	CapPopupMenu* iPopupMenu;
	static vector<TPmenuSpecElem> iPmenuSpec;
};

#endif 
