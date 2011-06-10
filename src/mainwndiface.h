
#ifndef __FAPBVP_GTK_MWIFACE_H
#define __FAPBVP_GTK_MWIFACE_H

class MOpMainWnd
{
    public:
	static inline const char* Type() { return "MOpMainWnd";} ; 
	virtual CagScrolledWindow* ClientWnd() = 0;
	virtual CagToolBar* Toolbar() = 0;
};

#endif
