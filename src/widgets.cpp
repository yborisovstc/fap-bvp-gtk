#include "widgets.h"
#include <map>

CagWidget::CagWidget(GType aType, const string& aName): 
    iName(aName), iWidget(gtk_widget_new(aType, NULL)), iParent(NULL)
{
    if (iOwned)
	Construct();
}

CagWidget::CagWidget(GtkWidget* aWidget, const string& aName, TBool aOwned): 
    iWidget(aWidget), iOwned(aOwned), iName(aName), iParent(NULL)
{
    if (iOwned)
	Construct();
}

void* CagWidget::DoGetObj(const char *aName)
{
    return (strcmp(aName, Type()) == 0) ? this : NULL;
}

void CagWidget::Construct()
{
    gtk_widget_set_name(iWidget, iName.c_str());
    gtk_widget_set_events (iWidget, GDK_ALL_EVENTS_MASK);

    g_signal_connect(G_OBJECT(iWidget), "expose_event", G_CALLBACK(handle_expose_event), this);
    g_signal_connect(G_OBJECT(iWidget), "button_press_event", G_CALLBACK (handle_button_press_event), this);
    g_signal_connect(G_OBJECT(iWidget), "button_release_event", G_CALLBACK (handle_button_release_event), this);
    g_signal_connect(G_OBJECT(iWidget), "size_allocate", G_CALLBACK (handle_size_allocate_event), this);
    g_signal_connect(G_OBJECT(iWidget), "size_request", G_CALLBACK (handle_size_request_event), this);
    g_signal_connect(G_OBJECT(iWidget), "motion_notify_event", G_CALLBACK (handle_motion_notify_event), this);
    g_signal_connect(G_OBJECT(iWidget), "enter_notify_event", G_CALLBACK (handle_enter_notify_event), this);
    g_signal_connect(G_OBJECT(iWidget), "leave_notify_event", G_CALLBACK (handle_leave_notify_event), this);
    g_signal_connect(G_OBJECT(iWidget), "state_changed", G_CALLBACK (handle_state_changed_event), this);
}

CagWidget::~CagWidget()
{
    // It is possible that the widget is already unreferenced
    if (GTK_IS_WIDGET(iWidget)) {
	gtk_widget_destroy(iWidget);
    }
}

CagWidget* CagWidget::GetWidget(GtkWidget* aGtkWidget, CagWidget* aRequester)
{
    CagWidget* res = NULL;
    if (iWidget == aGtkWidget) {
	res = this;
    }
    else if (iParent != NULL && iParent != aRequester) {
	res = iParent->GetWidget(aGtkWidget);	
    }
    return res;
}

void CagWidget::SetParent(CagWidget* aParent)
{
    _FAP_ASSERT(iParent == NULL);
    iParent = aParent;
}

void CagWidget::ResetParent(CagWidget* aParent)
{
    _FAP_ASSERT(iParent == aParent);
    iParent = NULL;
}

void CagWidget::Show()
{
    gtk_widget_show(iWidget);
}

void CagWidget::Hide()
{
    gtk_widget_hide(iWidget);
}

TBool CagWidget::IsVisible()
{
    return  gtk_widget_get_visible(iWidget);
}

void CagWidget::SizeAllocate(GtkAllocation* aAlloc)
{
    gtk_widget_size_allocate(iWidget, aAlloc);
}

void CagWidget::SizeRequest(GtkRequisition* aReq)
{
    gtk_widget_size_request(iWidget, aReq);
}

void CagWidget::Allocation(GtkAllocation *aAlloc)
{
    gtk_widget_get_allocation(iWidget, aAlloc);
}

void CagWidget::SetState(GtkStateType aState)
{
    gtk_widget_set_state(iWidget, aState);
}

GtkStateType CagWidget::State()
{
    return gtk_widget_get_state(iWidget);
}

GdkWindow* CagWidget::GdkWnd()
{
    return gtk_widget_get_window(iWidget);
}

GtkStyle* CagWidget::Style()
{
    return gtk_widget_get_style(iWidget);
}

GdkGC* CagWidget::Gc(TGcType aType)
{
    GdkGC* res = NULL;
    GtkStyle* style = gtk_widget_get_style(iWidget);
    GtkStateType state = gtk_widget_get_state(iWidget);
    if (aType == EGt_Fg) {
	res = style->fg_gc[state];
    }
    else if (aType == EGt_Bg) {
	res = style->bg_gc[state];
    }
    return res;
}

PangoContext* CagWidget::PangoCtext()
{
    return gtk_widget_get_pango_context(iWidget);
}



gboolean CagWidget::handle_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    CagWidget* self = (CagWidget*) data;
    self->OnExpose(event);
    return FALSE;
}

gboolean CagWidget::handle_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    CagWidget* self = (CagWidget*) data;
    return self->OnButtonPress(event);
}

gboolean CagWidget::handle_button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    CagWidget* self = (CagWidget*) data;
    return self->OnButtonRelease(event);
}

void CagWidget::handle_size_allocate_event( GtkWidget *widget, GtkAllocation *allocation, gpointer data)
{
    CagWidget* self = (CagWidget*) data;
    self->OnSizeAllocate(allocation);
}

void CagWidget::handle_size_request_event( GtkWidget *widget, GtkRequisition *requisition, gpointer data)
{
    CagWidget* self = (CagWidget*) data;
    self->OnSizeRequest(requisition);
}

void CagWidget::handle_motion_notify_event( GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
    CagWidget* self = (CagWidget*) data;
    self->OnMotion(event);
}

gboolean CagWidget::handle_enter_notify_event( GtkWidget *widget, GdkEventCrossing *event, gpointer data)
{
    CagWidget* self = (CagWidget*) data;
    GtkWidget* wd = GTK_WIDGET(self->iWidget);
    if (widget == wd) {
	self->OnEnter(event);
    }
    return FALSE;
}

gboolean CagWidget::handle_leave_notify_event( GtkWidget *widget, GdkEventCrossing *event, gpointer data)
{
    CagWidget* self = (CagWidget*) data;
    if (widget == GTK_WIDGET(self->iWidget)) {
	self->OnLeave(event);
    }
    return FALSE;
}

void     CagWidget::handle_state_changed_event( GtkWidget *widget, GtkStateType state, gpointer data)
{
    CagWidget* self = (CagWidget*) data;
    self->OnStateChanged(state);
}



