#ifndef __FAPBVP_GTK_CTERM_H
#define __FAPBVP_GTK_CTERM_H

#include <fapbase.h>
#include "caglayout.h"
#include "cagbutton.h"
#include "capbutton.h"
#include "capdect.h"

// Pair in Connection terminator
class CapCtermPair: public CagToggleButton
{
    public:
	CapCtermPair(const string& aName, CAE_ConnPointBase& aCp);
	virtual ~CapCtermPair();
	const CAE_ConnPointBase* Pair() const {return &iCp;};
	CAE_ConnPointBase* Cp();
    private:
	CAE_ConnPointBase& iCp;
	string iLabel;
};

class MCapCtermObserver
{
    public:
	static inline const char* Type() { return "CapCtermObserver";} ; 
	virtual void OnCpPairToggled(CapCtermPair* aPair) = 0;
};

// Connpoint pair resolver ifale
class MCapCpPairRes
{
    public:
	static inline const char* Type() { return "MCapCpPairRes";} ; 
	virtual CapCtermPair* GetCpPair(CapCtermPair* aPair) = 0;
};


// Connection terminator
class CapCterm: public CagLayout, public MDectObserver, public MCagToggleButtonObs, public MCapCpPairRes
{
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
	virtual TBool OnButtonPress(GdkEventButton* aEvent);
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
	// From MDectObserver
	virtual void OnDetLevelChanged(int aLevel);
	// From MCagToggleButtonObs
	virtual void OnToggled(CagToggleButton* aBtn);
    private:
	CAE_ConnPointBase& iCp;
	CapDect* iContr; // Controller
	CagButton* iInfo; // Info. Shown when multiple pairs, and  minimized
	map<CAE_ConnPointBase*, CapCtermPair*> iPairs; // Connection pairs
	TBool iLeft; // Connected from left (i.e to comp output)
	MCapCtermObserver* iTermObs;
	TBool iExt; // Role is "extentder"
	TInt iDetLevel; 
};

#endif 
