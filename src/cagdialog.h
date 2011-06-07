
#ifndef __FAPBVP_GTK_DIALOG_H
#define __FAPBVP_GTK_DIALOG_H

#include "cagwindow.h"


class CagDialog: public CagWindow
{
    public:
	CagDialog(const string& aName);
	void AddButton(const string& aLabel, TInt aId);
	void AddActionWidget(CagWidget* aWidget, TInt aId);
	void AddContentWidget(CagWidget* aWidget, gboolean expand, gboolean fill, guint padding);
	TInt Run();
};


#endif
