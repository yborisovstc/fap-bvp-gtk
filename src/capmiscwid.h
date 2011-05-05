#ifndef __FAPBVP_GTK_MISCWID_H
#define __FAPBVP_GTK_MISCWID_H

#include "cagentry.h"

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

#endif 
