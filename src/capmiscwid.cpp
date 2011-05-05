#include "capmiscwid.h"
#include <gdk/gdkkeysyms.h>


CapEopEntry::CapEopEntry(const string& aName): CagEntry(aName), iObserver(NULL)
{
    SetEditable(EFalse);
}

TBool CapEopEntry::OnButtonPress(GdkEventButton* aEvent)
{
    if (aEvent->button == 1) {
	if (aEvent->type == GDK_BUTTON_PRESS) {
	    if (State() == GTK_STATE_ACTIVE) {
		SetState(GTK_STATE_NORMAL);
		SetEditable(ETrue);
		GrabFocus();
	    }
	    else {
		SetState(GTK_STATE_ACTIVE);
	    }
	}
    }
    return EFalse;
}

void CapEopEntry::SetObserver(MapEopEntryObserver* aObserver)
{
    _FAP_ASSERT(iObserver == NULL);
    iObserver = aObserver;
}

gboolean CapEopEntry::OnKeyPressEvent(GdkEventKey *event)
{
    if (event->keyval == GDK_Return) {
	SetEditable(EFalse);
	if (iObserver != NULL) {
	    iObserver->OnUpdateCompleted();
	}
    }
    return EFalse;
}

