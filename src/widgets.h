#ifndef __FAPBVP_GTK_WIDGETS_H
#define __FAPBVP_GTK_WIDGETS_H

#include <gtk/gtk.h>
#include <string>
#include <fapbase.h>

using namespace std;

// Base wrapper classes

class CagWidget 
{
    public:
	// Embedded GC type (as stored in style)
	enum TGcType {
	    EGt_Fg,
	    EGt_Bg
	};
    public:
	CagWidget(GType aType, const string& aName);
	virtual ~CagWidget();
	const string& Name() {return iName;};
	void Show();
	void SetParent(CagWidget* aParent);
	void ResetParent(CagWidget* aParent);
	void SizeAllocate(GtkAllocation* aAlloc);
	void SizeRequest(GtkRequisition* aReq);
	void Allocation(GtkAllocation *aAlloc);
	void SetState(GtkStateType aState);
	GtkStateType State();
    protected:
	CagWidget(GtkWidget* aWidget, const string& aName, TBool aOwned = ETrue);
	void Construct();
    public:
	virtual void OnExpose(GdkEventExpose* aEvent) {};
	virtual TBool OnButtonPress(GdkEventButton* aEvent) { return ETrue;};
	virtual TBool OnButtonRelease(GdkEventButton* aEvent) { return ETrue;};
	virtual void OnSizeAllocate(GtkAllocation* aAllocation)  {};
	virtual void OnSizeRequest(GtkRequisition* aRequisition) {};
	virtual void OnMotion(GdkEventMotion *aEvent) {};
	virtual void OnEnter(GdkEventCrossing *aEvent) {};
	virtual void OnLeave(GdkEventCrossing *aEvent) {};
	virtual void OnStateChanged(GtkStateType state) {};
	virtual void OnChildStateChanged(CagWidget* aChild, GtkStateType aPrevState) {};
    protected:
	GdkWindow* GdkWnd();
	GtkStyle* Style();
	GdkGC* Gc(TGcType aType = EGt_Fg);
	PangoContext* PangoCtext();
    private:
	static gboolean handle_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);
	static gboolean handle_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data);
	static gboolean handle_button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data);
	static void     handle_size_allocate_event( GtkWidget *widget, GtkAllocation *allocation, gpointer data);
	static void     handle_size_request_event( GtkWidget *widget, GtkRequisition *requisition, gpointer data);
	static void     handle_motion_notify_event( GtkWidget *widget, GdkEventMotion *event, gpointer data);
	static gboolean handle_enter_notify_event( GtkWidget *widget, GdkEventCrossing *event, gpointer data);
	static gboolean handle_leave_notify_event( GtkWidget *widget, GdkEventCrossing *event, gpointer data);
	static void     handle_state_changed_event( GtkWidget *widget, GtkStateType state, gpointer data);
    public:
	GtkWidget* iWidget;
	CagWidget* iParent;
    protected:
	TBool iOwned;
	string iName;
};

#endif 
