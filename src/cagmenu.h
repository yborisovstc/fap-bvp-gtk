#ifndef __FAPBVP_GTK_MENU_H
#define __FAPBVP_GTK_MENU_H

#include "cagcontainer.h"
#include "cagbin.h"

// Gtk menu item observer iface
class CagMenuItem;
class MCagMenuItemObs
{
    public:
	static const char* Type() { return "MCagMenuItemObs";}; 
	virtual void OnActivated(CagMenuItem* aItem) = 0;
};

class CagMenuItem: public CagBin
{
    public:
	CagMenuItem(const string& aName);
	CagMenuItem(const string& aName, const string& aLabel);
	void SetMenuItemObs(CagWidget* aObs);
	void SetSubmenu(CagWidget* aSubmenu);
    private:
	static void handle_activate(GtkMenuItem *menuitem, gpointer user_data);
};


class CagMenuShell;
class MagMenuShellObs
{
    public:
	static const char* Type() { return "MagMenuShellObs";}; 
	virtual void OnItemActivated(CagMenuShell* aMenuShell, CagMenuItem* aItem) = 0;
};

class CagMenuItem;
class CagMenuShell: public CagContainer, public MCagMenuItemObs
{
    public:
	CagMenuShell(const string& aName);
	void Append(CagMenuItem* aChild);
	void SetMenuShellObs(MagMenuShellObs* aObs);
	static inline const char* Type() { return "CagMenuShell";} ; 
	// From CAE_Base
	virtual void *DoGetObj(const char *aName);
	// From MCagMenuItemObs
	virtual void OnActivated(CagMenuItem* aItem);
    protected:
	CagMenuShell(GType aType, const string& aName);
    private:
	MagMenuShellObs* iObs;
};


class CagMenu: public CagMenuShell
{
    public:
	CagMenu(const string& aName);
	void SetTitle(const string& aTitle);
	void Popup(guint button, guint32 activate_time);
};


class CagMenuBar: public CagMenuShell
{
    public:
	CagMenuBar(const string& aName);
};

#endif
