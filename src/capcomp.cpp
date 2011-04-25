#include "capcomp.h"
#include "capcommon.h"
#include "capcomphead.h"


CapComp::CapComp(const string& aName, CAE_Object& aComp): CagLayout(aName), iComp(aComp), iObs(NULL)
{
    // Add header
    iHead = new CapCompHead("Title", iComp);
    Add(iHead);
    iHead->Show();
    // Add Inputs
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iComp.Inputs().begin(); it != iComp.Inputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp(cp->Name(), *cp, EFalse, ETrue);
	Add(cpw);
	cpw->SetObs(this);
	iInps[cp] = cpw;
	cpw->Show();
    }
    // Add outputs
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iComp.Outputs().begin(); it != iComp.Outputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp(cp->Name(), *cp, ETrue, ETrue);
	Add(cpw);
	cpw->SetObs(this);
	iOutps[cp] = cpw;
	cpw->Show();
    }
}

CapComp::~CapComp()
{
}

void CapComp::SetObs(MCapCompObserver* aObs)
{
    _FAP_ASSERT(iObs == NULL);
    iObs = aObs;
}

int CapComp::GetInpTermY(CAE_ConnPointBase* aCp)
{
    _FAP_ASSERT(iInps.count(aCp) > 0);
    GtkAllocation alc; iInps[aCp]->Allocation(&alc);
    return alc.y + alc.height/2;
}

int CapComp::GetOutpTermY(CAE_ConnPointBase* aCp)
{
    _FAP_ASSERT(iOutps.count(aCp) > 0);
    GtkAllocation alc; iOutps[aCp]->Allocation(&alc);
    return alc.y + alc.height/2;
}

int CapComp::GetBodyCenterX() const
{
    return iBodyAlc.x + iBodyAlc.width/2;
}

void CapComp::OnExpose(GdkEventExpose* aEvent)
{
    GtkAllocation alc; Allocation(&alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, iBodyAlc.x, iBodyAlc.y, iBodyAlc.width - 1, iBodyAlc.height - 1);
}

TBool CapComp::OnButtonPress(GdkEventButton* aEvent)
{
}

TBool CapComp::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapComp::OnSizeAllocate(GtkAllocation* aAllc)
{
     // Calculate inputs lables width
    TInt inp_w = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end(); it++) {
	CapCp* cpw = it->second;
	inp_w = max(inp_w, cpw->GetLabelWidth());
    }
     // Calculate outputs labels width
    TInt outp_w = 0;
    int outp_maxw = 0; // Max output with
    int outp_maxw_l = 0; // Label width of output with max width
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end(); it++) {
	CapCp* cpw = it->second;
	outp_w = max(outp_w, cpw->GetLabelWidth());
	GtkRequisition outp_req; cpw->SizeRequest(&outp_req);
	outp_maxw = max(outp_maxw, outp_req.width);
	if (outp_req.width == outp_maxw)
	    outp_maxw_l = cpw->GetLabelWidth();
    }
    // Calculate allocation of comp body
    iBodyAlc = (GtkAllocation) {outp_maxw - outp_maxw_l, 0, KViewCompInpOutpGapWidth + inp_w + outp_w, aAllc->height};

    // Allocate header
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    GtkAllocation head_alc = {iBodyAlc.x, iBodyAlc.y, iBodyAlc.width, head_req.height};
    iHead->SizeAllocate(&head_alc);

    // Allocate inputs size 
    TInt inpb_x = aAllc->width - inp_w, inpb_y = head_alc.height + KViewCompCpGapHeight;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end(); it++) {
	CapCp* cpw = it->second;
	int lab_w = cpw->GetLabelWidth();
	GtkRequisition inp_req; cpw->SizeRequest(&inp_req);
	GtkAllocation inp_alc = {iBodyAlc.x + iBodyAlc.width - lab_w, inpb_y, inp_req.width, inp_req.height};
	cpw->SizeAllocate(&inp_alc);
	inpb_y += inp_req.height + KViewCompCpGapHeight;
    }
    // Allocate outputs size 
    TInt outpb_x = 0, outpb_y = head_alc.height + KViewCompCpGapHeight;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end(); it++) {
	CapCp* cpw = it->second;
	int lab_w = cpw->GetLabelWidth();
	GtkRequisition outp_req; cpw->SizeRequest(&outp_req);
	GtkAllocation outp_alc = {iBodyAlc.x + lab_w - outp_req.width, outpb_y, outp_req.width, outp_req.height};
	cpw->SizeAllocate(&outp_alc);
	outpb_y += outp_req.height + KViewCompCpGapHeight;
    }
}

void CapComp::OnSizeRequest(GtkRequisition* aRequisition)
{
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    // Calculate inputs size
    TInt inp_w = 0, inp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition inp_req; cpw->SizeRequest(&inp_req);
	inp_w = max(inp_w, inp_req.width);
	inp_h += inp_req.height + KViewCompCpGapHeight;
    }
    // Calculate outputs size
    TInt outp_w = 0, outp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition outp_req; cpw->SizeRequest(&outp_req);
	outp_w = max(outp_w, outp_req.width);
	outp_h += outp_req.height + KViewCompCpGapHeight;
    }

    aRequisition->width = max(head_req.width, inp_w + outp_w + KViewCompInpOutpGapWidth); 
    aRequisition->height = head_req.height + max(inp_h, outp_h) + KViewCompCpGapHeight;
}

void CapComp::OnMotion(GdkEventMotion *aEvent)
{
}

void CapComp::OnEnter(GdkEventCrossing *aEvent)
{
}

void CapComp::OnLeave(GdkEventCrossing *aEvent)
{
}

void CapComp::OnStateChanged(GtkStateType state)
{
}

void CapComp::OnChildStateChanged(CagWidget* aChild, GtkStateType aPrevState)
{
    if (aChild == iHead) {
	iParent->OnChildStateChanged(aChild, aPrevState);
    }
}

void CapComp::OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair)
{
    if (iObs != NULL) {
	iObs->OnCompCpPairToggled(this, aPair);
    }

}

CapCtermPair* CapComp::GetCpPair(CapCtermPair* aPair)
{
    CapCtermPair* res = NULL;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    return res;
}

