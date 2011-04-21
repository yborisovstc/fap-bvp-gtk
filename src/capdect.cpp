
#include "capdect.h"
#include "cagbutton.h"
#include "capcommon.h"

CapDect::CapDect(const string& aName): CagLayout(aName), iObserver(NULL)
{
    // Create Less
    iLess = new CagButton("Less");
    Add(iLess);
    iLess->SetImage("dect_btn_less.png");
    iLess->Show();
    GtkSettings* sett = gtk_settings_get_default();
    GValue imgbut = {0};
    g_value_init(&imgbut, G_TYPE_BOOLEAN);
    g_object_get_property(G_OBJECT(sett), "gtk-button-images", &imgbut);
    gboolean imgbutb = g_value_get_boolean(&imgbut);

    GValue prop = {0};
    g_value_init(&prop, GTK_TYPE_BORDER);
    gtk_widget_style_get_property(iLess->iWidget, "inner-border", &prop);
    iLess->SetObs(this);
    // Create More
    iMore = new CagButton("More");
    Add(iMore);
    iMore->SetImage("dect_btn_more.png");
    iMore->Show();
    iMore->SetObs(this);
}

CapDect::~CapDect()
{
}

void* CapDect::DoGetObj(const char *aName)
{
    if (strcmp(aName, Type()) == 0)
	return this;
    else if (strcmp(aName, MCagButtonObs::Type()) == 0) 
	return (MCagButtonObs*) this;
    else 
	return CagLayout::DoGetObj(aName);
}

void CapDect::SetObserver(MDectObserver* aObs)
{
    _FAP_ASSERT(iObserver == NULL);
    iObserver = aObs;
}

void CapDect::OnExpose(GdkEventExpose* aEvent)
{
}

TBool CapDect::OnButtonPress(GdkEventButton* aEvent)
{
    if (aEvent->type == GDK_BUTTON_PRESS) {
	SetState(GTK_STATE_SELECTED);
    }
}

TBool CapDect::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapDect::OnSizeAllocate(GtkAllocation* aAllocation)
{
    // Allocate size for Less
    GtkRequisition less_req; iLess->SizeRequest(&less_req);
    GtkAllocation less_alc = { 0, 0, less_req.width, less_req.height};
    iLess->SizeAllocate(&less_alc);
    // Allocate size for More
    GtkRequisition more_req; iMore->SizeRequest(&more_req);
    GtkAllocation more_alc = { less_alc.width + KViewDectGapWidth, 0, more_req.width, more_req.height};
    iMore->SizeAllocate(&more_alc);
}

void CapDect::OnSizeRequest(GtkRequisition* aReq)
{
    GtkRequisition less_req; iLess->SizeRequest(&less_req);
    GtkRequisition more_req; iMore->SizeRequest(&more_req);
    *aReq = (GtkRequisition) {less_req.width + more_req.width + KViewDectGapWidth, max(less_req.height, more_req.height)};
}

void CapDect::OnMotion(GdkEventMotion *aEvent)
{
}

void CapDect::OnEnter(GdkEventCrossing *aEvent)
{
}

void CapDect::OnLeave(GdkEventCrossing *aEvent)
{
}

void CapDect::OnStateChanged(GtkStateType state)
{
}

void CapDect::OnClicked(CagButton* aBtn)
{
    if (aBtn == iLess) {
	iLevel--;
	HandleLevelChanged();
    }
    else {
	iLevel++;
	HandleLevelChanged();
    }
}

void CapDect::SetLevel(int aLevel)
{
    int prev = iLevel;
    iLevel == aLevel;
    if (iLevel < iLowerLim) {
	iLevel = iLowerLim;
    }
    else if (iLevel > iUpperLim) {
	iLevel = iUpperLim;
    }
    if (iLevel != prev) {
	HandleLevelChanged();
    }
}

void CapDect::SetLowerLim(int aLim)
{
    iLowerLim = aLim;
    if (iLevel < iLowerLim) {
	iLevel = iLowerLim;
	HandleLevelChanged();
    }
}

void CapDect::SetUpperLim(int aLim)
{
    iUpperLim = aLim;
    if (iLevel > iUpperLim) {
	iLevel = iUpperLim;
	HandleLevelChanged();
    }
}

void CapDect::HandleLevelChanged()
{
    if (iLevel == iLowerLim) {
	iLess->Hide();
	if (iLevel < iUpperLim && !iMore->IsVisible()) {
	    iMore->Show();
	}
    }
    else if (iLevel == iUpperLim) {
	iMore->Hide();
	if (iLevel > iLowerLim && !iLess->IsVisible()) {
	    iLess->Show();
	}
    }
    else {
	if (!iMore->IsVisible()) {
	    iMore->Show();
	}
	if (!iLess->IsVisible()) {
	    iLess->Show();
	}
    }
    NotifyLevelChanged();
}

void CapDect::NotifyLevelChanged()
{
    if (iObserver != NULL) {
	iObserver->OnDetLevelChanged(iLevel);
    }
}

