
#include "capsys.h"
#include "cagproxy.h"
#include "capcomp.h"
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
    CagLayout(aName), iSys(aSys), iObserver(aObserver)
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
	    comp->Show();
	    // Add conn terms for component inputs
	    for (map<string, CAE_ConnPointBase*>::iterator iit = obj->Inputs().begin(); iit != obj->Inputs().end(); iit++) {
		CAE_ConnPointBase* cp = iit->second;
		CapCterm* cterm = new CapCterm("Cterm." + string(obj->InstName()) + ".Inp" + cp->Name(), *(iit->second), EFalse);
		iCterms[cp] = cterm;
		Add(cterm);
		cterm->Show();
	    }
	}
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
    // Allocate components
    int compb_x = aAllc->width / 2, compb_y = head_req.height + KViewCompGapHight;
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	CAE_Object* obj = it->first;
	CapComp* comp = it->second;
	GtkRequisition req; comp->SizeRequest(&req);
	GtkAllocation allc = {compb_x - req.width / 2, compb_y, req.width, req.height};
	comp->SizeAllocate(&allc);
	compb_y += req.height + KViewCompGapHight;
	// Allocate inputs conn terms for the comp
	for (map<string, CAE_ConnPointBase*>::iterator iit = obj->Inputs().begin(); iit != obj->Inputs().end(); iit++) {
	    int itermy = comp->GetInpTermY(iit->second);
	    CapCterm* ict = iCterms[iit->second];
	    GtkRequisition ict_req; ict->SizeRequest(&ict_req);
	    GtkAllocation ict_alc = {allc.x + allc.width + KViewConnLineLen, allc.y + itermy - ict_req.height/2, ict_req.width, ict_req.height};
	    ict->SizeAllocate(&ict_alc);
	}
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
	// Calculate Inputs conn terms size width for the comp
	// TODO [YB] To consider using of auxiliary map comp - cterms 
	int ct_w = 0;
	for (map<CAE_ConnPointBase*, CapCterm*>::iterator tit = iCterms.begin(); tit != iCterms.end(); tit++) {
	    CapCterm* ct = tit->second;
	    if (&(ct->Cp().Man()) == &(comp->Comp()) && !ct->IsLeft()) {
		GtkRequisition ctreq; ct->SizeRequest(&ctreq);
		ct_w = max(ct_w, ctreq.width);
	    }
	}
	comp_w = max(comp_w, req.width + ct_w);
	comp_h += req.height + KViewCompGapHight ;
    }
    aRequisition->width = comp_w; aRequisition->height = head_req.height + comp_h;
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

