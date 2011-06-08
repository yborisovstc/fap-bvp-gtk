
#include "capopwnd.h"
#include "capcommon.h"

const char* KIconsDir = "/usr/share/fap-bvp-gtk/icons/";
const char* KIcon_NewSyst = "/usr/share/fap-bvp-gtk/icons/tbar_btn_syst.png";
const char* KIcon_NewState = "/usr/share/fap-bvp-gtk/icons/tbar_btn_state.png";
const char* KIcon_NewTrans = "/usr/share/fap-bvp-gtk/icons/tbar_btn_trans.png";
const char* KIcon_NewInp = "/usr/share/fap-bvp-gtk/icons/tbar_btn_inp.png";
const char* KIcon_NewOutp = "/usr/share/fap-bvp-gtk/icons/tbar_btn_outp.png";

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


CapOpWnd::CapOpWnd(const string& aName, CagToolBar* aToolbar): 
    CagLayout(aName), iSysWidget(NULL), iObs(NULL), iToolbar(aToolbar)
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
    // Adding buttons to toolbar
    // Button "Back"
    CagToolButton* sBtnBack = new CagToolButton("BtnBack", GTK_STOCK_GO_BACK);
    sBtnBack->SetObserver(this);
    iToolbar->Insert(sBtnBack, 0);
    sBtnBack->Show();
    // Button Up
    CagToolButton* sBtnUp = new CagToolButton("BtnUp", GTK_STOCK_GO_UP);
    sBtnUp->SetObserver(this);
    iToolbar->Insert(sBtnUp, 0);
    sBtnUp->Show();
    // Separator
    iToolbar->InsertSeparator(-1, ETrue);
    // Button "New state"
    CapOtbDragItem* sBtnNewState = new CapOtbDragItem("BntNewState", KTe_NewState, KTe_NewState_Len, "_new_state");
    sBtnNewState->SetImage(KIcon_NewState);
    iToolbar->Insert(sBtnNewState, -1);
    sBtnNewState->Show();
    // Button "New system"
    CapOtbDragItem* sBtnNewSyst = new CapOtbDragItem("BntNewSyst", KTe_NewObject, KTe_NewObject_Len, "_new_object");
    sBtnNewSyst->SetImage(KIcon_NewSyst);
    iToolbar->Insert(sBtnNewSyst, -1);
    sBtnNewSyst->Show();
    // Button "New trans"
    CapOtbDragItem* sBtnNewTrans = new CapOtbDragItem("BntNewTrans", KTe_NewTrans, KTe_NewTrans_Len, "_new_trans");
    sBtnNewTrans->SetImage(KIcon_NewTrans);
    iToolbar->Insert(sBtnNewTrans, -1);
    sBtnNewTrans->Show();
    // Button "New Outp"
    CapOtbDragItem* sBtnNewOutp = new CapOtbDragItem("BntNewOutp", KTe_NewOutp, KTe_NewOutp_Len, "_new_outp");
    sBtnNewOutp->SetImage(KIcon_NewOutp);
    iToolbar->Insert(sBtnNewOutp, -1);
    sBtnNewOutp->Show();
    // Button "New Inp"
    CapOtbDragItem* sBtnNewInp = new CapOtbDragItem("BntNewInp", KTe_NewInp, KTe_NewInp_Len, "_new_inp");
    sBtnNewInp->SetImage(KIcon_NewInp);
    iToolbar->Insert(sBtnNewInp, -1);
    sBtnNewInp->Show();
    // Separator
    iToolbar->InsertSeparator(-1, ETrue);
    // Cursor
    iCursName = new CagLabelToolItem("CursName");
    iCursName->SetLabel("Test");
    iToolbar->Insert(iCursName, -1);
    iCursName->Show();
    // Separator
    iToolbar->InsertSeparator(-1, ETrue);
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
    iCursName->SetLabel(aObj->Object().InstName());
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
    if (iToolbar) {
    iToolbar->Child("BtnBack")->SetSensitive(iObs->OnCmdUpdateRequest(MOpWndObserver::ECmd_Back));
    iToolbar->Child("BtnUp")->SetSensitive(iObs->OnCmdUpdateRequest(MOpWndObserver::ECmd_Up));
    }
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
    if (aBtn == iToolbar->Child("BtnUp")) {
	if (iObs!= NULL) {
	    iObs->OnCmd(MOpWndObserver::ECmd_Up);
	    UpdateCmds();
	}
    }
    else if (aBtn == iToolbar->Child("BtnBack")) {
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
    else return CagLayout::DoGetObj(aName);
}

void CapOpWnd::OnSizeAllocate(GtkAllocation* aAllocation)
{
}

void CapOpWnd::OnSizeRequest(GtkRequisition* aRequisition)
{
    GtkRequisition child_req; iVbox->SizeRequest(&child_req);
    *aRequisition = child_req;
}

