#include "cagmenu.h"

CagMenuShell::CagMenuShell(const string& aName): CagContainer(GTK_TYPE_MENU_SHELL, aName)
{
}

CagMenuShell::CagMenuShell(GType aType, const string& aName): CagContainer(aType, aName)
{
}

void CagMenuShell::Append(CagWidget* aChild)
{
    gtk_menu_shell_append(GTK_MENU_SHELL(iWidget), aChild->iWidget);
    Add(aChild, EFalse);
}


CagMenuBar::CagMenuBar(const string& aName): CagMenuShell(GTK_TYPE_MENU_BAR, aName)
{
}

