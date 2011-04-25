
#include "capsys.h"
#include "cagproxy.h"
#include "capcomp.h"
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




CapSys::CapSys(const string& aName, CAE_Object::Ctrl& aSys, MagSysObserver* aObserver): 
    CagLayout(aName), iSys(aSys), iObserver(aObserver), iCpPairObs(*this)
{
    // Add header
    iHead = new CapSysHead("Title", iSys);
    Add(iHead);
    iHead->Show();
    // Add components
    for (vector<CAE_EBase*>::iterator it = iSys.CompReg().begin(); it != iSys.CompReg().end(); it++) {
	CAE_Object* obj = (*it)->GetFbObj(obj);
	if (obj != NULL) {
	    CapComp* comp = new CapComp("comp." + string(obj->InstName()), *obj);
	    Add(comp);
	    iComps[obj] = comp;
	    comp->SetObs(this);
	    comp->Show();
	}
    }
    // Add inputs
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iSys.Object().Inputs().begin(); it != iSys.Object().Inputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp("Inp." + cp->Name(), *cp, ETrue);
	Add(cpw);
	iInputs[cp] = cpw;
	cpw->Show();
    }
    // Add outputs
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iSys.Object().Outputs().begin(); it != iSys.Object().Outputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp("Outp." + cp->Name(), *cp, EFalse);
	Add(cpw);
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

TBool CapSys::OnButtonPress(GdkEventButton* aEvent)
{
}

TBool CapSys::OnButtonRelease(GdkEventButton* aEvent)
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
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInputs.begin(); it != iInputs.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	GtkAllocation allc = {inpb_x, inpb_y, req.width, req.height};
	cpw->SizeAllocate(&allc);
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
    // Allocate components
    int compb_x = (outp_w + KViewConnGapHeight + aAllc->width)/2, compb_y = head_req.height + KViewCompGapHight;
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

    aRequisition->width = outp_w + KViewExtAreaWidth*2 + comp_w + inp_w; 
    aRequisition->height = head_req.height + max(max(outp_h, comp_h), inp_h);
}

void CapSys::OnMotion(GdkEventMotion *aEvent)
{
}

void CapSys::OnEnter(GdkEventCrossing *aEvent)
{
}

void CapSys::OnLeave(GdkEventCrossing *aEvent)
{
}

void CapSys::OnStateChanged(GtkStateType state)
{
}

void CapSys::OnChildStateChanged(CagWidget* aChild, GtkStateType aPrevState)
{
    if (aChild->Name().compare("Title") == 0) {
	if (aChild == iHead) {
	    iObserver->OnHeadSelected();
	}
	else {
	    CapComp* comp = Comp(aChild->iParent);
	    if (comp != NULL) {
		iObserver->OnCompSelected(&(comp->iComp));
	    }
	}
    }
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

void CapSys::CpPairObs::OnToggled(CagToggleButton* aBtn)
{
}

void CapSys::OnCompCpPairToggled(CapComp* aComp, CapCtermPair* aPair)
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

CapCtermPair* CapSys::GetCpPair(CapCtermPair* aPair)
{
    CapCtermPair* res = NULL;
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    return res;
}

