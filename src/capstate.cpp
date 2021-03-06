#include "capstate.h"
#include "capcommon.h"
#include "caglabel.h"
#include "cagtextview.h"
#include "caplogdlg.h"


const char* KStatePmenu_Del = "Del";
const char* KStatePmenu_AddInp = "AddInp";
const char* KStatePmenu_Log = "Log";



CapStatePopupMenu::CapStatePopupMenu(const string& aName, const vector<TPmenuSpecElem>& aSpec): CagMenu(aName)
{
    for (vector<TPmenuSpecElem>::const_iterator it = aSpec.begin(); it != aSpec.end(); it++) {
	const TPmenuSpecElem& spec = *it;
	CagMenuItem* itm_del = new CagMenuItem(spec.iName, spec.iLabel);
	itm_del->Show();
	Append(itm_del);
    }
}


CapStateHead::CapStateHead(const string& aName, CAE_StateBase& aState): CagHBox(aName), iState(aState), iObs(NULL)
{
    SetBorderWidth(1);
    // Create Name
    iName = new CapEopEntry("Name");
    iName->SetHasFrame(EFalse);
    GtkBorder name_brd = (GtkBorder) {0, 4, 0, 0};
    iName->SetInnerBorder(&name_brd);
    iName->SetWidthChars(iState.Name().size());
    iName->SetText(iState.Name());
    iName->SetObserver(this);
    iName->Show();
    PackStart(iName, false, false, 2);
    // Create Type
    iType = new CapEopEntry("Type");
    iType->SetHasFrame(EFalse);
    iType->SetWidthChars(strlen(iState.TypeName()));
    iType->SetText(iState.TypeName());
    iType->SetObserver(this);
    iType->Show();
    PackStart(iType, false, false, 2);
}

void CapStateHead::SetObserver(MStateHeadObserver* aObs)
{
    _FAP_ASSERT(iObs == NULL);
    iObs = aObs;
}

void CapStateHead::OnUpdateCompleted(CapEopEntry* aEntry)
{
    if (iObs != NULL) {
	if (aEntry == iName) {
	    iObs->OnStateNameChanged(iName->GetText());
	}
	else if (aEntry == iType) {
	    iObs->OnStateTypeChanged(iType->GetText());
	}
    }
}

vector<TPmenuSpecElem> CapState::iPmenuSpec;


CapState::CapState(const string& aName, CAE_StateBase& aState, CAE_Object::Ctrl& aOwner): 
    CagLayout(aName), iState(aState), iObs(NULL), iOwner(aOwner)
{
    if (iPmenuSpec.empty()) {
	iPmenuSpec.push_back(TPmenuSpecElem(KStatePmenu_Del, "Delete"));
	iPmenuSpec.push_back(TPmenuSpecElem(KStatePmenu_AddInp, "Add Input"));
	iPmenuSpec.push_back(TPmenuSpecElem(KStatePmenu_Log, "Logging"));
    }
    // Popup Menu
    iPopupMenu = new CapStatePopupMenu("Menu", iPmenuSpec);
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
    iTrans->SetBorderWidth(KViewStateTransIntBorder);
    GtkTextBuffer* buf = gtk_text_buffer_new(NULL);
    // TODO [YB] Hack
    const TTransInfo& tinfo = iState.GetTrans(); 
    if (tinfo.iETrans.empty()) {
	string buftxt("   ");
	gtk_text_buffer_set_text(buf, buftxt.c_str(), buftxt.size());
    }
    else {
	gtk_text_buffer_set_text(buf, tinfo.iETrans.c_str(), tinfo.iETrans.size());
    }
    iTrans->SetBuffer(buf);
    iTrans->SetEditable(ETrue);
    Add(iTrans);
    iTrans->Show();
    iTrans->SetWidgetObs(this);
    // Add Value
    iValue = new CagTextView("Value");
    GtkTextBuffer* valuebuf = gtk_text_buffer_new(NULL);
    const string& val = iState.ValStr();
    // TODO [YB] Hack
    if (val.empty()) {
	string buftxt("   ");
	gtk_text_buffer_set_text(valuebuf, buftxt.c_str(), buftxt.size());
    }
    else {
	gtk_text_buffer_set_text(valuebuf, val.c_str(), val.size());
    }
    iValue->SetBuffer(valuebuf);
    iValue->SetEditable(ETrue);
    Add(iValue);
    iValue->Show();
    iValue->SetWidgetObs(this);
    // Add init
    iInit = new CagTextView("Init");
    GtkTextBuffer* initbuf = gtk_text_buffer_new(NULL);
    // TODO [YB] There is the problem with attr not presenting in run-time when wrapped appr is used (ref md#sec_desg_chromo_full_sol_1)
    // -- to consider if it makes sense to move such attr in run-time
#if 0
    CAE_Object::ChromoPx* cpx = iOwner.Object().ChromoIface();
    CAE_ChromoNode chroot = cpx->Chr().Root();
    CAE_ChromoNode::Iterator chrstit = chroot.Find(ENt_State, iState.Name());
    _FAP_ASSERT(chrstit != chroot.End());
    CAE_ChromoNode chrst = *chrstit;
    const string& init = chrst.Attr(ENa_StInit);
#endif
    const string& init = iState.Init();
    // TODO [YB] Hack
    if (init.empty()) {
	string buftxt("   ");
	gtk_text_buffer_set_text(initbuf, buftxt.c_str(), buftxt.size());
    }
    else {
	gtk_text_buffer_set_text(initbuf, init.c_str(), init.size());
    }
    iInit->SetBuffer(initbuf);
    iInit->SetEditable(ETrue);
    Add(iInit);
    iInit->Show();
    iInit->SetWidgetObs(this);
}

CapState::~CapState()
{
}

void* CapState::DoGetObj(const char *aName)
{
    if (strcmp(aName, MWidgetObs::Type()) == 0)
	return (MWidgetObs*) this;
    else if (strcmp(aName, Type()) == 0) 
	return this;
    else return CagLayout::DoGetObj(aName);
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
    // Draw value rect
    GtkAllocation val_alc; iValue->Allocation(&val_alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, val_alc.x -1 , val_alc.y -1 , val_alc.width + 1, val_alc.height + 1);
    // Draw init rect
    GtkAllocation init_alc; iInit->Allocation(&init_alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, init_alc.x -1 , init_alc.y -1 , init_alc.width + 1, init_alc.height + 1);
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
    TInt outp_term_w = 0, outp_lab_w = 0, outp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end(); it++) {
	CapCp* cpw = it->second;
	outp_term_w = max(outp_term_w, cpw->GetTermWidth());
	outp_lab_w = max(outp_lab_w, cpw->GetLabelWidth());
	GtkRequisition outp_req; cpw->SizeRequest(&outp_req);
	outp_h += outp_req.height + KViewCompCpGapHeight;
    }

    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    GtkRequisition value_req; iValue->SizeRequest(&value_req);
    GtkRequisition init_req; iInit->SizeRequest(&init_req);
    GtkRequisition tran_req; iTrans->SizeRequest(&tran_req);

    // Calculate allocation of comp body
    int body_int_width = max(tran_req.width + KViewStateTransBorder*2 , max(init_req.width, value_req.width));
    int body_width = max(head_req.width, body_int_width + inp_lab_w + outp_lab_w + KViewCompInpOutpGapWidth);
    iBodyAlc = (GtkAllocation) {outp_term_w, 0, body_width, aAllc->height};

    // Allocate header
    GtkAllocation head_alc = {iBodyAlc.x, iBodyAlc.y, iBodyAlc.width, head_req.height};
    iHead->SizeAllocate(&head_alc);

    // Allocate value
    GtkAllocation val_alc = 
	(GtkAllocation) {iBodyAlc.x + KViewStateTransBorder, head_alc.y + head_alc.height + KViewCompCpGapHeight, 
	    value_req.width, value_req.height};
    iValue->SizeAllocate(&val_alc);

    // Allocate init
    GtkAllocation init_alc = 
	(GtkAllocation) {iBodyAlc.x + KViewStateTransBorder, val_alc.y + val_alc.height + KViewCompCpGapHeight, 
	    init_req.width, init_req.height};
    iInit->SizeAllocate(&init_alc);

    // Allocate trans
    GtkAllocation tran_alc = 
	(GtkAllocation) {iBodyAlc.x + KViewStateTransBorder, init_alc.y + init_alc.height + KViewCompCpGapHeight, tran_req.width, tran_req.height};
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
    // Calculate value size
    GtkRequisition value_req; iValue->SizeRequest(&value_req);
    // Calculate init size
    GtkRequisition init_req; iInit->SizeRequest(&init_req);
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

    int body_int_width = max(tran_req.width + KViewStateTransBorder*2 , max(init_req.width, value_req.width));
    int body_width = max(head_req.width, body_int_width + inp_lab_w + outp_lab_w + KViewCompInpOutpGapWidth);
    aRequisition->width = outp_term_w + body_width + inp_term_w; 
    aRequisition->height = head_req.height + 
	max(inp_h, max(outp_h, value_req.height) + tran_req.height + init_req.height + 2*KViewCompCpGapHeight) + 2*KViewCompCpGapHeight;
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

void CapState::OnStateTypeChanged(const string& aName)
{
    if (iObs != NULL) {
	iObs->OnStateTypeChanged(this, aName);
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
	    if (aItem->Name().compare(KStatePmenu_Del) == 0) {
		iObs->OnStateDeleteRequested(this);
	    }
	    else if (aItem->Name().compare(KStatePmenu_AddInp) == 0) {
		iObs->OnStateAddingInput(this);
	    }
	    else if (aItem->Name().compare(KStatePmenu_Log) == 0) {
		CapLogDlg* dlg = new CapLogDlg("LogDlg", iState);
		TInt res = dlg->Run();
		if (res == CapLogDlg::EActionOK) {
		    map<TLeBase, TInt> logspec;
		    dlg->GetLogSpecUpdate(logspec);
		    iObs->OnStateLogspecChanged(this, logspec);
		}
		delete dlg;
	    }
	}
    }
}

void CapState::OnLabelRenamed(CapCp* aCp, const string& aName)
{
    if (iObs != NULL) {
	iObs->OnStateInpRenamed(this, aCp, aName);
    }
}

void CapState::OnCpAddPairRequested(CapCp* aCp, const string& aPairName)
{
    if (iObs != NULL) {
	iObs->OnStateCpAddPairRequested(this, aCp, aPairName);
    }
}


TBool CapState::OnWidgetFocusOut(CagWidget* aWidget, GdkEventFocus* aEvent)
{
    TBool res = EFalse;
    if (aWidget == iTrans) {
	if (iObs != NULL) {
	    iObs->OnStateTransUpdated(this, iTrans->GetBuffer());
	}
    }
    else if (aWidget == iInit) {
	if (iObs != NULL) {
	    iObs->OnStateInitUpdated(this, iInit->GetBuffer());
	}
    }
    return res;
}

void CapState::OnCpDelPairRequested(CapCp* aCp, CapCtermPair* aPair)
{
    if (iObs != NULL) {
	iObs->OnStateCpDelPairRequested(this, aCp, aPair);
    }
}
void CapState::OnCpDelRequested(CapCp* aCp)
{
    if (iObs != NULL) {
	iObs->OnStateNodeDelRequested(this, &aCp->iCp);
    }
}


