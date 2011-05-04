#include "cagimage.h"

static void handle_clicked_event(GtkButton *button, gpointer user_data);

CagImage::CagImage(const string& aName): CagMisc(GTK_TYPE_IMAGE, aName)
{
}

void CagImage::SetFromFile(const string& aFileName)
{
    gtk_image_set_from_file(GTK_IMAGE(iWidget), aFileName.c_str());
}


CagEImage::CagEImage(const string& aName): CagEventBox(aName)
{
    gtk_container_add(GTK_CONTAINER(iWidget), gtk_image_new());
    gtk_widget_show(GTK_WIDGET(Image()));
}


GtkImage* CagEImage::Image()
{
    GtkWidget* wid = gtk_bin_get_child(GTK_BIN(iWidget));
    return GTK_IMAGE(wid);
}

void CagEImage::SetFromFile(const string& aFileName)
{
    gtk_image_set_from_file(Image(), aFileName.c_str());
}

