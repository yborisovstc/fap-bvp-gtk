#include "capcomp.h"
#include "capcommon.h"

CapCompHead::CapCompHead(const string& aName, CAE_Object& aComp): CagHBox(aName), iComp(aComp), iObs(NULL)
{
    SetBorderWidth(1);
    // Create Name
    iName = new CapEopEntry("Name");
    iName->SetHasFrame(EFalse);
    GtkBorder name_brd = (GtkBorder) {5, 10, 0, 0};
    iName->SetInnerBorder(&name_brd);
    iName->SetWidthChars(strlen(iComp.InstName()));
    iName->SetText(iComp.InstName());
    iName->SetWidgetObs(this);
    iName->SetObserver(this);
    iName->Show();
    PackStart(iName, false, false, 2);
    // Create Parent
    iParent = new CagELabel("Parent");
    iParent->SetText(string(" <") + string((iComp.TypeName() == NULL) ? "no parent": iComp.TypeName()) + string(">"));
    iParent->Show();
    iParent->SetWidgetObs(this);
    PackStart(iParent, false, false, 2);
}

void CapCompHead::SetObserver(MCompHeadObserver* aObs)
{
    _FAP_ASSERT(iObs == NULL);
    iObs = aObs;
}

void CapCompHead::OnExpose(GdkEventExpose* aEvent)
{
    GtkAllocation alc; Allocation(&alc);
//    gdk_draw_rectangle(GdkWnd(), Gc(), FALSE, 0, 0, alc.width - 1, alc.height - 1);
}

TBool CapCompHead::OnWidgetButtonPress(CagWidget* aWidget, GdkEventButton* aEvent)
{
    if (iObs != NULL) {
	if (aWidget == iName) {
	    if (aEvent->type == GDK_2BUTTON_PRESS) {
		iObs->OnCompNameClicked();
	    }
	}
    }
    else 
	iObs->OnCompParentClicked();
}

void CapCompHead::OnUpdateCompleted(CapEopEntry* aEntry)
{
    if (iObs != NULL) {
	iObs->OnCompNameChanged(iName->GetText());
    }
}

void* CapCompHead::DoGetObj(const char *aName)
{
    if (strcmp(aName, MWidgetObs::Type()) == 0)
	return (MWidgetObs*) this;
    else if (strcmp(aName, Type()) == 0) 
	return this;
    else return CagHBox::DoGetObj(aName);
}


CapComp::CapComp(const string& aName, CAE_Object& aComp): CagLayout(aName), iComp(aComp), iObs(NULL)
{
    // Add header
    iHead = new CapCompHead("Title", iComp);
    iHead->SetObserver(this);
    Add(iHead);
    iHead->Show();
    // Debug
    GdkEventMask em;
    g_object_get(iHead->iWidget, "events", &em, NULL);
    gboolean hw = gtk_widget_get_has_window(iHead->iWidget);

    // Add Inputs
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iComp.Inputs().begin(); it != iComp.Inputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp("Inp~" + cp->Name(), *cp, EFalse, EFalse, ETrue);
	Add(cpw);
	cpw->SetObs(this);
	iInps[cp] = cpw;
	cpw->Show();
    }
    // Add outputs
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iComp.Outputs().begin(); it != iComp.Outputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp("Outp~" + cp->Name(), *cp, EFalse, ETrue, ETrue);
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
    // Border
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, iBodyAlc.x, iBodyAlc.y, iBodyAlc.width - 1, iBodyAlc.height - 1);
    // Head separator
    GtkAllocation head_alc; iHead->Allocation(&head_alc);
    gdk_draw_line(BinWnd(), Gc(), iBodyAlc.x, head_alc.height, iBodyAlc.x + iBodyAlc.width - 1, head_alc.height);
}

TBool CapComp::OnButtonPress(GdkEventButton* aEvent)
{
}

TBool CapComp::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapComp::OnSizeAllocate(GtkAllocation* aAllc)
{
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    // Calculate inputs terms and labels width
    int inp_h = 0;
    int iterm_maxw = 0; // Max term with
    int ilab_maxw = 0; // Max label width 
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	TInt labw = cpw->GetLabelWidth();
	ilab_maxw = max(ilab_maxw, labw);
	iterm_maxw = max(iterm_maxw, req.width - labw);
	inp_h += req.height + KViewCompCpGapHeight;
    }
    // Calculate outputs terms and labels width
    int outp_h = 0;
    int oterm_maxw = 0; // Max term with
    int olab_maxw = 0; // Max label width
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	TInt labw = cpw->GetLabelWidth();
	olab_maxw = max(olab_maxw, labw);
	oterm_maxw = max(oterm_maxw, req.width - labw);
	outp_h += req.height + KViewCompCpGapHeight;
    }

    // Calculate allocation of comp body
    iBodyAlc = (GtkAllocation) {oterm_maxw, 0, max(head_req.width, KViewCompInpOutpGapWidth + ilab_maxw + olab_maxw), aAllc->height};

    // Allocate header
    GtkAllocation head_alc = {iBodyAlc.x, iBodyAlc.y, iBodyAlc.width, head_req.height};
    iHead->SizeAllocate(&head_alc);

    // Allocate inputs size 
    TInt inpb_x = iBodyAlc.x + iBodyAlc.width, inpb_y = head_alc.height + KViewCompCpGapHeight;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end(); it++) {
	CapCp* cpw = it->second;
	int lab_w = cpw->GetLabelWidth();
	GtkRequisition inp_req; cpw->SizeRequest(&inp_req);
	GtkAllocation inp_alc = {inpb_x - lab_w, inpb_y, inp_req.width, inp_req.height};
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
    // Calculate inputs terms and labels width
    int inp_h = 0;
    int iterm_maxw = 0; // Max term with
    int ilab_maxw = 0; // Max label width 
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInps.begin(); it != iInps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	TInt labw = cpw->GetLabelWidth();
	ilab_maxw = max(ilab_maxw, labw);
	iterm_maxw = max(iterm_maxw, req.width - labw);
	inp_h += req.height + KViewCompCpGapHeight;
    }
    // Calculate outputs terms and labels width
    int outp_h = 0;
    int oterm_maxw = 0; // Max term with
    int olab_maxw = 0; // Max label width
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutps.begin(); it != iOutps.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	TInt labw = cpw->GetLabelWidth();
	olab_maxw = max(olab_maxw, labw);
	oterm_maxw = max(oterm_maxw, req.width - labw);
	outp_h += req.height + KViewCompCpGapHeight;
    }

    // Body width
    TInt body_w = max(head_req.width, KViewCompInpOutpGapWidth + ilab_maxw + olab_maxw);
    aRequisition->width = oterm_maxw + body_w + iterm_maxw; 
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

void CapComp::OnCompNameClicked()
{
    if (iObs != NULL) {
	iObs->OnCompNameClicked(this);
    }
}

void CapComp::OnCompParentClicked()
{
    if (iObs != NULL) {
	iObs->OnCompParentClicked(this);
    }
}

void CapComp::OnCompNameChanged(const string& aName)
{
    if (iObs != NULL) {
	iObs->OnCompNameChanged(this, aName);
    }
}

void CapComp::ChangeName(const string& aName)
{
    CAE_Object::ChromoPx* cpx = iComp.ChromoIface();
    CAE_ChromoNode smut = cpx->Mut().Root();
    CAE_ChromoNode chnode = smut.AddChild(ENt_MutChange);
    chnode.SetAttr(ENa_Id, aName);
    iComp.Mutate();
}

void CapComp::OnLabelRenamed(CapCp* aCp, const string& aName)
{
    RenameCp(aCp, aName);
}

void CapComp::OnCpAddPairRequested(CapCp* aCp, const string& aPairName)
{
    if (iObs != NULL) {
	iObs->OnCompCpAddPairRequested(this, aCp, aPairName);
    }
}

void CapComp::OnCpDelPairRequested(CapCp* aCp, CapCtermPair* aPair)
{
}

void CapComp::RenameCp(CapCp* aCp, const string& aName)
{
    TBool isoutp = iOutps.count(&(aCp->iCp));
    if (iObs != NULL) {
	iObs->OnCompCpRenamed(this, aCp, aName, isoutp);
    }
}

