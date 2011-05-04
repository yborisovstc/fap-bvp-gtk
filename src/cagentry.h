#ifndef __FAPBVP_GTK_ENTRY_H
#define __FAPBVP_GTK_ENTRY_H

#include "widgets.h"

class CagEntry: public CagWidget 
{
    public:
	CagEntry(const string& aName);
	void SetText(const string& aText);
	TInt GetTextLen();
	string GetText();
	void SetHasFrame(TBool aHas);
	void SetInnerBorder(const GtkBorder *border);
	void SetWidthChars(TInt aNchars);
	void SetEditable(TBool aEditable);
	void SetPosition(TInt aPos);
};

#endif 
