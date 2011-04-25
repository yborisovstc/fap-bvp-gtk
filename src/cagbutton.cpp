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

void CagToggleButton::handle_toggled_event(GtkToggleButton *togglebutton, gpointer user_data)
{
    CagWidget* hler = (CagWidget*) user_data;
    MCagToggleButtonObs* obs = hler->GetObj(obs);
    _FAP_ASSERT(obs != NULL);
    CagWidget* wid = hler->GetWidget(GTK_WIDGET(togglebutton));
    _FAP_ASSERT(wid != NULL);
    CagToggleButton* btn = wid->GetObj(btn);
    _FAP_ASSERT(btn != NULL);
    obs->OnToggled(btn);
}

