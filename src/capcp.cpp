
#include "capcp.h"
#include "capcterm.h"
#include "capcommon.h"

CapCp::CapCp(const string& aName, CAE_ConnPointBase& aCp, TBool aLeft, TBool aLineSep, TBool aNoLabel): 
    CagLayout(aName), iCp(aCp), iLeft(aLeft), iLabel(NULL), iLineSep(aLineSep), iCpObs(NULL), iNoLabel(aNoLabel)
{
    // Create label
    iLabel = new CagLabel("Label");
    iLabel->SetText(iCp.Name());
    Add(iLabel);
    iLabel->Show();
    if (iNoLabel) {
	GtkRequisition lab_req; iLabel->SizeRequest(&lab_req);
	iLabel->SetSizeRequest(0, lab_req.height);
    }
    // Create terminator
    iTerm = new CapCterm("Term", iCp, iLeft);
    Add(iTerm);
    iTerm->SetObs(this);
    GtkRequisition lab_req; iLabel->SizeRequest(&lab_req);
    iTerm->SetItemHeightHint(lab_req.height + KViewBtnHeightFromHintVar);
    iTerm->Show();
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
    return lab_req.width; 
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
    if (iLineSep) { 
	// Draw Label separator
	gint x1, y1, x2, y2;
	if (iLeft) {
	    if (iLabel != NULL) {
		x1 = lab_alc.x; y1 = lab_alc.y; x2 = x1; y2 = y1 + alc.height;
	    }
	    else {
		x1 = alc.width; y1 = alc.y; x2 = x1; y2 = y1 + alc.height;
	    }
	}
	else {
	    if (iLabel != NULL) {
		x1 = lab_alc.x + lab_alc.width - 1; y1 = lab_alc.y; x2 = x1; y2 = y1 + alc.height;
	    }
	    else {
		x1 = 0; y1 = alc.y; x2 = x1; y2 = y1 + alc.height;
	    }
	}
	gdk_draw_line(BinWnd(), Gc(), x1, y1, x2, y2);
    }
}

TBool CapCp::OnButtonPress(GdkEventButton* aEvent)
{
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
    *aReq = (GtkRequisition) {lab_req.width + term_req.width + KViewConnLineLen, max(lab_req.height, term_req.height)};
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

void CapCp::OnCpPairToggled(CapCtermPair* aPair)
{
    if (iCpObs != NULL) {
	iCpObs->OnCpPairToggled(this, aPair);
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
