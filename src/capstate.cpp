#include "capstate.h"
#include "capcommon.h"
#include "caglabel.h"
#include "cagtextview.h"

CapStatePopupMenu::CapStatePopupMenu(const string& aName): CagMenu(aName)
{
    CagMenuItem* itm_del = new CagMenuItem("Del", "Delete");
    itm_del->Show();
    Append(itm_del);
}


CapStateHead::CapStateHead(const string& aName, CAE_StateBase& aState): CagHBox(aName), iState(aState), iObs(NULL)
{
    SetBorderWidth(1);
    // Create Name
    iName = new CapEopEntry("Name");
    iName->SetHasFrame(EFalse);
    GtkBorder name_brd = (GtkBorder) {0, 4, 0, 0};
    iName->SetInnerBorder(&name_brd);
    iName->SetWidthChars(strlen(iState.InstName()));
    iName->SetText(iState.InstName());
    iName->SetObserver(this);
    iName->Show();
    PackStart(iName, false, false, 2);
    // Create Type
    iType = new CagELabel("Type");
    iType->SetText(string(" :: ") + iState.TypeName());
    iType->Show();
    PackStart(iType, false, false, 2);
}

void CapStateHead::SetObserver(MStateHeadObserver* aObs)
{
    _FAP_ASSERT(iObs == NULL);
    iObs = aObs;
}

void CapStateHead::OnUpdateCompleted()
{
    if (iObs != NULL) {
	iObs->OnStateNameChanged(iName->GetText());
    }
}



CapState::CapState(const string& aName, CAE_StateBase& aState): CagLayout(aName), iState(aState), iObs(NULL)
{
    // Popup Menu
    iPopupMenu = new CapStatePopupMenu("Menu");
    iPopupMenu->SetTitle("cont menu");
    iPopupMenu->Show();
    iPopupMenu->SetMenuShellObs(this);
    // Add header
    iHead = new CapStateHead("Title", iState);
    iHead->SetObserver(this);
    Add(iHead);
    iHead->Show();
    // Add Inputs
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iState.Inputs().begin(); it != iState.Inputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp("Inp~" + cp->Name(), *cp, EFalse, EFalse, ETrue);
	Add(cpw);
	cpw->SetObs(this);
	iInps[cp] = cpw;
	cpw->Show();
    }
    // Add output
    CAE_ConnPointBase* cp = iState.Output();
    CapCp* cpw = new CapCp("Outp~" + cp->Name(), *cp, EFalse, ETrue, ETrue, ETrue);
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
    // Head separator
    GtkAllocation head_alc; iHead->Allocation(&head_alc);
    gdk_draw_line(BinWnd(), Gc(), iBodyAlc.x, head_alc.height, iBodyAlc.x + iBodyAlc.width - 1, head_alc.height);
    // Draw trans rect
    GtkAllocation tran_alc; iTrans->Allocation(&tran_alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, tran_alc.x -1 , tran_alc.y -1 , tran_alc.width + 1, tran_alc.height + 1);
}

void CapState::OnSizeAllocate(GtkAllocation* aAllc)
{
    // Calculate inputs size
    TInt inp_term_w = 0,  inp_lab_w = 0, inp_w = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end(); it++) {
	CapCp* cpw = it->second;
	inp_term_w = max(inp_term_w, cpw->GetTermWidth());
	inp_lab_w = max(inp_lab_w, cpw->GetLabelWidth());
    }
    inp_w = inp_term_w + inp_lab_w;
    // Calculate outputs size
    TInt outp_term_w = 0, outp_lab_w = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end(); it++) {
	CapCp* cpw = it->second;
	outp_term_w = max(outp_term_w, cpw->GetTermWidth());
	outp_lab_w = max(outp_lab_w, cpw->GetLabelWidth());
    }

    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    GtkRequisition tran_req; iTrans->SizeRequest(&tran_req);

    // Calculate allocation of comp body
    int body_width = max(head_req.width, tran_req.width + KViewStateTransBorder*2 + inp_lab_w + outp_lab_w + KViewCompInpOutpGapWidth);
    iBodyAlc = (GtkAllocation) {outp_term_w, 0, body_width, aAllc->height};

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
    TInt inp_term_w = 0, inp_h = 0, inp_lab_w = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition inp_req; cpw->SizeRequest(&inp_req);
	inp_term_w = max(inp_term_w, cpw->GetTermWidth());
	inp_lab_w = max(inp_lab_w, cpw->GetLabelWidth());
	inp_h += inp_req.height + KViewCompCpGapHeight;
    }
    // Calculate outputs size
    TInt outp_term_w = 0, outp_h = 0, outp_lab_w = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition outp_req; cpw->SizeRequest(&outp_req);
	outp_term_w = max(outp_term_w, cpw->GetTermWidth());
	outp_lab_w = max(outp_lab_w, cpw->GetLabelWidth());
	outp_h += outp_req.height + KViewCompCpGapHeight;
    }

    int body_width = max(head_req.width, tran_req.width + KViewStateTransBorder*2 + inp_lab_w + outp_lab_w + KViewCompInpOutpGapWidth);
    aRequisition->width = outp_term_w + body_width + inp_term_w; 
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

void CapState::OnStateNameChanged(const string& aName)
{
    if (iObs != NULL) {
	iObs->OnStateNameChanged(this, aName);
    }
}

TBool CapState::OnButtonPress(GdkEventButton* aEvent)
{
    if (aEvent->button == 3) {
	// Popup context menu
	iPopupMenu->Popup(aEvent->button, aEvent->time);
	return ETrue;
    }
    return EFalse;
}

void CapState::OnItemActivated(CagMenuShell* aMenuShell, CagMenuItem* aItem)
{
    if (iObs != NULL) {
	if (aMenuShell == iPopupMenu) {
	    if (aItem->Name().compare("Del") == 0) {
		iObs->OnStateDeleteRequested(this);
	    }
	}
    }
}

