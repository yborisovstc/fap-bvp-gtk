#include "cagbutton.h"

static void handle_clicked_event(GtkButton *button, gpointer user_data);

CagButton::CagButton(const string& aName): CagBin(GTK_TYPE_BUTTON, aName)
{
}

CagButton::CagButton(GType aType, const string& aName): CagBin(aType, aName)
{
}

CagButton::~CagButton()
{
}

void CagButton::SetLabel(const string& aLabel)
{
    gtk_button_set_label(GTK_BUTTON(iWidget), aLabel.c_str());
}

void CagButton::SetImage(const string& aPath)
{
    GtkWidget* img =  gtk_image_new_from_file(aPath.c_str());
    gtk_button_set_image(GTK_BUTTON(iWidget), img);
}

void CagButton::SetObs(CagWidget* aObs)
{
    g_signal_connect(G_OBJECT(iWidget), "clicked", G_CALLBACK(handle_clicked_event), aObs);
}

void handle_clicked_event(GtkButton *button, gpointer user_data)
{

    CagWidget* hler = (CagWidget*) user_data;
    MCagButtonObs* obs = hler->GetObj(obs);
    _FAP_ASSERT(obs != NULL);
    CagWidget* wid = hler->GetWidget(GTK_WIDGET(button));
    _FAP_ASSERT(wid != NULL);
    CagButton* btn = wid->GetObj(btn);
    _FAP_ASSERT(btn != NULL);
    obs->OnClicked(btn);
}



CagToggleButton::CagToggleButton(const string& aName): CagButton(GTK_TYPE_TOGGLE_BUTTON, aName)
{
}

CagToggleButton::CagToggleButton(GType aType, const string& aName): CagButton(aType, aName)
{
}

void CagToggleButton::SetActive(TBool aActive)
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(iWidget), aActive);
}

TBool CagToggleButton::IsActive() const
{
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(iWidget));
}

void* CagToggleButton::DoGetObj(const char *aName)
{
    return (strcmp(aName, Type()) == 0) ? this : CagButton::DoGetObj(aName);
}

void CagToggleButton::SetObs(CagWidget* aObs)
{
    MCagToggleButtonObs* obs = aObs->GetObj(obs);
    _FAP_ASSERT(obs != NULL);
    g_signal_connect(G_OBJECT(iWidget), "toggled", G_CALLBACK(handle_toggled_event), aObs);
}

void CagToggleButton::handle_toggled_event(GtkToggleButton *button, gpointer user_data)
{
    /* Debug
    GtkWidget* child = gtk_bin_get_child(GTK_BIN(button));
    GType type = G_OBJECT_TYPE(child);
    const gchar* name = G_OBJECT_TYPE_NAME(child);
    GtkLabel* lbl = GTK_LABEL(child);
    guint bw = gtk_container_get_border_width(GTK_CONTAINER(button));
    GtkBorder* ibrd;
    gtk_widget_style_get(GTK_WIDGET(button), "default_outside_border", &ibrd, NULL);
    GdkColor* clr;
    gtk_widget_style_get(GTK_WIDGET(button), "cursor_color", &clr, NULL);
    gboolean dipf;
    gtk_widget_style_get(GTK_WIDGET(button), "displace_focus", &dipf, NULL);
    PangoLayout* lout = gtk_label_get_layout(lbl);
    int baseline = pango_layout_get_baseline(lout);
    int sp = pango_layout_get_spacing(lout);
    pango_layout_set_indent(lout, 20);
    gtk_container_set_border_width(GTK_CONTAINER(button), bw);
*/


    CagWidget* hler = (CagWidget*) user_data;
    MCagToggleButtonObs* obs = hler->GetObj(obs);
    _FAP_ASSERT(obs != NULL);
    CagWidget* wid = hler->GetWidget(GTK_WIDGET(button));
    _FAP_ASSERT(wid != NULL);
    CagToggleButton* btn = wid->GetObj(btn);
    _FAP_ASSERT(btn != NULL);
    obs->OnToggled(btn);
}




CagCheckButton::CagCheckButton(const string& aName): CagToggleButton(GTK_TYPE_CHECK_BUTTON, aName)
{
}
