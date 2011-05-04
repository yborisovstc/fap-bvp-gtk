
#include <stdlib.h>
#include "capsys.h"
#include "cagproxy.h"
#include "capcomp.h"
#include "capstate.h"
#include "capcp.h"
#include "capcommon.h"
#include "capcterm.h"

CapSysHead::CapSysHead(const string& aName, CAE_Object::Ctrl& aSys): CagLayout(aName), iSys(aSys)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iSys.Object().InstName(), strlen(iSys.Object().InstName()));
    int w, h; pango_layout_get_pixel_size(plo, &w, &h);
    gtk_widget_set_size_request(iWidget, w, h);
    g_object_unref(plo);
}

CapSysHead::~CapSysHead()
{
}


void CapSysHead::OnExpose(GdkEventExpose* aEvent)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iSys.Object().InstName(), strlen(iSys.Object().InstName()));
    gdk_draw_layout(BinWnd(), Gc(), 0, 0, plo);
    GtkAllocation alc; gtk_widget_get_allocation(iWidget, &alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, alc.x, alc.y, alc.width - 1, alc.height - 1);
    g_object_unref(plo);
}

TBool CapSysHead::OnButtonPress(GdkEventButton* aEvent)
{
    if (aEvent->type == GDK_BUTTON_PRESS) {
	SetState(GTK_STATE_SELECTED);
    }
}

TBool CapSysHead::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapSysHead::OnSizeAllocate(GtkAllocation* aAllocation)
{
    
}

void CapSysHead::OnSizeRequest(GtkRequisition* aRequisition)
{
}

void CapSysHead::OnMotion(GdkEventMotion *aEvent)
{
}

void CapSysHead::OnEnter(GdkEventCrossing *aEvent)
{
}

void CapSysHead::OnLeave(GdkEventCrossing *aEvent)
{
}

void CapSysHead::OnStateChanged(GtkStateType state)
{
    if (State() == GTK_STATE_SELECTED) {
	iParent->OnChildStateChanged(this, state);
    }
}




CapSys::CapSys(const string& aName, CAE_Object::Ctrl& aSys, MCapSysObserver* aObserver): 
    CagLayout(aName), iSys(aSys), iObserver(aObserver), iCpPairObs(*this)
{
    Construct();
}

void CapSys::Construct()
{
    // Add header
    iHead = new CapSysHead("Title", iSys);
    Add(iHead);
    iHead->Show();
    // Add states
    for (vector<CAE_EBase*>::iterator it = iSys.CompReg().begin(); it != iSys.CompReg().end(); it++) {
	CAE_StateBase* state = (*it)->GetFbObj(state);
	if (state != NULL) {
	    CapState* stw = new CapState("State~" + string(state->InstName()), *state);
	    Add(stw);
	    iStates[state] = stw;
	    stw->SetObs(this);
	    stw->Show();
	}
    }
    // Add components
    for (vector<CAE_EBase*>::iterator it = iSys.CompReg().begin(); it != iSys.CompReg().end(); it++) {
	CAE_Object* obj = (*it)->GetFbObj(obj);
	if (obj != NULL) {
	    CapComp* comp = new CapComp("Comp~" + string(obj->InstName()), *obj);
	    Add(comp);
	    iComps[obj] = comp;
	    comp->SetObs(this);
	    comp->Show();
	}
    }
    // Add inputs extenders
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iSys.Object().Inputs().begin(); it != iSys.Object().Inputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp("Inp~" + cp->Name(), *cp, ETrue, ETrue);
	Add(cpw);
	cpw->SetObs(this);
	iInputs[cp] = cpw;
	cpw->Show();
    }
    // Add outputs extenderd
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iSys.Object().Outputs().begin(); it != iSys.Object().Outputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp("Outp~" + cp->Name(), *cp, ETrue, EFalse);
	Add(cpw);
	cpw->SetObs(this);
	iOutputs[cp] = cpw;
	cpw->Show();
    }
}

CapSys::~CapSys()
{
}


void CapSys::OnExpose(GdkEventExpose* aEvent)
{
}

void CapSys::OnSizeAllocate(GtkAllocation* aAllc)
{
    GtkAllocation alc; Allocation(&alc);
    // Allocate header
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    GtkAllocation head_alc = { 0, 0, alc.width, head_req.height};
    iHead->SizeAllocate(&head_alc);
    // Allocate inputs
    int inpb_x = aAllc->width - iInpReq.width, inpb_y = head_req.height + KViewCompGapHight;
    int inp_w = 0, inp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInputs.begin(); it != iInputs.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	GtkAllocation allc = {inpb_x, inpb_y, req.width, req.height};
	cpw->SizeAllocate(&allc);
	inp_w = max(inp_w, req.width);
	inpb_y += req.height + KViewConnGapHeight;
    }
    // Allocate outputs
    int outpb_x = 0, outpb_y = head_req.height + KViewCompGapHight;
    int outp_w = 0, outp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutputs.begin(); it != iOutputs.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	GtkAllocation allc = {outpb_x, outpb_y, req.width, req.height};
	cpw->SizeAllocate(&allc);
	outp_w = max(outp_w, req.width);
	outpb_y += req.height + KViewConnGapHeight;
    }
    // Allocate states
    int statb_x = (outp_w + aAllc->width - inp_w)/2, statb_y = head_req.height + KViewCompGapHight;
    for (map<CAE_StateBase*, CapState*>::iterator it = iStates.begin(); it != iStates.end(); it++) {
	CAE_StateBase* state = it->first;
	CapState* stw = it->second;
	GtkRequisition req; stw->SizeRequest(&req);
	int body_center_x = stw->GetBodyCenterX();
	GtkAllocation allc = {statb_x - body_center_x, statb_y, req.width, req.height};
	stw->SizeAllocate(&allc);
	statb_y += req.height + KViewCompGapHight;
    }
    // Allocate components
    int compb_x = (outp_w + aAllc->width - inp_w)/2, compb_y = statb_y;
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	CAE_Object* obj = it->first;
	CapComp* comp = it->second;
	GtkRequisition req; comp->SizeRequest(&req);
	int comp_body_center_x = comp->GetBodyCenterX();
	GtkAllocation allc = {compb_x - comp_body_center_x, compb_y, req.width, req.height};
	comp->SizeAllocate(&allc);
	compb_y += req.height + KViewCompGapHight;
    }
}

void CapSys::OnSizeRequest(GtkRequisition* aRequisition)
{
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    // Calculate size of states
    int stat_w = 0, stat_h = 0;
    for (map<CAE_StateBase*, CapState*>::iterator it = iStates.begin(); it != iStates.end(); it++) {
	CapState* state = it->second;
	GtkRequisition req; state->SizeRequest(&req);
	stat_w = max(stat_w, req.width);
	stat_h += req.height + KViewCompGapHight;
    }
    // Calculate size of comps
    int comp_w = 0, comp_h = 0;
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	CapComp* comp = it->second;
	GtkRequisition req; comp->SizeRequest(&req);
	comp_w = max(comp_w, req.width);
	comp_h += req.height + KViewCompGapHight ;
    }
    // Calculate size of inputs
    int inp_w = 0, inp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInputs.begin(); it != iInputs.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	inp_w = max(inp_w, req.width);
	inp_h += req.height + KViewConnGapHeight;
    }
    iInpReq.width = inp_w; iInpReq.height = inp_h;
    // Calculate size of outputs
    int outp_w = 0, outp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutputs.begin(); it != iOutputs.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	outp_w = max(outp_w, req.width);
	outp_h += req.height + KViewConnGapHeight;
    }

    aRequisition->width = outp_w + KViewExtCompGapWidth*2 + max(stat_w, comp_w) + inp_w; 
    aRequisition->height = head_req.height + max(max(outp_h, comp_h + stat_h), inp_h);
}


CapComp* CapSys::Comp(CagWidget* aWidget)
{
    CapComp* res = NULL;
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	if (it->second == aWidget) {
	    res = it->second; break;
	}
    }
    return res;
}

void CapSys::ActivateConn(CapCtermPair* aPair)
{
    CapCtermPair* pair = GetCpPair(aPair);
    if (pair != NULL) {
	if (aPair->IsActive() && !pair->IsActive()) {
	    pair->SetActive(ETrue);
	}
	else if (!aPair->IsActive() && pair->IsActive()) {
	    pair->SetActive(EFalse);
	}
    }
}

void CapSys::CpPairObs::OnToggled(CagToggleButton* aBtn)
{
}

void CapSys::OnCompCpPairToggled(CapComp* aComp, CapCtermPair* aPair)
{
    ActivateConn(aPair);
}

void CapSys::OnCompNameClicked(CapComp* aComp)
{
    if (iObserver != NULL) {
	iObserver->OnCompSelected(&(aComp->iComp));
    }
}

void CapSys::OnCompParentClicked(CapComp* aComp)
{
    if (iObserver != NULL) {
	iObserver->OnSystSelected(aComp->iComp.TypeName());
    }
}

CapCtermPair* CapSys::GetCpPair(CapCtermPair* aPair)
{
    CapCtermPair* res = NULL;
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    for (map<CAE_StateBase*, CapState*>::iterator it = iStates.begin(); it != iStates.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutputs.begin(); it != iOutputs.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInputs.begin(); it != iInputs.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    return res;
}

void CapSys::OnStateCpPairToggled(CapState* aComp, CapCtermPair* aPair)
{
    ActivateConn(aPair);
}

void CapSys::OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair)
{
    ActivateConn(aPair);
}

TBool CapSys::OnDragDrop(GdkDragContext *drag_context, gint x, gint y, guint time)
{
}

void CapSys::OnDragDataReceived(GdkDragContext *drag_context, gint x, gint y, GtkSelectionData *data, guint info, guint time)
{
    if (info == KTei_NewObject) {
	//gdk_drag_status(drag_context, GDK_ACTION_COPY, time);
	AddComponent();
	gtk_drag_finish(drag_context, true, false, time);
    }
}

void CapSys::AddComponent() 
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smut = cpx->Mut().Root();
    CAE_ChromoNode mutadd = smut.AddChild(ENt_MutAdd);
    CAE_ChromoNode comp = mutadd.AddChild(ENt_Object);
    comp.SetAttr(ENa_Type, "none");
    char *name = (char*) malloc(100);
    sprintf(name, "noname_%d", rand());
    comp.SetAttr(ENa_Id, name);
    free(name);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::Refresh()
{
    // Remove all elements
    Remove(iHead);
    iHead = NULL;
    for (map<CAE_StateBase*, CapState*>::iterator it = iStates.begin(); it != iStates.end(); it++) {
	Remove(it->second);
    }
    iStates.erase(iStates.begin(), iStates.end());
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	Remove(it->second);
    }
    iComps.erase(iComps.begin(), iComps.end());
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInputs.begin(); it != iInputs.end(); it++) {
	Remove(it->second);
    }
    iInputs.erase(iInputs.begin(), iInputs.end());
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutputs.begin(); it != iOutputs.end(); it++) {
	Remove(it->second);
    }
    iOutputs.erase(iOutputs.begin(), iOutputs.end());
    // Construct again
    Construct();
}

