#ifndef __FAPBVP_GTK_STYLE_H
#define __FAPBVP_GTK_STYLE_H


#include <gtk/gtk.h>

class CagStyle
{
    public:
	CagStyle(GtkStyle* aStyle);
	virtual ~CagStyle();
    private:
	GtkStyle* iStyle;
};

#endif 
