
#include "cagdialog.h"


CagDialog::CagDialog(const string& aName): CagWindow(GTK_TYPE_DIALOG, aName)
{
}

void CagDialog::AddButton(const string& aLabel, TInt aId)
{
    gtk_dialog_add_button(GTK_DIALOG(iWidget), aLabel.c_str(), aId);
}

TInt CagDialog::Run()
{
    return gtk_dialog_run(GTK_DIALOG(iWidget));
}

void CagDialog::AddActionWidget(CagWidget* aWidget, TInt aId)
{
    // Widget is not added to container because it is owned by dialog internal widget
     gtk_dialog_add_action_widget(GTK_DIALOG(iWidget), aWidget->iWidget, aId);
}

void CagDialog::AddContentWidget(CagWidget* aWidget, gboolean expand, gboolean fill, guint padding)
{
   GtkVBox* contarea = GTK_VBOX (gtk_dialog_get_content_area(GTK_DIALOG(iWidget)));
    gtk_box_pack_start(GTK_BOX(contarea), aWidget->iWidget, expand, fill, padding);
   
}

