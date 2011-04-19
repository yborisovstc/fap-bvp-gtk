#include "capcomp.h"
#include "capcommon.h"
#include "capcomphead.h"
#include "capcp.h"



CapComp::CapComp(const string& aName, CAE_Object& aComp): CagLayout(aName), iComp(aComp)
{
    // Add header
    iHead = new CapCompHead("Title", iComp);
    Add(iHead);
    iHead->Show();
    // Add Inputs
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iComp.Inputs().begin(); it != iComp.Inputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp(cp->Name(), *cp);
	Add(cpw);
	iInps[cp] = cpw;
	cpw->Show();
    }
    // Add outputs
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iComp.Outputs().begin(); it != iComp.Outputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp(cp->Name(), *cp);
	Add(cpw);
	iOutps[cp] = cpw;
	cpw->Show();
    }
}

CapComp::~CapComp()
{
}


void CapComp::OnExpose(GdkEventExpose* aEvent)
{
    GtkAllocation alc; Allocation(&alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, 0, 0, alc.width - 1, alc.height - 1);
}

TBool CapComp::OnButtonPress(GdkEventButton* aEvent)
{
}

TBool CapComp::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapComp::OnSizeAllocate(GtkAllocation* aAllc)
{
    // Allocate header
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    int head_w = max(aAllc->width, head_req.width);
    GtkAllocation head_alc = {0, 0, head_w, head_req.height};
    iHead->SizeAllocate(&head_alc);
     // Calculate inputs width
    TInt inp_w = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition inp_req; cpw->SizeRequest(&inp_req);
	inp_w = max(inp_w, inp_req.width);
    }
     // Calculate outputs width
    TInt outp_w = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition outp_req; cpw->SizeRequest(&outp_req);
	outp_w = max(outp_w, outp_req.width);
    }

   // Allocate inputs size 
    TInt inpb_x = aAllc->width - inp_w, inpb_y = head_alc.height + KViewCompCpGapHeight;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition inp_req; cpw->SizeRequest(&inp_req);
	GtkAllocation inp_alc = {inpb_x, inpb_y, inp_w, inp_req.height};
	cpw->SizeAllocate(&inp_alc);
	inpb_y += inp_req.height + KViewCompCpGapHeight;
    }
   // Allocate outputs size 
    TInt outpb_x = 0, outpb_y = head_alc.height + KViewCompCpGapHeight;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition outp_req; cpw->SizeRequest(&outp_req);
	GtkAllocation outp_alc = {outpb_x, outpb_y, outp_w, outp_req.height};
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
    aRequisition->height = head_req.height + max(inp_h, outp_h);
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


