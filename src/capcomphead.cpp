
#include "capcomphead.h"
#include "capcommon.h"

CapCompHead::CapCompHead(const string& aName, CAE_Object& aComp): CagLayout(aName), iComp(aComp)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iComp.InstName(), strlen(iComp.InstName()));
    int w, h; pango_layout_get_pixel_size(plo, &w, &h);
    gtk_widget_set_size_request(iWidget, w, h);
    g_object_unref(plo);
}

CapCompHead::~CapCompHead()
{
}


void CapCompHead::OnExpose(GdkEventExpose* aEvent)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iComp.InstName(), strlen(iComp.InstName()));
    gdk_draw_layout(BinWnd(), Gc(), 0, 0, plo);
    GtkAllocation alc; gtk_widget_get_allocation(iWidget, &alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, 0, 0, alc.width - 1, alc.height - 1);
    g_object_unref(plo);
}

TBool CapCompHead::OnButtonPress(GdkEventButton* aEvent)
{
    if (aEvent->type == GDK_BUTTON_PRESS) {
	SetState(GTK_STATE_SELECTED);
    }
}

TBool CapCompHead::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapCompHead::OnSizeAllocate(GtkAllocation* aAllocation)
{
}

void CapCompHead::OnSizeRequest(GtkRequisition* aRequisition)
{
}

void CapCompHead::OnMotion(GdkEventMotion *aEvent)
{
}

void CapCompHead::OnEnter(GdkEventCrossing *aEvent)
{
}

void CapCompHead::OnLeave(GdkEventCrossing *aEvent)
{
}

void CapCompHead::OnStateChanged(GtkStateType state)
{
    if (State() == GTK_STATE_SELECTED) {
	iParent->OnChildStateChanged(this, state);
    }
}


