#ifndef __FAPBVP_GTK_DECT_H
#define __FAPBVP_GTK_DECT_H

#include <fapbase.h>
#include "caglayout.h"
#include "cagbutton.h"

class MDectObserver
{
    public:
	static inline const char* Type(); 
	virtual void OnDetLevelChanged(int aLevel) = 0;
};

inline const char* MDectObserver::Type() { return "DectObsr";} 

class CagButton;
// Detalisation control
class CapDect: public CagLayout, public MWidgetObs, public MCagButtonObs
{
    public:
	CapDect(const string& aName);
	virtual ~CapDect();
	static inline const char* Type(); 
	void SetObserver(MDectObserver* aObs);
	int Level() const { return iLevel;};
	void SetLevel(int aLevel);
	void SetLowerLim(int aLim);
	void SetUpperLim(int aLim);
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
	void HandleLevelChanged();
	void NotifyLevelChanged();
	// From MCagButtonObs
	virtual void OnClicked(CagButton* aBtn);
	// From CAE_Base
	virtual void *DoGetObj(const char *aName);
	// From MWidgetObs
	virtual TBool OnWidgetButtonPress(CagWidget* aWidget, GdkEventButton* aEvent);
    private:
	CagButton* iLess;
	CagButton* iMore;
	MDectObserver* iObserver;
	int iLevel;
	int iLowerLim;
	int iUpperLim;
};

inline const char* CapDect::Type() { return "CapDect";} 

#endif 
