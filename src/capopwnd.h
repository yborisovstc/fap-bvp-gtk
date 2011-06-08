#ifndef __FAPBVP_GTK_OPWND_H
#define __FAPBVP_GTK_OPWND_H

#include "cagwindow.h"
#include "cagtoolbar.h"
#include "cagbox.h"
#include "capsys.h"

class CapOtbDragItem: public CagToolItem
{
    public:
	CapOtbDragItem(const string& aName, GtkTargetEntry* aTes, TInt aTesLen, const string& aSel);
    protected:
	virtual TBool OnButtonPress(GdkEventButton* aEvent);
	virtual void OnDragDataGet(GdkDragContext *drag_context, GtkSelectionData *data, guint info, guint time); 
    private:
	GtkTargetEntry* iTes;
	TInt iTesLen;
	string iSel;
};

class MOpWndObserver
{
    public:
	enum TCmd
	{
	    ECmd_Up,
	    ECmd_Back
	};
    public:
	virtual void OnTurnToComp(CAE_Object* aComp) = 0;
	virtual void OnTurnToSyst(const string& aName) = 0;
	virtual void OnCmd(TCmd aCmd) = 0;
	virtual TBool OnCmdUpdateRequest(TCmd aCmd) = 0;
};

class CapOpWnd: public CagLayout, public MCapSysObserver, public MagToolButtonObserver
{
    public:
	static inline const char* Type() { return "CapOpWnd";} ; 
	CapOpWnd(const string& aName, CagToolBar* aToolbar);
	virtual ~CapOpWnd();
	void AddView(CagWidget* aView);
	void RemoveView(CagWidget* aView);
	void SetSys(CAE_Object::Ctrl* aObj);
	void UnsetSys(CAE_Object::Ctrl* aObj);
	void SetObserver(MOpWndObserver* aObs);
	void UpdateCmds();
	// From CAE_Base
	virtual void *DoGetObj(const char *aName);
    protected:
	void Construct();
	// From MCapSysObserver	
	virtual void OnCompSelected(CAE_Object* aComp);
	virtual void OnSystSelected(const string& aName);
	// From MagToolButtonObserver
	virtual void OnClicked(CagToolButton* aBtn);
    private:
	virtual void OnSizeAllocate(GtkAllocation* aAllocation);
	virtual void OnSizeRequest(GtkRequisition* aRequisition);
    private:
	CagVBox* iVbox;
	CagToolBar* iToolbar;
	CapSys* iSysWidget;
	MOpWndObserver* iObs;
	CagLabelToolItem* iCursName; // Not owned
};

#endif 
