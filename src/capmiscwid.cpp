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
		SelectRegion(0, GetTextLen());
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
	    iObserver->OnUpdateCompleted(this);
	}
    }
    return EFalse;
}

CapPopupMenu::CapPopupMenu(const string& aName, const vector<TPmenuSpecElem>& aSpec): 
    CagMenu(aName), iContext(NULL)
{
    for (vector<TPmenuSpecElem>::const_iterator it = aSpec.begin(); it != aSpec.end(); it++) {
	const TPmenuSpecElem& spec = *it;
	CagMenuItem* itm_del = new CagMenuItem(spec.iName, spec.iLabel);
	itm_del->Show();
	Append(itm_del);
    }
}

void* CapPopupMenu::DoGetObj(const char *aName)
{
    if (strcmp(aName, Type()) == 0) 
	return this;
    else return CagMenu::DoGetObj(aName);
}


