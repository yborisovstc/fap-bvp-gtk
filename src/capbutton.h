#ifndef __FAPBVP_GTK_COMPHEAD_H
#define __FAPBVP_GTK_COMPHEAD_H

#include <fapbase.h>
#include "caglayout.h"

class CapButton: public CagLayout
{
    public:
	CapButton(const string& aName);
	CapButton(const string& aName, const string& aText);
	virtual ~CapButton();
	void SetText(const string& aText);
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
	string iText;
};

#endif 
