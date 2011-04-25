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
	CagButton(GType aType, const string& aName);
};


class CagToggleButton;
// Gtk toggle button observer iface
class MCagToggleButtonObs
{
    public:
	static inline const char* Type(); 
	virtual void OnToggled(CagToggleButton* aBtn) = 0;
};

inline const char* MCagToggleButtonObs::Type() { return "CagToggleBtnObs";} 

// Toggle button
class CagToggleButton: public CagButton 
{
    public:
	static inline const char* Type() { return "CagToggleBtn";}; 
	CagToggleButton(const string& aName);
	void SetActive(TBool aActive);
	TBool IsActive() const;
	void SetObs(CagWidget* aObs);
    protected:
	// From CAE_Base
	virtual void *DoGetObj(const char *aName);
    private:
	static void handle_toggled_event(GtkToggleButton *togglebutton, gpointer user_data);
};



#endif 
