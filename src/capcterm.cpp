
#include "capcommon.h"
#include "capcterm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// *************************************************************
// Pair in Connection terminator
// *************************************************************

CapCtermPair::CapCtermPair(const string& aName, CAE_ConnPointBase& aCp): CagToggleButton(aName), iCp(aCp)
{
    SetLabel(string(iCp.Man().InstName()) + "." + iCp.Name());
}

CapCtermPair::~CapCtermPair()
{
}

CAE_ConnPointBase* CapCtermPair::Cp()
{
    CapCterm* parent = iParent->GetObj(parent);
    _FAP_ASSERT(parent != NULL);
    return parent->Cp();
}


// *************************************************************
// Connection terminator
// *************************************************************

CapCterm::CapCterm(const string& aName, CAE_ConnPointBase& aCp, TBool aExt, TBool aLeft): 
    CagLayout(aName), iCp(aCp), iLeft(aLeft), iInfo(NULL), iTermObs(NULL), iExt(aExt), iDetLevel(1), iContr(NULL)
{
    // Create pairs
    if (iExt) {
	for (vector<CAE_ConnPointBase*>::const_iterator it = iCp.Exts().begin(); it != iCp.Exts().end(); it++) {
	    CAE_ConnPointBase* pair = *it;
	    CapCtermPair* pairw = new CapCtermPair("CtermPair~" + string(pair->Man().InstName()) + "." + pair->Name(), *pair);
	    iPairs[pair] = pairw;
	    Add(pairw);
	    pairw->SetObs(this);
	}
    }
    else {
	for (vector<CAE_ConnPointBase*>::const_iterator it = iCp.Conns().begin(); it != iCp.Conns().end(); it++) {
	    CAE_ConnPointBase* pair = *it;
	    CapCtermPair* pairw = new CapCtermPair("CtermPair~" + string(pair->Man().InstName()) + "." + pair->Name(), *pair);
	    iPairs[pair] = pairw;
	    Add(pairw);
	    pairw->SetObs(this);
	}
    }
    if (iPairs.size() == 1) {
	iDetLevel = 2;
    }
    else {
	// Create controller
	iDetLevel = 1;
	iContr = new CapDect("CtermCtrl~" + iCp.Name());
	Add(iContr);
	iContr->SetObserver(this);
	iContr->SetLowerLim(1);
	iContr->SetUpperLim(2);
	iContr->SetLevel(iDetLevel);
	iContr->Show();
	// Create info
	char* buf = (char *) malloc(9);
	memset(buf, 0, 10);
	sprintf(buf, "%d", iCp.Conns().size());
	iInfo = new CagButton("Info");
	iInfo->SetLabel(string(buf) + " conn");
	free(buf);  
	Add(iInfo);
	iInfo->Show();
    }

    OnDetLevelChanged(iDetLevel);
}

CapCterm::~CapCterm()
{
}

void* CapCterm::DoGetObj(const char *aName)
{
    if (strcmp(aName, MCagToggleButtonObs::Type()) == 0)
	return (MCagToggleButtonObs*) this;
    else if (strcmp(aName, Type()) == 0) 
	return this;
    else return CagLayout::DoGetObj(aName);
}

void CapCterm::SetItemHeightHint(int aHeight)
{
    if (iInfo != NULL) {
	GtkRequisition req; iInfo->SizeRequest(&req);
	iInfo->SetSizeRequest(req.width, aHeight);
    }
    for (map<CAE_ConnPointBase*, CapCtermPair*>::iterator it = iPairs.begin(); it != iPairs.end(); it++) {
	CapCtermPair* pairw = it->second;
	GtkRequisition req; pairw->SizeRequest(&req);
	pairw->SetSizeRequest(req.width, aHeight);
    }
}

void CapCterm::OnExpose(GdkEventExpose* aEvent)
{
    GtkAllocation alc; Allocation(&alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, 0, 0, alc.width - 1, alc.height - 1);
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
    GtkRequisition ctrl_req = (GtkRequisition) {0, 0}; if (iContr != NULL) iContr->SizeRequest(&ctrl_req);
    GtkAllocation ctrl_all = (GtkAllocation) { iLeft ? aAllc->width - ctrl_req.width - KViewBtnBoxInnerBoard: KViewBtnBoxInnerBoard, 
	KViewBtnBoxInnerBoard, ctrl_req.width, ctrl_req.height};
    if (iContr != NULL) iContr->SizeAllocate(&ctrl_all);
    // Allocate size for info
    GtkRequisition info_req = (GtkRequisition) {0, 0}; if (iInfo != NULL) iInfo->SizeRequest(&info_req);
    GtkAllocation info_alc = { iLeft ? KViewBtnBoxInnerBoard : ctrl_all.width + KViewConnGapWidth, KViewBtnBoxInnerBoard, 
	info_req.width, info_req.height};
    if (iInfo != NULL) iInfo->SizeAllocate(&info_alc);
    // Allocate size for pairs
    int pairb_x = iLeft ? KViewBtnBoxInnerBoard : info_alc.x; 
    int pairb_y = info_alc.y + info_alc.height + KViewConnGapHeight; 
    for (map<CAE_ConnPointBase*, CapCtermPair*>::iterator it = iPairs.begin(); it != iPairs.end(); it++) {
	CapCtermPair* pairw = it->second;
	GtkRequisition pair_req; pairw->SizeRequest(&pair_req);
	GtkAllocation pair_alc = (GtkAllocation) { pairb_x, pairb_y, 0, 0};
	if (iDetLevel <= 1) {
	    pair_alc.width = pair_req.width; pair_alc.height = pair_req.height;
	}
	else if (iDetLevel == 2) {
	    pair_alc.width = pair_req.width; pair_alc.height = pair_req.height;
	    pairb_y += pair_alc.height + KViewConnGapHeight;
	}
	pairw->SizeAllocate(&pair_alc);
    }
}

void CapCterm::OnSizeRequest(GtkRequisition* aReq)
{
    // Get size request for controller
    GtkRequisition ctrl_req = (GtkRequisition) {0, 0}; 
    if (iContr != NULL) {
	iContr->SizeRequest(&ctrl_req);
    }
    // Get size request for info
    GtkRequisition info_req = {0, 0}; if (iInfo != NULL) iInfo->SizeRequest(&info_req);
    int pair_w = 0;
    int pair_h = 0;
    // Get size request for pairs
    for (map<CAE_ConnPointBase*, CapCtermPair*>::iterator it = iPairs.begin(); it != iPairs.end(); it++) {
	CapCtermPair* pairw = it->second;
	GtkRequisition pair_req; pairw->SizeRequest(&pair_req);
	pair_w = max(pair_w, pair_req.width);
	if (iDetLevel == 2) {
	    pair_h += pair_req.height + KViewConnGapHeight;
	}
    }
    int pw = (iDetLevel<= 1) ? info_req.width : max(info_req.width, pair_w);
    *aReq = (GtkRequisition) 
    {ctrl_req.width + KViewConnGapWidth + pw + KViewBtnBoxInnerBoard*2, max(ctrl_req.height, info_req.height + pair_h) + KViewBtnBoxInnerBoard*2};
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
    iDetLevel = aLevel;
    for (map<CAE_ConnPointBase*, CapCtermPair*>::iterator it = iPairs.begin(); it != iPairs.end(); it++) {
	CapCtermPair* pairw = it->second;
	if (iDetLevel <= 1) {
	    pairw->Hide();
	}
	else {
	    pairw->Show();
	}
    }
}

void CapCterm::OnToggled(CagToggleButton* aBtn)
{
    if (iTermObs) {
	CapCtermPair* pair = aBtn->GetObj(pair);
	_FAP_ASSERT(pair != NULL);
	iTermObs->OnCpPairToggled(pair);
    }
}

void CapCterm::SetObs(MCapCtermObserver* aObs)
{
    _FAP_ASSERT(iTermObs == NULL);
    iTermObs = aObs;
}

CapCtermPair* CapCterm::GetCpPair(CapCtermPair* aPair)
{
    CapCtermPair* res = NULL;
    if (aPair->Pair() == &iCp) {
	if (iPairs.count(aPair->Cp()) > 0) {
	  res = iPairs[aPair->Cp()];
	}
    }
    return res;
}
