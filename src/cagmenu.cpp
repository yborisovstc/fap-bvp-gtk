#include "cagmenu.h"

CagMenuShell::CagMenuShell(const string& aName): CagContainer(GTK_TYPE_MENU_SHELL, aName), iObs(NULL)
{
}

CagMenuShell::CagMenuShell(GType aType, const string& aName): CagContainer(aType, aName), iObs(NULL)
{
}

void CagMenuShell::SetMenuShellObs(MagMenuShellObs* aObs)
{
    _FAP_ASSERT(iObs == NULL);
    iObs = aObs;
}

void CagMenuShell::Append(CagMenuItem* aChild)
{
    gtk_menu_shell_append(GTK_MENU_SHELL(iWidget), aChild->iWidget);
    Add(aChild, EFalse);
    aChild->SetMenuItemObs(this);
}

void* CagMenuShell::DoGetObj(const char *aName)
{
    if (strcmp(aName, Type()) == 0) 
	return this;
    else if (strcmp(aName, MCagMenuItemObs::Type()) == 0) 
	return (MCagMenuItemObs*) this;
    else return CagContainer::DoGetObj(aName);
}

void CagMenuShell::OnActivated(CagMenuItem* aItem)
{
    if (iObs != NULL) {
	iObs->OnItemActivated(this, aItem);
    }
}




CagMenu::CagMenu(const string& aName): CagMenuShell(GTK_TYPE_MENU, aName)
{
}

void CagMenu::Popup(guint button, guint32 activate_time)
{
    gtk_menu_popup(GTK_MENU(iWidget), NULL, NULL, NULL, NULL, button, activate_time);
}

void CagMenu::SetTitle(const string& aTitle)
{
    gtk_menu_set_title(GTK_MENU(iWidget), aTitle.c_str());
}


CagMenuBar::CagMenuBar(const string& aName): CagMenuShell(GTK_TYPE_MENU_BAR, aName)
{
}


CagMenuItem::CagMenuItem(const string& aName): CagBin(GTK_TYPE_MENU_ITEM, aName)
{
}

CagMenuItem::CagMenuItem(const string& aName, const string& aLabel): CagBin(GTK_TYPE_MENU_ITEM, aName)
{
    gtk_menu_item_set_label(GTK_MENU_ITEM(iWidget), aLabel.c_str());
}

void CagMenuItem::SetMenuItemObs(CagWidget* aObs)
{
    g_signal_connect(G_OBJECT(iWidget), "activate", G_CALLBACK(handle_activate), aObs);
}

void CagMenuItem::handle_activate(GtkMenuItem *item, gpointer user_data)
{
    CagWidget* hler = (CagWidget*) user_data;
    MCagMenuItemObs* obs = hler->GetObj(obs);
    _FAP_ASSERT(obs != NULL);
    CagWidget* wid = hler->GetWidget(GTK_WIDGET(item));
    _FAP_ASSERT(wid != NULL);
    CagMenuItem* mitem = wid->GetObj(mitem);
    _FAP_ASSERT(mitem != NULL);
    obs->OnActivated(mitem);
}

void CagMenuItem::SetSubmenu(CagWidget* aSubmenu)
{
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(iWidget), aSubmenu->iWidget);
    Add(aSubmenu); 
}

