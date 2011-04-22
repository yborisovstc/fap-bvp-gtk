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

