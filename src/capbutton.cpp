
#include "capbutton.h"
#include "capcommon.h"

CapButton::CapButton(const string& aName, const string& aText): CagLayout(aName), iText(aText)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iText.c_str(), iText.size());
    int w, h; pango_layout_get_pixel_size(plo, &w, &h);
    gtk_widget_set_size_request(iWidget, w, h);
    g_object_unref(plo);
}

CapButton::CapButton(const string& aName): CagLayout(aName)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iText.c_str(), iText.size());
    int w, h; pango_layout_get_pixel_size(plo, &w, &h);
    gtk_widget_set_size_request(iWidget, w, h);
    g_object_unref(plo);
}

CapButton::~CapButton()
{
}

void CapButton::SetText(const string& aText)
{
    iText = aText;
}

void CapButton::OnExpose(GdkEventExpose* aEvent)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iText.c_str(), iText.size());
    gdk_draw_layout(BinWnd(), Gc(), 0, 0, plo);
    GtkAllocation alc; gtk_widget_get_allocation(iWidget, &alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, 0, 0, alc.width - 1, alc.height - 1);
    g_object_unref(plo);
}

TBool CapButton::OnButtonPress(GdkEventButton* aEvent)
{
    if (aEvent->type == GDK_BUTTON_PRESS) {
	SetState(GTK_STATE_SELECTED);
    }
}

TBool CapButton::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapButton::OnSizeAllocate(GtkAllocation* aAllocation)
{
}

void CapButton::OnSizeRequest(GtkRequisition* aRequisition)
{
}

void CapButton::OnMotion(GdkEventMotion *aEvent)
{
}

void CapButton::OnEnter(GdkEventCrossing *aEvent)
{
}

void CapButton::OnLeave(GdkEventCrossing *aEvent)
{
}

void CapButton::OnStateChanged(GtkStateType state)
{
}


