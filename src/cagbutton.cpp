#include "cagbutton.h"

static void handle_clicked_event(GtkButton *button, gpointer user_data);

CagButton::CagButton(const string& aName): CagBin(GTK_TYPE_BUTTON, aName)
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

