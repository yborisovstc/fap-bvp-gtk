#ifndef __FAPBVP_GTK_IMAGE_H
#define __FAPBVP_GTK_IMAGE_H

#include "cagmisc.h"
#include "cagbin.h"

class CagImage: public CagMisc 
{
    public:
	CagImage(const string& aName);
	void SetFromFile(const string& aFileName);
};

// Image with events support
class CagEImage: public CagEventBox 
{
    public:
	CagEImage(const string& aName);
	void SetFromFile(const string& aFileName);
    private:
	GtkImage* Image();
};



#endif 
