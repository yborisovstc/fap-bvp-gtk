#ifndef __FAPBVP_GTK_BUTTON_H
#define __FAPBVP_GTK_BUTTON_H

#include "cagbin.h"

class CagButton;
// Gtk button observer iface
class MCagButtonObs
{
    public:
	static inline const char* Type(); 
	virtual void OnClicked(CagButton* aBtn) = 0;
};

inline const char* MCagButtonObs::Type() { return "CagBtnObs";} 

class CagButton: public CagBin 
{
    public:
	CagButton(const string& aName);
	virtual ~CagButton();
	void SetLabel(const string& aLabel);
	void SetImage(const string& aPath);
	void SetObs(CagWidget* aObs);
    protected:
};

#endif 
