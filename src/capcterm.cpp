
#include "capcommon.h"
#include "capcterm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// *************************************************************
// Pair in Connection terminator
// *************************************************************

CapCtermPair::CapCtermPair(const string& aName, CAE_ConnPointBase& aCp): CagButton(aName), iCp(aCp)
{
    SetLabel(string(iCp.Man().InstName()) + "." + iCp.Name());
}

CapCtermPair::~CapCtermPair()
{
}


void CapCtermPair::OnExpose(GdkEventExpose* aEvent)
{
}

TBool CapCtermPair::OnButtonPress(GdkEventButton* aEvent)
{
}

TBool CapCtermPair::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapCtermPair::OnSizeAllocate(GtkAllocation* aAllocation)
{
}

void CapCtermPair::OnSizeRequest(GtkRequisition* aRequisition)
{
}

void CapCtermPair::OnMotion(GdkEventMotion *aEvent)
{
}

void CapCtermPair::OnEnter(GdkEventCrossing *aEvent)
{
}

void CapCtermPair::OnLeave(GdkEventCrossing *aEvent)
{
}

void CapCtermPair::OnStateChanged(GtkStateType state)
{
}



// *************************************************************
// Connection terminator
// *************************************************************

CapCterm::CapCterm(const string& aName, CAE_ConnPointBase& aCp, TBool aLeft): 
    CagLayout(aName), iCp(aCp), iLeft(aLeft), iInfo(NULL)
{
    // Create controller
    iContr = new CapDect("CtermCtrl." + iCp.Name());
    Add(iContr);
    iContr->SetObserver(this);
    iContr->SetLevel(1);
    iContr->SetLowerLim(1);
    iContr->SetUpperLim(2);
    iContr->Show();
    // Create info
    char* buf = (char *) malloc(9);
    memset(buf, 0, 10);
    sprintf(buf, "%d", iCp.Conns().size());
    //iInfo = new CapButton("Info", string(buf) + " conn");
    iInfo = new CagButton("Info");
    iInfo->SetLabel(string(buf) + " conn");
    free(buf);   // Create info
    Add(iInfo);
    iInfo->Show();
    // Create pairs
    for (vector<CAE_ConnPointBase*>::const_iterator it = iCp.Conns().begin(); it != iCp.Conns().end(); it++) {
	CAE_ConnPointBase* pair = *it;
	CapCtermPair* pairw = new CapCtermPair("CtermPair." + string(pair->Man().InstName()) + "." + pair->Name(), *pair);
	iPairs[pair] = pairw;
	Add(pairw);
	pairw->Show();
    }
}

CapCterm::~CapCterm()
{
}


void CapCterm::OnExpose(GdkEventExpose* aEvent)
{
}

TBool CapCterm::OnButtonPress(GdkEventButton* aEvent)
{
}

TBool CapCterm::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapCterm::OnSizeAllocate(GtkAllocation* aAllc)
{
    // Allocate size for controller
    GtkRequisition ctrl_req; iContr->SizeRequest(&ctrl_req);
    GtkAllocation ctrl_all = { 0, 0, ctrl_req.width, ctrl_req.height};
    iContr->SizeAllocate(&ctrl_all);
    // Allocate size for info
    GtkRequisition info_req; iInfo->SizeRequest(&info_req);
    GtkAllocation info_alc = { ctrl_all.width + KViewConnGapWidth, 0, info_req.width, info_req.height};
    iInfo->SizeAllocate(&info_alc);
    // Allocate size for pairs
    int pairb_x = info_alc.x; 
    int pairb_y = info_alc.y + info_alc.height + KViewConnGapHeight; 
    for (map<CAE_ConnPointBase*, CapCtermPair*>::iterator it = iPairs.begin(); it != iPairs.end(); it++) {
	CapCtermPair* pairw = it->second;
	GtkRequisition pair_req; pairw->SizeRequest(&pair_req);
	GtkAllocation pair_alc = (GtkAllocation) { pairb_x, pairb_y, pair_req.width, pair_req.height};
	pairw->SizeAllocate(&pair_alc);
	pairb_y += pair_alc.height + KViewConnGapHeight;
    }
}

void CapCterm::OnSizeRequest(GtkRequisition* aReq)
{
    // Get size request for controller
    GtkRequisition ctrl_req; iContr->SizeRequest(&ctrl_req);
    // Get size request for info
    GtkRequisition info_req = {0, 0}; iInfo->SizeRequest(&info_req);
    int pair_w = info_req.width;
    int pair_h = 0;
    // Get size request for pairs
    for (map<CAE_ConnPointBase*, CapCtermPair*>::iterator it = iPairs.begin(); it != iPairs.end(); it++) {
	CapCtermPair* pairw = it->second;
	GtkRequisition pair_req; pairw->SizeRequest(&pair_req);
	pair_w = max(pair_w, pair_req.width);
	pair_h += pair_req.height + KViewConnGapHeight;
    }
    *aReq = (GtkRequisition) {ctrl_req.width + pair_w + KViewConnGapWidth, max(ctrl_req.height, info_req.height + pair_h)};
}

void CapCterm::OnMotion(GdkEventMotion *aEvent)
{
}

void CapCterm::OnEnter(GdkEventCrossing *aEvent)
{
}

void CapCterm::OnLeave(GdkEventCrossing *aEvent)
{
}

void CapCterm::OnStateChanged(GtkStateType state)
{
}

void CapCterm::OnDetLevelChanged(int aLevel)
{
}
