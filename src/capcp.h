#ifndef __FAPBVP_GTK_CP_H
#define __FAPBVP_GTK_CP_H

#include <fapbase.h>
#include "caglayout.h"
#include "caglabel.h"


class CapCterm;
class CapCp: public CagLayout
{
    public:
	CapCp(const string& aName, CAE_ConnPointBase& aCp, TBool aLeft);
	virtual ~CapCp();
	int GetLabelWidth() const;
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
	CagLabel* iLabel;
	CapCterm* iTerm; // Connections terminator
	TBool iLeft;
};

#endif 
