#ifndef __FAPBVP_GTK_MISCWID_H
#define __FAPBVP_GTK_MISCWID_H

#include "cagentry.h"
#include "cagmenu.h"
#include "capcommon.h"

class MapEopEntryObserver
{
    public:
	virtual void OnUpdateCompleted() = 0;
};

class CapEopEntry: public CagEntry 
{
    public:
	CapEopEntry(const string& aName);
	void SetObserver(MapEopEntryObserver* aObserver);
    protected:
	virtual TBool OnButtonPress(GdkEventButton* aEvent);
	virtual gboolean OnKeyPressEvent(GdkEventKey *event); 
    private:
	MapEopEntryObserver* iObserver;
};

class CapPopupMenu: public CagMenu
{
    public:
	static inline const char* Type() { return "CapPopupMenu";} ; 
	CapPopupMenu(const string& aName, const vector<TPmenuSpecElem>& aSpec);
	CagWidget* Context() { return iContext;};
	void SetContext(CagWidget* aContext) { iContext = aContext;};
	// From CAE_Base
	virtual void *DoGetObj(const char *aName);
    private:
	CagWidget* iContext;
};

#endif 
