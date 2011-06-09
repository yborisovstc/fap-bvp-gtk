#ifndef __FAPBVP_GTK_TEXTVIEW_H
#define __FAPBVP_GTK_TEXTVIEW_H

#include "cagcontainer.h"

class CagTextView: public CagContainer 
{
    public:
	CagTextView(const string& aName);
	void SetBuffer(GtkTextBuffer* aBuffer);
	void SetEditable(TBool aEditable);
	gboolean ScrollToIter(GtkTextIter *iter, gdouble within_margin, gboolean use_align, gdouble xalign, gdouble yalign);
	string GetBuffer();
};



#endif 
