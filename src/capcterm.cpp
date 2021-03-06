
#include "capcommon.h"
#include "capcterm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// *************************************************************
// Pair in Connection terminator
// *************************************************************

CapCtermPair::CapCtermPair(const string& aName, CAE_ConnPointBase& aCp, TBool aLocalCtx): 
    CagToggleButton(aName), iCp(aCp), iLocalCtx(aLocalCtx)
{
    if (aLocalCtx) {
	SetLabel(iCp.Name());
    }
    else {
	SetLabel(string(iCp.Man().Name()) + "." + iCp.Name());
    }
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

void* CapCtermPair::DoGetObj(const char *aName)
{
    if (strcmp(aName, Type()) == 0) 
	return this;
    else return CagToggleButton::DoGetObj(aName);
}

string CapCtermPair::GetFullName()
{
    CAE_Object* obj = Pair()->Man().GetFbObj(obj);
    TNodeType type = obj != NULL ? ENt_Object : ENt_State;
    return MAE_Chromo::GetTName(type, (iLocalCtx ? string() :  string(Pair()->Man().Name()) + ".")  + Pair()->Name());
}

// *************************************************************
// Connection terminator
// *************************************************************

const char* KCpTermPmenu_Disconnect = "Disconnect";

vector<TPmenuSpecElem> CapCterm::iPmenuSpec;

CapCterm::CapCterm(const string& aName, CAE_ConnPointBase& aCp, TBool aExt, TBool aLeft): 
    CagLayout(aName), iCp(aCp), iLeft(aLeft), iInfo(NULL), iTermObs(NULL), iExt(aExt), iDetLevel(1), iContr(NULL)
{
    if (iPmenuSpec.empty()) {
	iPmenuSpec.push_back(TPmenuSpecElem(KCpTermPmenu_Disconnect, "Disconnect"));
    }
    // Popup Menu
    iPopupMenu = new CapPopupMenu("Menu", iPmenuSpec);
    iPopupMenu->SetTitle("cont menu");
    iPopupMenu->Show();
    iPopupMenu->SetMenuShellObs(this);
    // Create pairs
    for (vector<CAE_ConnBase*>::const_iterator it = iCp.Conns().begin(); it != iCp.Conns().end(); it++) {
	CAE_ConnBase* conn = *it;
	CAE_ConnPointBase* pair = &iCp == conn->Point() ? conn->Pair() : conn->Point();
	CAE_NBase* syst = iExt ? iCp.Owner() : iCp.Owner()->Owner(); // System having view context
	if (pair->Owner() == syst || pair->Owner()->Owner() == syst) {
	    CapCtermPair* pairw = new CapCtermPair("CtermPair~" + pair->Owner()->Name() + "." + pair->Name(), *pair);
	    iPairs[pair] = pairw;
	    Add(pairw);
	    pairw->SetObs(this);
	    pairw->SetWidgetObs(this);
	}
    }
    if (iPairs.size() == 0) {
	iDetLevel = 1;
	SetVisible(EFalse);
    }
    else if (iPairs.size() == 1) {
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
	sprintf(buf, "%d", iPairs.size());
	iInfo = new CagButton("Info");
	iInfo->SetLabel(string(buf) + " conn");
	GtkRcStyle* style = gtk_widget_get_modifier_style(iInfo->iWidget);
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
    else if (strcmp(aName, MWidgetObs::Type()) == 0)
	return (MWidgetObs*) this;
    else return CagLayout::DoGetObj(aName);
}

void CapCterm::SetItemHeightHint(int aHeight)
{
    if (iInfo != NULL) {
	GtkRequisition req; iInfo->SizeRequest(&req);
	iInfo->SetSizeRequest(req.width + 1, aHeight);
    }
    for (map<CAE_ConnPointBase*, CapCtermPair*>::iterator it = iPairs.begin(); it != iPairs.end(); it++) {
	CapCtermPair* pairw = it->second;
	GtkRequisition req; pairw->SizeRequest(&req);
	pairw->SetSizeRequest(req.width, aHeight);
    }
}

void CapCterm::OnExpose(GdkEventExpose* aEvent)
{
//    GtkAllocation alc; Allocation(&alc);
//    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, 0, 0, alc.width - 1, alc.height - 1);
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
	iTermObs->OnCpTermPairToggled(pair);
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

void CapCterm::OnDragDataReceived(GdkDragContext *drag_context, gint x, gint y, GtkSelectionData *data, guint info, guint time)
{
    char* textd = (char*) gtk_selection_data_get_text(data);
    if (info == KTei_Conn)
    {
	gtk_drag_finish(drag_context, true, false, time);
	if (iTermObs != NULL) {
	    iTermObs->OnCpTermAddPairRequested(this, textd);
	}
    }
    else {
	gdk_drag_status(drag_context, (GdkDragAction) 0, time);
    }
}

void CapCterm::OnItemActivated(CagMenuShell* aMenuShell, CagMenuItem* aItem)
{
    CapPopupMenu* menu = aMenuShell->GetObj(menu);
    _FAP_ASSERT(menu != NULL);
    CagWidget* context = menu->Context();
    _FAP_ASSERT(context != NULL);
    CapCtermPair* pair = context->GetObj(pair);
    _FAP_ASSERT(pair != NULL);
    if (iTermObs != NULL) {
	iTermObs->OnCpTermDelPairRequested(this, pair);
    }
}

TBool CapCterm::OnWidgetButtonPress(CagWidget* aWidget, GdkEventButton* aEvent)
{
    if (aEvent->button == 3) {
	// Popup context menu
	iPopupMenu->SetContext(aWidget);
	iPopupMenu->Popup(aEvent->button, aEvent->time);
	return ETrue;
    }
    return EFalse;
}
