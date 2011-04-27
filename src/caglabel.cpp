#include "caglabel.h"

static void handle_clicked_event(GtkButton *button, gpointer user_data);

CagLabel::CagLabel(const string& aName): CagMisc(GTK_TYPE_LABEL, aName)
{
}

CagLabel::~CagLabel()
{
}

void CagLabel::SetText(const string& aText)
{
    gtk_label_set_text(GTK_LABEL(iWidget), aText.c_str());
}


CagELabel::CagELabel(const string& aName): CagEventBox(aName)
{
    gtk_container_add(GTK_CONTAINER(iWidget), gtk_label_new(NULL));
    gtk_widget_show(GTK_WIDGET(Label()));
}


GtkLabel* CagELabel::Label()
{
    GtkWidget* wid = gtk_bin_get_child(GTK_BIN(iWidget));
    return GTK_LABEL(wid);
}

void CagELabel::SetText(const string& aText)
{
    gtk_label_set_text(GTK_LABEL(Label()), aText.c_str());
}
