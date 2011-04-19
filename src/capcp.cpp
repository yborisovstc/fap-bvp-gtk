
#include "capcp.h"
#include "capcommon.h"

CapCp::CapCp(const string& aName, CAE_ConnPointBase& aCp): CagLayout(aName), iCp(aCp)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iCp.Name().c_str(), iCp.Name().size());
    int w, h; pango_layout_get_pixel_size(plo, &w, &h);
    gtk_widget_set_size_request(iWidget, w, h);
    g_object_unref(plo);
}

CapCp::~CapCp()
{
}


void CapCp::OnExpose(GdkEventExpose* aEvent)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iCp.Name().c_str(), iCp.Name().size());
    gdk_draw_layout(BinWnd(), Gc(), 0, 0, plo);
    GtkAllocation alc; gtk_widget_get_allocation(iWidget, &alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, 0, 0, alc.width - 1, alc.height - 1);
    g_object_unref(plo);
}

TBool CapCp::OnButtonPress(GdkEventButton* aEvent)
{
}

TBool CapCp::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapCp::OnSizeAllocate(GtkAllocation* aAllocation)
{
}

void CapCp::OnSizeRequest(GtkRequisition* aRequisition)
{
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


