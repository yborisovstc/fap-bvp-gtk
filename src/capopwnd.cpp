
#include "capopwnd.h"
#include "capcommon.h"

const char* KIconsDir = "/usr/share/fap-bvp-gtk/icons/";
const char* KIcon_NewSyst = "/usr/share/fap-bvp-gtk/icons/tbar_btn_syst.png";
const char* KIcon_NewTrans = "/usr/share/fap-bvp-gtk/icons/tbar_btn_trans.png";

CapOtbDragItem::CapOtbDragItem(const string& aName, GtkTargetEntry* aTes, TInt aTesLen, const string& aSel): 
    CagToolItem(aName), iTes(aTes), iTesLen(aTesLen), iSel(aSel)
{
}

TBool CapOtbDragItem::OnButtonPress(GdkEventButton* aEvent)
{
    GtkTargetList* tarlist = gtk_target_list_new(iTes, iTesLen);
    GdkDragContext* ctx = gtk_drag_begin(iWidget, tarlist, GDK_ACTION_COPY, 1, (GdkEvent*) aEvent);
}

void CapOtbDragItem::OnDragDataGet(GdkDragContext *drag_context, GtkSelectionData *data, guint info, guint time)
{
    if (info == iTes->info) {
	gtk_selection_data_set_text(data, iSel.c_str(), -1);
    }
}



CapOpWndToolbar::CapOpWndToolbar(const string& aName): CagToolBar(aName)
{
    // Button "Back"
    iBtnBack = new CagToolButton("BntBack", GTK_STOCK_GO_BACK);
    Insert(iBtnBack, 0);
    iBtnBack->Show();
    // Button "Up"
    iBtnUp = new CagToolButton("BntUp", GTK_STOCK_GO_UP);
    Insert(iBtnUp, 0);
    iBtnUp->Show();
    // Button "New system"
    iBtnNewSyst = new CapOtbDragItem("BntNewSyst", KTe_NewObject, KTe_NewObject_Len, "_new_object");
    iBtnNewSyst->SetImage(KIcon_NewSyst);
    Insert(iBtnNewSyst, -1);
    iBtnNewSyst->Show();
    // Button "New state"
    iBtnNewState = new CapOtbDragItem("BntNewState", KTe_NewState, KTe_NewState_Len, "_new_state");
    iBtnNewState->SetImage("tbar_btn_state.png");
    Insert(iBtnNewState, -1);
    iBtnNewState->Show();
    // Button "New trans"
    iBtnNewTrans = new CapOtbDragItem("BntNewTrans", KTe_NewTrans, KTe_NewTrans_Len, "_new_trans");
    iBtnNewTrans->SetImage(KIcon_NewTrans);
    Insert(iBtnNewTrans, -1);
    iBtnNewTrans->Show();
}




CapOpWnd::CapOpWnd(const string& aName): CagWindow(aName), iSysWidget(NULL), iObs(NULL)
{
    Construct();
}

CapOpWnd::CapOpWnd(GtkWidget* aWidget, TBool aOwned): CagWindow(aWidget, aOwned), iSysWidget(NULL), iObs(NULL)
{
    Construct();
}

CapOpWnd::~CapOpWnd()
{
}

void CapOpWnd::Construct()
{
    // Vertical layot - toolbar - view
    iVbox = new CagVBox("Vbox");
    Add(iVbox);
    iVbox->Show();
    // Toolbar
    iToolbar = new CapOpWndToolbar("Toolbar");
    iVbox->PackStart(iToolbar, false, false, 1);
    iToolbar->iBtnUp->SetObserver(this);
    iToolbar->iBtnBack->SetObserver(this);
    iToolbar->Show();
}


void CapOpWnd::AddView(CagWidget* aView)
{
    iVbox->PackStart(aView, false, false, 1);
    gtk_drag_dest_set(aView->iWidget, GTK_DEST_DEFAULT_ALL, KTe_NewObject, KTe_NewObject_Len, GDK_ACTION_COPY);
//    GtkTargetList* tarlist = gtk_target_list_new(KTe_NewObject, KTe_NewObject_Len);
//    gtk_target_list_add_table(tarlist, KTe_NewState, KTe_NewState_Len);
//    gtk_drag_dest_set_target_list(aView->iWidget, tarlist);
    CapSys* sys = (CapSys*) aView;
//    sys->iHead->DragDestAdd(GTK_DEST_DEFAULT_ALL, KTe_Conn, KTe_Conn_Len, GDK_ACTION_COPY);
}

void CapOpWnd::RemoveView(CagWidget* aView)
{
    iVbox->Remove(aView);
}

void CapOpWnd::SetSys(CAE_Object::Ctrl* aObj)
{
    _FAP_ASSERT(iSysWidget == NULL);
    iSysWidget = new CapSys("System", *aObj, this);
    AddView(iSysWidget);
    iSysWidget->Show();
}

void CapOpWnd::UnsetSys(CAE_Object::Ctrl* aObj)
{
    _FAP_ASSERT(iSysWidget != NULL);
    RemoveView(iSysWidget);
    delete iSysWidget;
    iSysWidget = NULL;
}

void CapOpWnd::SetObserver(MOpWndObserver* aObs)
{
    _FAP_ASSERT(iObs == NULL);
    iObs = aObs;
    // Update toolbuttons
    UpdateCmds();
}

void CapOpWnd::UpdateCmds()
{
    iToolbar->iBtnBack->SetSensitive(iObs->OnCmdUpdateRequest(MOpWndObserver::ECmd_Back));
    iToolbar->iBtnUp->SetSensitive(iObs->OnCmdUpdateRequest(MOpWndObserver::ECmd_Up));
}

void CapOpWnd::OnCompSelected(CAE_Object* aComp)
{
    if (iObs != NULL) {
	iObs->OnTurnToComp(aComp);
	UpdateCmds();
    }
}

void CapOpWnd::OnSystSelected(const string& aName)
{
    if (iObs != NULL) {
	iObs->OnTurnToSyst(aName);
	UpdateCmds();
    }
}

// Toolbar buttons handling
void CapOpWnd::OnClicked(CagToolButton* aBtn)
{
    if (aBtn == iToolbar->iBtnUp) {
	if (iObs!= NULL) {
	    iObs->OnCmd(MOpWndObserver::ECmd_Up);
	    UpdateCmds();
	}
    }
    else if (aBtn == iToolbar->iBtnBack) {
	if (iObs!= NULL) {
	    iObs->OnCmd(MOpWndObserver::ECmd_Back);
	    UpdateCmds();
	}
    }
}

void* CapOpWnd::DoGetObj(const char *aName)
{
    if (strcmp(aName, MagToolButtonObserver::Type()) == 0)
	return (MagToolButtonObserver*) this;
    else if (strcmp(aName, Type()) == 0) 
	return this;
    else return CagWindow::DoGetObj(aName);
}

