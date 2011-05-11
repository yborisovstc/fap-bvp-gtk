#ifndef __FAPBVP_GTK_TEXTVIEW_H
#define __FAPBVP_GTK_TEXTVIEW_H

#include "cagcontainer.h"

class CagTextView: public CagContainer 
{
    public:
	CagTextView(const string& aName);
	void SetBuffer(GtkTextBuffer* aBuffer);
	void SetEditable(TBool aEditable);
	string GetBuffer();
};



#endif 
