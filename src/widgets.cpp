#include "widgets.h"
#include <map>

CagWidget::CagWidget(GType aType, const string& aName): 
    iName(aName), iWidget(gtk_widget_new(aType, NULL)), iParent(NULL), iOwned(ETrue), iLbtnHoldsPressed(FALSE)
{
    if (iOwned)
	Construct();
}

CagWidget::CagWidget(GtkWidget* aWidget, const string& aName, TBool aOwned): 
    iWidget(aWidget), iOwned(aOwned), iName(aName), iParent(NULL), iLbtnHoldsPressed(FALSE)
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
    g_signal_connect(G_OBJECT(iWidget), "drag_drop", G_CALLBACK (handle_drag_drop), this);
    g_signal_connect(G_OBJECT(iWidget), "drag_begin", G_CALLBACK (handle_drag_begin), this);
    g_signal_connect(G_OBJECT(iWidget), "drag_data_received", G_CALLBACK (handle_drag_data_received), this);
    g_signal_connect(G_OBJECT(iWidget), "drag_motion", G_CALLBACK (handle_drag_motion), this);
    g_signal_connect(G_OBJECT(iWidget), "drag_data_get", G_CALLBACK (handle_drag_data_get), this);
    g_signal_connect(G_OBJECT(iWidget), "key-press-event", G_CALLBACK (handle_key_press_event), this);
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

void CagWidget::SetVisible(TBool aVisible)
{
    gtk_widget_set_visible(iWidget, aVisible);
}

void CagWidget::SetSensitive(gboolean aSet)
{
    gtk_widget_set_sensitive(iWidget, aSet);
}

gboolean CagWidget::IsSensitive() const
{
    return gtk_widget_is_sensitive(iWidget);
}

void CagWidget::SizeAllocate(GtkAllocation* aAlloc)
{
    gtk_widget_size_allocate(iWidget, aAlloc);
}

void CagWidget::SizeRequest(GtkRequisition* aReq)
{
    gtk_widget_size_request(iWidget, aReq);
}

void CagWidget::SetSizeRequest(gint aWidth, gint aHeight)
{
    gtk_widget_set_size_request(iWidget, aWidth, aHeight);
}

void CagWidget::Allocation(GtkAllocation *aAlloc) const
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

void CagWidget::SetBorder(GtkBorder* aBorder)
{
}

void CagWidget::GetBorder(GtkBorder* aBorder)
{
    GValue prop = {0};
    g_value_init(&prop, GTK_TYPE_BORDER);
    gtk_widget_style_get_property(iWidget, "draw-border", &prop);
    gtk_widget_style_get(iWidget, "draw-border", aBorder, NULL);
}

void CagWidget::GrabFocus()
{
    gtk_widget_grab_focus(iWidget);
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

void CagWidget::DragSourceAdd(GdkModifierType start_button_mask, const GtkTargetEntry *targets, gint n_targets, GdkDragAction actions)
{
    GtkTargetList* list = gtk_drag_source_get_target_list(iWidget);
    if (list == NULL) {
	gtk_drag_source_set(iWidget, start_button_mask, targets, n_targets, actions);
    }
    else {
	gtk_target_list_add_table(list, targets, n_targets);
	gtk_drag_source_set_target_list(iWidget, list);
    }
}

void CagWidget::DragDestAdd(GtkDestDefaults flags, const GtkTargetEntry *targets, gint n_targets, GdkDragAction actions)
{
    GtkTargetList* list = gtk_drag_dest_get_target_list(iWidget);
    if (list == NULL) {
	gtk_drag_dest_set(iWidget, flags, targets, n_targets, actions);
    }
    else {
	gtk_target_list_add_table(list, targets, n_targets);
	gtk_drag_dest_set_target_list(iWidget, list);
    }
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
    if (event->button == 1) {
	self->iLbtnHoldsPressed = TRUE;
    }
    return self->OnButtonPress(event);
}

gboolean CagWidget::handle_button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    CagWidget* self = (CagWidget*) data;
    if (event->button == 1 && self->iLbtnHoldsPressed) {
	self->iLbtnHoldsPressed = FALSE;
    }
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
    if (self->iLbtnHoldsPressed) {
	GtkTargetList* tarlist = gtk_drag_source_get_target_list(self->iWidget);
	GdkDragContext* ctx = gtk_drag_begin(self->iWidget, tarlist, GDK_ACTION_COPY, 1, (GdkEvent*) event);
    }
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


void CagWidget::SetWidgetObs(CagWidget* aObs)
{
    MWidgetObs* obs = aObs->GetObj(obs);
    _FAP_ASSERT(obs != NULL);
    g_signal_connect(G_OBJECT(iWidget), "button_press_event", G_CALLBACK (handle_widget_button_press_event), aObs);
    g_signal_connect(G_OBJECT(iWidget), "focus_out_event", G_CALLBACK (handle_widget_focus_out_event), aObs);
}


gboolean CagWidget::handle_drag_drop(GtkWidget *widget, GdkDragContext *drag_context, gint x, gint y, guint time, gpointer user_data)
{
    CagWidget* self = (CagWidget*) user_data;
    return self->OnDragDrop(drag_context, x, y, time);  
}

void CagWidget::handle_drag_begin(GtkWidget *widget, GdkDragContext *drag_context, gpointer user_data)
{
}

void CagWidget::handle_drag_data_received(GtkWidget *widget, GdkDragContext *drag_context, gint x, gint y, GtkSelectionData *data,
	      guint info, guint time, gpointer user_data)
{
    CagWidget* self = (CagWidget*) user_data;
    self->OnDragDataReceived(drag_context, x, y, data, info, time);
}

gboolean CagWidget::handle_drag_motion(GtkWidget *widget, GdkDragContext *drag_context, gint x, gint y, guint time, gpointer user_data)
{
    return false;
}

void  CagWidget::handle_drag_data_get(GtkWidget *widget, GdkDragContext *drag_context, GtkSelectionData *data, guint info, 
		guint time, gpointer user_data)
{
    CagWidget* self = (CagWidget*) user_data;
    self->OnDragDataGet(drag_context, data, info, time);
}

gboolean CagWidget::handle_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    CagWidget* self = (CagWidget*) user_data;
    return self->OnKeyPressEvent(event);
}





gboolean CagWidget::handle_widget_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    CagWidget* hler = (CagWidget*) data;
    MWidgetObs* obs = hler->GetObj(obs);
    _FAP_ASSERT(obs != NULL);
    CagWidget* wid = hler->GetWidget(GTK_WIDGET(widget));
    _FAP_ASSERT(wid != NULL);
    return obs->OnWidgetButtonPress(wid, event);
}

gboolean CagWidget::handle_widget_focus_out_event(GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
    CagWidget* hler = (CagWidget*) data;
    MWidgetObs* obs = hler->GetObj(obs);
    _FAP_ASSERT(obs != NULL);
    CagWidget* wid = hler->GetWidget(GTK_WIDGET(widget));
    _FAP_ASSERT(wid != NULL);
    return obs->OnWidgetFocusOut(wid, event);
}

void CagWidget::OnDragDataGet(GdkDragContext *drag_context, GtkSelectionData *data, guint info, guint time)
{
    if (iSelText.empty()) {
	gtk_selection_data_set_text(data, iName.c_str(), -1);
    }
    else {
	gtk_selection_data_set_text(data, iSelText.c_str(), -1);
    }
}

