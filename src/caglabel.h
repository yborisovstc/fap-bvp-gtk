#ifndef __FAPBVP_GTK_LABEL_H
#define __FAPBVP_GTK_LABEL_H

#include "cagmisc.h"

class CagLabel: public CagMisc 
{
    public:
	CagLabel(const string& aName);
	virtual ~CagLabel();
	void SetText(const string& aText);
};

#endif 
