
#ifndef __FAPBVP_GTK_COMP_H
#define __FAPBVP_GTK_COMP_H

#include <fapbase.h>
#include "caglayout.h"


class CapCompHead;
class CapCp;
class CapComp: public CagLayout
{
    public:
	CapComp(const string& aName, CAE_Object& aComp);
	virtual ~CapComp();
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
	virtual void OnChildStateChanged(CagWidget* aChild, GtkStateType aPrevState);
    public:
	CAE_Object& iComp;
    private:
	CapCompHead* iHead;
	map<CAE_ConnPointBase*, CapCp*> iInps;
	map<CAE_ConnPointBase*, CapCp*> iOutps;
};

#endif
