#include "capstate.h"
#include "capcommon.h"
#include "caglabel.h"
#include "cagtextview.h"

CapStateHead::CapStateHead(const string& aName, CAE_StateBase& aState): CagLayout(aName), iState(aState)
{
    // Create Name
    iName = new CagLabel("Name");
    iName->SetText(iState.InstName());
    Add(iName);
    iName->Show();

    // Create Type
    iType = new CagLabel("Type");
    iType->SetText(string(" :: ") + iState.TypeName());
    Add(iType);
    iType->Show();
}

void CapStateHead::OnExpose(GdkEventExpose* aEvent)
{
    GtkAllocation alc; Allocation(&alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, 0, 0, alc.width - 1, alc.height - 1);
}

void CapStateHead::OnSizeAllocate(GtkAllocation* aAlc)
{
    // Name
    GtkRequisition name_req; iName->SizeRequest(&name_req);
    GtkAllocation name_alc = (GtkAllocation) {0, 0, name_req.width, name_req.height};
    iName->SizeAllocate(&name_alc);
    // Type
    GtkRequisition type_req; iType->SizeRequest(&type_req);
    GtkAllocation type_alc = (GtkAllocation) {name_alc.x + name_alc.width, 0, type_req.width, type_req.height};
    iType->SizeAllocate(&type_alc);
}

void CapStateHead::OnSizeRequest(GtkRequisition* aReq)
{
    // Name
    GtkRequisition name_req; iName->SizeRequest(&name_req);
    // Type
    GtkRequisition type_req; iType->SizeRequest(&type_req);
    *aReq = (GtkRequisition) { name_req.width + type_req.width, max(name_req.height, type_req.height)};
}





CapState::CapState(const string& aName, CAE_StateBase& aState): CagLayout(aName), iState(aState), iObs(NULL)
{
    // Add header
    iHead = new CapStateHead("Title", iState);
    Add(iHead);
    iHead->Show();
    // Add Inputs
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iState.Inputs().begin(); it != iState.Inputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp(cp->Name(), *cp, EFalse, ETrue);
	Add(cpw);
	cpw->SetObs(this);
	iInps[cp] = cpw;
	cpw->Show();
    }
    // Add output
    CAE_ConnPointBase* cp = iState.Output();
    CapCp* cpw = new CapCp(cp->Name(), *cp, ETrue, ETrue, ETrue);
    Add(cpw);
    cpw->SetObs(this);
    iOutps[cp] = cpw;
    cpw->Show();
    // Add trans
    iTrans = new CagTextView("Trans");
    GtkTextBuffer* buf = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(buf, iState.GetTrans().iETrans.c_str(), iState.GetTrans().iETrans.size());
    iTrans->SetBuffer(buf);
    iTrans->SetEditable(EFalse);
    Add(iTrans);
    iTrans->Show();
}

CapState::~CapState()
{
}

void CapState::SetObs(MCapStateObserver* aObs)
{
    _FAP_ASSERT(iObs == NULL);
    iObs = aObs;
}

int CapState::GetBodyCenterX() const
{
    return iBodyAlc.x + iBodyAlc.width/2;
}

void CapState::OnExpose(GdkEventExpose* aEvent)
{
    // Draw body rect
    GtkAllocation alc; Allocation(&alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, iBodyAlc.x, iBodyAlc.y, iBodyAlc.width - 1, iBodyAlc.height - 1);
    // Draw trans rect
    GtkAllocation tran_alc; iTrans->Allocation(&tran_alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, tran_alc.x -1 , tran_alc.y -1 , tran_alc.width + 1, tran_alc.height + 1);
}

void CapState::OnSizeAllocate(GtkAllocation* aAllc)
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

    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    GtkRequisition tran_req; iTrans->SizeRequest(&tran_req);

    // Calculate allocation of comp body
    iBodyAlc = (GtkAllocation) {outp_maxw - outp_maxw_l, 0, tran_req.width + KViewCompInpOutpGapWidth + inp_w + outp_w, aAllc->height};

    // Allocate header
    GtkAllocation head_alc = {iBodyAlc.x, iBodyAlc.y, iBodyAlc.width, head_req.height};
    iHead->SizeAllocate(&head_alc);

    // Allocate trans
    GtkAllocation tran_alc = 
	(GtkAllocation) {iBodyAlc.x + KViewStateTransBorder, head_alc.y + head_alc.height + KViewCompCpGapHeight, tran_req.width, tran_req.height};
    iTrans->SizeAllocate(&tran_alc);

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

void CapState::OnSizeRequest(GtkRequisition* aRequisition)
{
    // Calculate header size
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    // Calculate trans size
    GtkRequisition tran_req; iTrans->SizeRequest(&tran_req);
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

    aRequisition->width = max(head_req.width, tran_req.width + KViewStateTransBorder*2 + inp_w + outp_w + KViewCompInpOutpGapWidth); 
    aRequisition->height = head_req.height + max(max(inp_h, outp_h), tran_req.height) + KViewCompCpGapHeight;
}

CapCtermPair* CapState::GetCpPair(CapCtermPair* aPair)
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

void CapState::OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair)
{
    if (iObs != NULL) {
	iObs->OnStateCpPairToggled(this, aPair);
    }

}


