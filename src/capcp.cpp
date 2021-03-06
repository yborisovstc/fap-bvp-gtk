
#include "capcp.h"
#include "capcterm.h"
#include "capcommon.h"


const char* KCpPmenu_Del = "Del";

vector<TPmenuSpecElem> CapCp::iPmenuSpec;

CapCp::CapCp(const string& aName, CAE_ConnPointBase& aCp, TBool aExt, TBool aLeft, TBool aLineSep, TBool aNoLabel): 
    CagLayout(aName), iCp(aCp), iLeft(aLeft), iLabel(NULL), iLineSep(aLineSep), iCpObs(NULL), iNoLabel(aNoLabel), iExt(aExt)
{
    if (iPmenuSpec.empty()) {
	iPmenuSpec.push_back(TPmenuSpecElem(KCpPmenu_Del, "Delete"));
    }
    // Popup Menu
    iPopupMenu = new CapPopupMenu("Menu", iPmenuSpec);
    iPopupMenu->SetTitle("cont menu");
    iPopupMenu->Show();
    iPopupMenu->SetMenuShellObs(this);

    // Create label
    iLabel = new CapEopEntry("Label");
    iLabel->SetText(iCp.Name());
    iLabel->SetHasFrame(EFalse);
    GtkBorder name_brd = (GtkBorder) {0, 0, 0, 0};
    iLabel->SetInnerBorder(&name_brd);
    iLabel->SetWidthChars(iCp.Name().size());
    iLabel->SetObserver(this);
    Add(iLabel);
    iLabel->Show();
    if (iNoLabel) {
	GtkRequisition lab_req; iLabel->SizeRequest(&lab_req);
	iLabel->SetSizeRequest(0, lab_req.height);
    }
    // Set as source for DnD
    CAE_Object* obj = iCp.Man().GetFbObj(obj);
    TNodeType type = obj != NULL ? ENt_Object : ENt_State;
    iLabel->DragSourceAdd(GDK_MODIFIER_MASK, KTe_Conn, KTe_Conn_Len, GDK_ACTION_COPY);
    DragDestAdd(GTK_DEST_DEFAULT_ALL, KTe_Conn, KTe_Conn_Len, GDK_ACTION_COPY);
    if (aExt) {
	// Don't specify man if cp is local extention
	iLabel->SetSelText(iCp.Name());
    }
    else {
	//iLabel->SetSelText(MAE_Chromo::GetTName(type, string(iCp.Man().Name()) + "." + iCp.Name()));
	DesUri cpuri(&iCp, iCp.Owner()->Owner());
	iLabel->SetSelText(cpuri.GetUri());
    }
    // Create terminator
    iTerm = new CapCterm("Term", iCp, iExt, iLeft);
    Add(iTerm);
    iTerm->SetObs(this);
    GtkRequisition lab_req; iLabel->SizeRequest(&lab_req);
    iTerm->SetItemHeightHint(lab_req.height + KViewBtnHeightFromHintVar);
    iTerm->Show();
    // Set as source for DnD
    iTerm->DragDestAdd(GTK_DEST_DEFAULT_ALL, KTe_Conn, KTe_Conn_Len, GDK_ACTION_COPY);
}

CapCp::~CapCp()
{
}

int CapCp::GetLabelWidth() const
{
    GtkRequisition lab_req = (GtkRequisition) {0, 0}; 
    if (iLabel != NULL) {
	iLabel->SizeRequest(&lab_req);
    }
    return lab_req.width + KViewConnLabelGapWidth; 
}

int CapCp::GetTermWidth()
{
    GtkRequisition req = (GtkRequisition) {0, 0}; 
    SizeRequest(&req);
    return req.width - GetLabelWidth();
}

void CapCp::OnExpose(GdkEventExpose* aEvent)
{
    GtkAllocation alc; Allocation(&alc);
    GtkAllocation lab_alc = (GtkAllocation) {0, 0, 0, 0};
    if (iLabel != NULL) {
	// Draw rectangle of label
	iLabel->Allocation(&lab_alc);
	gdk_draw_rectangle(BinWnd(), Gc(), FALSE, lab_alc.x, lab_alc.y, lab_alc.width - 1, lab_alc.height - 1);
    }
    // Draw Label separator
    if (iLineSep) { 
	gint x1, y1, x2, y2;
	if (iLeft) {
	    if (iLabel != NULL) {
		x1 = lab_alc.x - KViewConnLabelGapWidth; y1 = lab_alc.y; x2 = x1; y2 = y1 + alc.height;
	    }
	    else {
		x1 = alc.width; y1 = alc.y; x2 = x1; y2 = y1 + alc.height;
	    }
	}
	else {
	    if (iLabel != NULL) {
		x1 = lab_alc.x + lab_alc.width + KViewConnLabelGapWidth - 1; y1 = lab_alc.y; x2 = x1; y2 = y1 + alc.height;
	    }
	    else {
		x1 = 0; y1 = alc.y; x2 = x1; y2 = y1 + alc.height;
	    }
	}
	gdk_draw_line(BinWnd(), Gc(), x1, y1, x2, y2);
    }
    // Draw connection line
    gint x1, y1, x2, y2;
    if (iLeft) {
	x1 = lab_alc.x - KViewConnLabelGapWidth - KViewConnLineLen; y1 = lab_alc.y + lab_alc.height/2; x2 = x1 + KViewConnLineLen; y2 = y1;
    }
    else {
	x1 = lab_alc.x + lab_alc.width + KViewConnLabelGapWidth; y1 = lab_alc.y + lab_alc.height/2; x2 = x1 + KViewConnLineLen; y2 = y1;
    }
    gdk_draw_line(BinWnd(), Gc(), x1, y1, x2, y2);
}

TBool CapCp::OnButtonPress(GdkEventButton* aEvent)
{
    if (aEvent->button == 3) {
	// Popup context menu
	iPopupMenu->Popup(aEvent->button, aEvent->time);
	return ETrue;
    }
    return EFalse;
}

TBool CapCp::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapCp::OnSizeAllocate(GtkAllocation* aAlc)
{
    if (iLabel != NULL) {
	// Allocate size for label
	GtkRequisition lab_req; iLabel->SizeRequest(&lab_req);
	GtkAllocation lab_alc = (GtkAllocation) { iLeft ? aAlc->width - lab_req.width: 0, 0, lab_req.width, lab_req.height};
	iLabel->SizeAllocate(&lab_alc);
    }
    // Allocate size for term
    GtkRequisition term_req; iTerm->SizeRequest(&term_req);
    GtkAllocation term_alc = (GtkAllocation) { iLeft ? 0 : aAlc->width - term_req.width, 0, term_req.width, term_req.height};
    iTerm->SizeAllocate(&term_alc);
}

void CapCp::OnSizeRequest(GtkRequisition* aReq)
{
    // Get size request for label
    GtkRequisition lab_req = (GtkRequisition) {0, 0}; 
    if (iLabel != NULL) {
	iLabel->SizeRequest(&lab_req);
    }
    // Get size request for term
    GtkRequisition term_req; iTerm->SizeRequest(&term_req);
    *aReq = (GtkRequisition) {lab_req.width + KViewConnLabelGapWidth + term_req.width + KViewConnLineLen, max(lab_req.height, term_req.height)};
}

void CapCp::OnMotion(GdkEventMotion *aEvent)
{
}

void CapCp::OnEnter(GdkEventCrossing *aEvent)
{
}

void CapCp::OnLeave(GdkEventCrossing *aEvent)
{
}

void CapCp::OnStateChanged(GtkStateType state)
{
}

void CapCp::SetObs(MCapCpObserver* aObs)
{
    _FAP_ASSERT(iCpObs == NULL);
    iCpObs = aObs;
}

void CapCp::OnCpTermPairToggled(CapCtermPair* aPair)
{
    if (iCpObs != NULL) {
	iCpObs->OnCpPairToggled(this, aPair);
    }
}

void CapCp::OnCpTermAddPairRequested(CapCterm* aCpTerm, const string& aPairName)
{
    if (iCpObs != NULL) {
	iCpObs->OnCpAddPairRequested(this, aPairName);
    }
}

CapCtermPair* CapCp::GetCpPair(CapCtermPair* aPair)
{
    CapCtermPair* res = NULL;
    if (aPair->Pair() == &iCp) {
	  res = iTerm->GetCpPair(aPair);
    }
    return res;
}

void CapCp::OnUpdateCompleted(CapEopEntry* aEntry)
{
    if (iCpObs != NULL) {
	iCpObs->OnLabelRenamed(this, iLabel->GetText());
    }
}

void CapCp::OnCpTermDelPairRequested(CapCterm* aCpTerm, CapCtermPair* aPair)
{
    if (iCpObs != NULL) {
	iCpObs->OnCpDelPairRequested(this, aPair);
    }
}

void CapCp::OnItemActivated(CagMenuShell* aMenuShell, CagMenuItem* aItem)
{
    if (aMenuShell == iPopupMenu) {
	if (aItem->Name().compare(KCpPmenu_Del) == 0) {
	    if (iCpObs != NULL) {
		iCpObs->OnCpDelRequested(this);
	    }
	}
    }
}

void CapCp::OnDragDataReceived(GdkDragContext *drag_context, gint x, gint y, GtkSelectionData *data, guint info, guint time)
{
    iTerm->OnDragDataReceived(drag_context, x, y, data, info, time);
}

