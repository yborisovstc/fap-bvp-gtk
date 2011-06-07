
#include <stdlib.h>
#include "capsys.h"
#include "cagproxy.h"
#include "capcomp.h"
#include "capstate.h"
#include "capcp.h"
#include "capcommon.h"
#include "capcterm.h"
#include "cagtextview.h"

CapTranHead::CapTranHead(const string& aName): CagHBox(aName)
{
    SetBorderWidth(1);
    // Create Type
    iLabel = new CagLabel("Label");
    iLabel->SetPadding(0, 0);
    iLabel->SetText("Trans");
    iLabel->Show();
    PackStart(iLabel, false, false, 2);
}

CapTran::CapTran(const string& aName, const string& aTranData): CagLayout(aName), iTranData(aTranData)
{
    // Create header
    iHead = new CapTranHead("Head");
    iHead->Show();
    Add(iHead);
    // Create trans
    iTrans = new CagTextView("Trans");
    GtkTextBuffer* buf = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(buf, aTranData.c_str(), aTranData.size());
    iTrans->SetBuffer(buf);
    iTrans->SetEditable(ETrue);
    iTrans->SetBorderWidth(10);
    iTrans->Show();
    Add(iTrans);
}

void CapTran::OnExpose(GdkEventExpose* aEvent)
{
    // Draw body rect
    GtkAllocation alc; Allocation(&alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, 0, 0, alc.width - 1, alc.height - 1);
    // Head separator
    GtkAllocation head_alc; iHead->Allocation(&head_alc);
    gdk_draw_line(BinWnd(), Gc(), 0, head_alc.height, alc.width - 1, head_alc.height);
}

void CapTran::OnSizeAllocate(GtkAllocation* aAllc)
{
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    GtkRequisition tran_req; iTrans->SizeRequest(&tran_req);

    // Allocate header
    GtkAllocation head_alc = {0, 0, aAllc->width, head_req.height};
    iHead->SizeAllocate(&head_alc);

    // Allocate trans
    GtkAllocation tran_alc = 
	(GtkAllocation) {KViewBorder, head_alc.y + head_alc.height + KViewBorder, tran_req.width, tran_req.height};
    iTrans->SizeAllocate(&tran_alc);
}

void CapTran::OnSizeRequest(GtkRequisition* aRequisition)
{
    // Calculate header size
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    // Calculate trans size
    GtkRequisition tran_req; iTrans->SizeRequest(&tran_req);
    aRequisition->width = max(head_req.width, tran_req.width) + KViewBorder*2; 
    aRequisition->height = head_req.height + tran_req.height + KViewBorder*2;
}



CapSysHead::CapSysHead(const string& aName, CAE_Object::Ctrl& aSys): CagLayout(aName), iSys(aSys)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iSys.Object().InstName(), strlen(iSys.Object().InstName()));
    int w, h; pango_layout_get_pixel_size(plo, &w, &h);
    gtk_widget_set_size_request(iWidget, w, h);
    g_object_unref(plo);
}

CapSysHead::~CapSysHead()
{
}


void CapSysHead::OnExpose(GdkEventExpose* aEvent)
{
    PangoLayout* plo =  pango_layout_new(PangoCtext());
    pango_layout_set_text(plo, iSys.Object().InstName(), strlen(iSys.Object().InstName()));
    gdk_draw_layout(BinWnd(), Gc(), 0, 0, plo);
    GtkAllocation alc; gtk_widget_get_allocation(iWidget, &alc);
    gdk_draw_rectangle(BinWnd(), Gc(), FALSE, alc.x, alc.y, alc.width - 1, alc.height - 1);
    g_object_unref(plo);
}

TBool CapSysHead::OnButtonPress(GdkEventButton* aEvent)
{
    if (aEvent->type == GDK_BUTTON_PRESS) {
	SetState(GTK_STATE_SELECTED);
    }
}

TBool CapSysHead::OnButtonRelease(GdkEventButton* aEvent)
{
}

void CapSysHead::OnSizeAllocate(GtkAllocation* aAllocation)
{
    
}

void CapSysHead::OnSizeRequest(GtkRequisition* aRequisition)
{
}

void CapSysHead::OnMotion(GdkEventMotion *aEvent)
{
}

void CapSysHead::OnEnter(GdkEventCrossing *aEvent)
{
}

void CapSysHead::OnLeave(GdkEventCrossing *aEvent)
{
}

void CapSysHead::OnStateChanged(GtkStateType state)
{
    if (State() == GTK_STATE_SELECTED) {
	iParent->OnChildStateChanged(this, state);
    }
}




CapSys::CapSys(const string& aName, CAE_Object::Ctrl& aSys, MCapSysObserver* aObserver): 
    CagLayout(aName), iSys(aSys), iObserver(aObserver), iCpPairObs(*this), iTrans(NULL)
{
    Construct();
}

void CapSys::Construct()
{
    // Add header
    iHead = new CapSysHead("Title", iSys);
    Add(iHead);
    iHead->Show();

    // Add states
    for (map<string, CAE_StateBase*>::iterator it = iSys.States().begin(); it != iSys.States().end(); it++) {
	CAE_StateBase* state = it->second;
	if (state != NULL) {
	    CapState* stw = new CapState("State~" + string(state->InstName()), *state, iSys);
	    Add(stw);
	    iStates[state] = stw;
	    stw->SetObs(this);
	    stw->Show();
	}
    }
    // Add components
    for (map<string, CAE_Object*>::iterator it = iSys.Comps().begin(); it != iSys.Comps().end(); it++) {
	CAE_Object* obj = it->second;
	if (obj != NULL) {
	    CapComp* comp = new CapComp("Comp~" + string(obj->InstName()), *obj);
	    Add(comp);
	    iComps[obj] = comp;
	    comp->SetObs(this);
	    comp->Show();
	}
    }
    // Add inputs extenders
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iSys.Object().Inputs().begin(); it != iSys.Object().Inputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp("Inp~" + cp->Name(), *cp, ETrue, ETrue);
	Add(cpw);
	cpw->SetObs(this);
	iInputs[cp] = cpw;
	cpw->Show();
    }
    // Add outputs extenderd
    for (map<string, CAE_ConnPointBase*>::const_iterator it = iSys.Object().Outputs().begin(); it != iSys.Object().Outputs().end(); it++) {
	CAE_ConnPointBase* cp = it->second;
	CapCp* cpw = new CapCp("Outp~" + cp->Name(), *cp, ETrue, EFalse);
	Add(cpw);
	cpw->SetObs(this);
	iOutputs[cp] = cpw;
	cpw->Show();
    }
    // Add trans
    if (!iSys.Trans().empty()) {
	iTrans = new CapTran("Transs", iSys.Trans());
	Add(iTrans);
	iTrans->Show();
	iTrans->iTrans->SetWidgetObs(this);
    }
}

CapSys::~CapSys()
{
}

void* CapSys::DoGetObj(const char *aName)
{
    if (strcmp(aName, Type()) == 0) 
	return this;
    else if (strcmp(aName, MWidgetObs::Type()) == 0)
	return (MWidgetObs*) this;
    else return CagLayout::DoGetObj(aName);
}

void CapSys::OnExpose(GdkEventExpose* aEvent)
{
}

void CapSys::OnSizeAllocate(GtkAllocation* aAllc)
{
    GtkAllocation alc; Allocation(&alc);
    // Allocate header
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    GtkAllocation head_alc = { 0, 0, alc.width, head_req.height};
    iHead->SizeAllocate(&head_alc);
    // Allocate inputs
    int inpb_x = aAllc->width - iInpReq.width, inpb_y = head_req.height + KViewCompGapHight;
    int inp_w = 0, inp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInputs.begin(); it != iInputs.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	GtkAllocation allc = {inpb_x, inpb_y, req.width, req.height};
	cpw->SizeAllocate(&allc);
	inp_w = max(inp_w, req.width);
	inpb_y += req.height + KViewConnGapHeight;
    }
    // Allocate outputs
    int outpb_x = 0, outpb_y = head_req.height + KViewCompGapHight;
    int outp_w = 0, outp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutputs.begin(); it != iOutputs.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	GtkAllocation allc = {outpb_x, outpb_y, req.width, req.height};
	cpw->SizeAllocate(&allc);
	outp_w = max(outp_w, req.width);
	outpb_y += req.height + KViewConnGapHeight;
    }
    // Allocate trans
    GtkRequisition tran_req = (GtkRequisition) {0, 0}; 
    if (iTrans != NULL) {
	iTrans->SizeRequest(&tran_req);
    }
    int tranb_x = (outp_w + aAllc->width - inp_w)/2, tranb_y = head_req.height + KViewCompGapHight;
    GtkAllocation tran_alc = { tranb_x - tran_req.width/2, tranb_y, tran_req.width, tran_req.height};
    if (iTrans != NULL) {
	iTrans->SizeAllocate(&tran_alc);
    }
    // Allocate states
    int statb_x = (outp_w + aAllc->width - inp_w)/2, statb_y = tran_alc.y + tran_alc.height + KViewCompGapHight;
    for (map<CAE_StateBase*, CapState*>::iterator it = iStates.begin(); it != iStates.end(); it++) {
	CAE_StateBase* state = it->first;
	CapState* stw = it->second;
	GtkRequisition req; stw->SizeRequest(&req);
	int body_center_x = stw->GetBodyCenterX();
	GtkAllocation allc = {statb_x - body_center_x, statb_y, req.width, req.height};
	stw->SizeAllocate(&allc);
	statb_y += req.height + KViewCompGapHight;
    }
    // Allocate components
    int compb_x = (outp_w + aAllc->width - inp_w)/2, compb_y = statb_y;
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	CAE_Object* obj = it->first;
	CapComp* comp = it->second;
	GtkRequisition req; comp->SizeRequest(&req);
	int comp_body_center_x = comp->GetBodyCenterX();
	GtkAllocation allc = {compb_x - comp_body_center_x, compb_y, req.width, req.height};
	comp->SizeAllocate(&allc);
	compb_y += req.height + KViewCompGapHight;
    }
}

void CapSys::OnSizeRequest(GtkRequisition* aRequisition)
{
    GtkRequisition head_req; iHead->SizeRequest(&head_req);
    // Calculate trans size
    GtkRequisition tran_req = (GtkRequisition) {0, 0}; 
    if (iTrans != NULL) {
	iTrans->SizeRequest(&tran_req);
    }
    // Calculate size of states
    int stat_w = 0, stat_h = 0;
    for (map<CAE_StateBase*, CapState*>::iterator it = iStates.begin(); it != iStates.end(); it++) {
	CapState* state = it->second;
	GtkRequisition req; state->SizeRequest(&req);
	stat_w = max(stat_w, req.width);
	stat_h += req.height + KViewCompGapHight;
    }
    // Calculate size of comps
    int comp_w = 0, comp_h = 0;
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	CapComp* comp = it->second;
	GtkRequisition req; comp->SizeRequest(&req);
	comp_w = max(comp_w, req.width);
	comp_h += req.height + KViewCompGapHight ;
    }
    // Calculate size of inputs
    int inp_w = 0, inp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInputs.begin(); it != iInputs.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	inp_w = max(inp_w, req.width);
	inp_h += req.height + KViewConnGapHeight;
    }
    iInpReq.width = inp_w; iInpReq.height = inp_h;
    // Calculate size of outputs
    int outp_w = 0, outp_h = 0;
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutputs.begin(); it != iOutputs.end(); it++) {
	CapCp* cpw = it->second;
	GtkRequisition req; cpw->SizeRequest(&req);
	outp_w = max(outp_w, req.width);
	outp_h += req.height + KViewConnGapHeight;
    }

    aRequisition->width = outp_w + KViewExtCompGapWidth*2 + max(max(stat_w, comp_w), tran_req.width) + inp_w; 
    aRequisition->height = head_req.height + max(max(outp_h, comp_h + stat_h + tran_req.height + KViewExtCompGapWidth), inp_h);
}


CapComp* CapSys::Comp(CagWidget* aWidget)
{
    CapComp* res = NULL;
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	if (it->second == aWidget) {
	    res = it->second; break;
	}
    }
    return res;
}

void CapSys::ActivateConn(CapCtermPair* aPair)
{
    CapCtermPair* pair = GetCpPair(aPair);
    if (pair != NULL) {
	if (aPair->IsActive() && !pair->IsActive()) {
	    pair->SetActive(ETrue);
	}
	else if (!aPair->IsActive() && pair->IsActive()) {
	    pair->SetActive(EFalse);
	}
    }
}

void CapSys::CpPairObs::OnToggled(CagToggleButton* aBtn)
{
}

void CapSys::OnCompCpPairToggled(CapComp* aComp, CapCtermPair* aPair)
{
    ActivateConn(aPair);
}

void CapSys::OnCompNameClicked(CapComp* aComp)
{
    if (iObserver != NULL) {
	iObserver->OnCompSelected(&(aComp->iComp));
    }
}

void CapSys::OnCompParentClicked(CapComp* aComp)
{
    if (iObserver != NULL) {
	iObserver->OnSystSelected(aComp->iComp.TypeName());
    }
}

CapCtermPair* CapSys::GetCpPair(CapCtermPair* aPair)
{
    CapCtermPair* res = NULL;
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    for (map<CAE_StateBase*, CapState*>::iterator it = iStates.begin(); it != iStates.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutputs.begin(); it != iOutputs.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInputs.begin(); it != iInputs.end() && res == NULL; it++) {
	res = it->second->GetCpPair(aPair);
    }
    return res;
}

void CapSys::OnStateCpPairToggled(CapState* aComp, CapCtermPair* aPair)
{
    ActivateConn(aPair);
}

void CapSys::OnCpPairToggled(CapCp* aCp, CapCtermPair* aPair)
{
    ActivateConn(aPair);
}

TBool CapSys::OnDragDrop(GdkDragContext *drag_context, gint x, gint y, guint time)
{
}

void CapSys::OnDragDataReceived(GdkDragContext *drag_context, gint x, gint y, GtkSelectionData *data, guint info, guint time)
{
    if (strcmp((char*) gtk_selection_data_get_text(data), "_new_object") == 0) 
	// TODO [YB] info received is incorrect for some reason. Consider
   // if (info == KTei_NewObject)
    {
	//gdk_drag_status(drag_context, GDK_ACTION_COPY, time);
	AddComponent();
	gtk_drag_finish(drag_context, true, false, time);
    }
    else if (strcmp((char*) gtk_selection_data_get_text(data), "_new_state") == 0) 
    {
	AddState();
	gtk_drag_finish(drag_context, true, false, time);
    }
    else if (strcmp((char*) gtk_selection_data_get_text(data), "_new_trans") == 0) 
    {
	AddTrans();
	gtk_drag_finish(drag_context, true, false, time);
    }
    else if (strcmp((char*) gtk_selection_data_get_text(data), "_new_inp") == 0) 
    {
	AddInp();
	gtk_drag_finish(drag_context, true, false, time);
    }
    else if (strcmp((char*) gtk_selection_data_get_text(data), "_new_outp") == 0) 
    {
	AddOutp();
	gtk_drag_finish(drag_context, true, false, time);
    }
}

void CapSys::AddComponent() 
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode comp = smutr.AddChild(ENt_Object);
    comp.SetAttr(ENa_Type, "none");
    char *name = (char*) malloc(100);
    sprintf(name, "noname%d", rand());
    comp.SetAttr(ENa_Id, name);
    free(name);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::AddState()
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode mutadd = smut.AddChild(ENt_MutAdd);
    CAE_ChromoNode comp = mutadd.AddChild(ENt_State);
    comp.SetAttr(ENa_Type, "StInt");
    char *name = (char*) malloc(100);
    sprintf(name, "noname%d", rand());
    comp.SetAttr(ENa_Id, name);
    comp.SetAttr(ENa_StInit, "");
    free(name);
    comp.AddChild(ENt_Trans);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::AddTrans()
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode stran = smutr.AddChild(ENt_Trans);
    stran.SetContent("rem To add transition here");
    iSys.Object().Mutate();
    Refresh();
}

string CapSys::GetRandomNum() const
{
    char *name = (char*) malloc(100);
    sprintf(name, "%d", rand());
    string res(name);
    free(name);
    return res;
}

void CapSys::AddInp()
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Stinp);
    smut.SetAttr(ENa_Id, string("inp") + GetRandomNum());
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::AddOutp()
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Soutp);
    smut.SetAttr(ENa_Id, string("outp") + GetRandomNum());
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::Refresh()
{
    // Remove all elements
    Remove(iHead);
    if (iTrans != NULL) {
	Remove(iTrans);
    }
    iHead = NULL;
    for (map<CAE_StateBase*, CapState*>::iterator it = iStates.begin(); it != iStates.end(); it++) {
	Remove(it->second);
    }
    iStates.erase(iStates.begin(), iStates.end());
    for (map<CAE_Object*, CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	Remove(it->second);
    }
    iComps.erase(iComps.begin(), iComps.end());
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iInputs.begin(); it != iInputs.end(); it++) {
	Remove(it->second);
    }
    iInputs.erase(iInputs.begin(), iInputs.end());
    for (map<CAE_ConnPointBase*, CapCp*>::iterator it = iOutputs.begin(); it != iOutputs.end(); it++) {
	Remove(it->second);
    }
    iOutputs.erase(iOutputs.begin(), iOutputs.end());
    // Construct again
    Construct();
}

void CapSys::OnCompNameChanged(CapComp* aComp, const string& aName)
{
    ChangeCompName(aComp, aName);
}

void CapSys::ChangeCompName(CapComp* aComp, const string& aName)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode chnode = smut.AddChild(ENt_MutChange);
    chnode.SetAttr(ENa_Type, "iobject");
    chnode.SetAttr(ENa_Id, aComp->iComp.InstName());
    chnode.SetAttr(ENa_MutChgAttr, "id");
    chnode.SetAttr(ENa_MutChgVal, aName);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnStateNameChanged(CapState* aState, const string& aName)
{
    ChangeStateName(aState, aName);
}

void CapSys::OnStateTypeChanged(CapState* aState, const string& aTypeName)
{
    ChangeStateType(aState, aTypeName);
}

void CapSys::ChangeStateName(CapState* aState, const string& aName)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode chnode = smut.AddChild(ENt_MutChange);
    chnode.SetAttr(ENa_Type, "state");
    chnode.SetAttr(ENa_Id, aState->iState.InstName());
    chnode.SetAttr(ENa_MutChgAttr, "id");
    chnode.SetAttr(ENa_MutChgVal, aName);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::ChangeStateType(CapState* aState, const string& aTypeName)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode chnode = smut.AddChild(ENt_MutChange);
    chnode.SetAttr(ENa_Type, ENt_State);
    chnode.SetAttr(ENa_Id, aState->iState.InstName());
    chnode.SetAttr(ENa_MutChgAttr, ENa_Type);
    chnode.SetAttr(ENa_MutChgVal, aTypeName);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnStateDeleteRequested(CapState* aState)
{
    DeleteState(aState);
}

TBool CapSys::OnButtonPress(GdkEventButton* aEvent)
{
    if (aEvent->button == 3) {
	return ETrue;
    }
    return EFalse;
}

void CapSys::DeleteState(CapState* aState)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode mutrm = smut.AddChild(ENt_MutRm);
    mutrm.SetAttr(ENa_Type, ENt_State);
    mutrm.SetAttr(ENa_Id, aState->iState.InstName());
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnStateAddingInput(CapState* aState)
{
    AddStateInp(aState);
}

void CapSys::AddStateInp(CapState* aState)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    smut.SetAttr(ENa_MutNode, MAE_Chromo::GetTName(ENt_State, aState->iState.InstName()));
    CAE_ChromoNode mutadd = smut.AddChild(ENt_MutAdd);
    CAE_ChromoNode addstinp = mutadd.AddChild(ENt_Stinp);
    char *name = (char*) malloc(100);
    sprintf(name, "noname%d", rand());
    addstinp.SetAttr(ENa_Id, name);
    free(name);
    iSys.Object().Mutate();
    Refresh();
}


void CapSys::OnLabelRenamed(CapCp* aCp, const string& aName)
{
    RenameInpOutp(aCp, aName);
}

void CapSys::RenameInpOutp(CapCp* aCp, const string& aName)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode chnode = smut.AddChild(ENt_MutChange);
    TBool isoutp = iOutputs.count(&(aCp->iCp)) > 0; 
    chnode.SetAttr(ENa_Type, isoutp ? ENt_Soutp : ENt_Stinp);
    chnode.SetAttr(ENa_Id, aCp->iCp.Name());
    chnode.SetAttr(ENa_MutChgAttr, ENa_Id);
    chnode.SetAttr(ENa_MutChgVal, aName);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnStateInpRenamed(CapState* aState, CapCp* aCp, const string& aName)
{
    RenameStateInp(aState, aCp, aName);
}

void CapSys::RenameStateInp(CapState* aState, CapCp* aCp, const string& aName)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    smut.SetAttr(ENa_MutNode, MAE_Chromo::GetTName(ENt_State, aState->iState.InstName()));
    CAE_ChromoNode chnode = smut.AddChild(ENt_MutChange);
    chnode.SetAttr(ENa_Type, ENt_Stinp);
    chnode.SetAttr(ENa_Id, aCp->iCp.Name());
    chnode.SetAttr(ENa_MutChgAttr, ENa_Id);
    chnode.SetAttr(ENa_MutChgVal, aName);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnStateTransUpdated(CapState* aState, const string& aTrans)
{
    ChangeStateTrans(aState, aTrans);
}

void CapSys::OnStateInitUpdated(CapState* aState, const string& aInit)
{
    ChangeStateInit(aState, aInit);
}

void CapSys::ChangeStateInit(CapState* aState, const string& aInit)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode chnode = smut.AddChild(ENt_MutChange);
    chnode.SetAttr(ENa_Type, ENt_State);
    chnode.SetAttr(ENa_Id, aState->iState.InstName());
    chnode.SetAttr(ENa_MutChgAttr, ENa_StInit);
    chnode.SetAttr(ENa_MutChgVal, aInit);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::ChangeStateTrans(CapState* aState, const string& aTrans)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    smut.SetAttr(ENa_MutNode, MAE_Chromo::GetTName(ENt_State, aState->iState.InstName()));
    CAE_ChromoNode chnode = smut.AddChild(ENt_MutChangeCont);
    chnode.SetAttr(ENa_MutNode, MAE_Chromo::GetTName(ENt_Trans, ""));
    chnode.SetAttr(ENa_MutChgVal, aTrans);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnStateCpAddPairRequested(CapState* aState, CapCp* aCp, const string& aPairName)
{
    AddStateCpPair(aState, aCp, aPairName);
}

void CapSys::AddStateCpPair(CapState* aState, CapCp* aCp, const string& aPairName)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode ndadd = smut.AddChild(ENt_MutAdd);
    CAE_ChromoNode nd_add_subj = ndadd.AddChild(ENt_Conn);
    TBool cpinp = aState->iInps.count(&(aCp->iCp)) > 0;
    if (cpinp) {
	nd_add_subj.SetAttr(ENa_Id, MAE_Chromo::GetTName(ENt_State, string(aState->iState.InstName()) + "." + aCp->iCp.Name()));
	nd_add_subj.SetAttr(ENa_ConnPair, aPairName);
    }
    else {
	nd_add_subj.SetAttr(ENa_Id, aPairName);
	nd_add_subj.SetAttr(ENa_ConnPair, MAE_Chromo::GetTName(ENt_State, string(aState->iState.InstName()) + "." + aCp->iCp.Name()));
    }
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnCpAddPairRequested(CapCp* aCp, const string& aPairName)
{
}

void CapSys::OnCpDelPairRequested(CapCp* aCp, CapCtermPair* aPair)
{
}

void CapSys::DelStateCpPair(CapState* aState, CapCp* aCp, const string& aPairName)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode rm = smut.AddChild(ENt_MutRm);
    rm.SetAttr(ENa_Type, ENt_Conn);
    rm.SetAttr(ENa_MutChgAttr, "pair");
    TBool cpinp = aState->iInps.count(&(aCp->iCp)) > 0;
    if (cpinp) {
	rm.SetAttr(ENa_Id, MAE_Chromo::GetTName(ENt_State, string(aState->iState.InstName()) + "." + aCp->iCp.Name()));
	rm.SetAttr(ENa_MutChgVal, aPairName);
    }
    else {
	rm.SetAttr(ENa_Id, aPairName);
	rm.SetAttr(ENa_MutChgVal, MAE_Chromo::GetTName(ENt_State, string(aState->iState.InstName()) + "." + aCp->iCp.Name()));
    }
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnStateCpDelPairRequested(CapState* aState, CapCp* aCp, const string& aPairName)
{
    DelStateCpPair(aState, aCp, aPairName);
}

TBool CapSys::OnWidgetFocusOut(CagWidget* aWidget, GdkEventFocus* aEvent)
{
    TBool res = EFalse;
    if (aWidget == iTrans->iTrans) {
	ChangeTrans(iTrans->iTrans->GetBuffer());
    }
    return res;
}

void CapSys::ChangeTrans(const string& aTrans)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode chnode = smut.AddChild(ENt_MutChangeCont);
    chnode.SetAttr(ENa_MutNode, MAE_Chromo::GetTName(ENt_Trans, ""));
    chnode.SetAttr(ENa_MutChgVal, aTrans);
    iSys.Object().Mutate();
    Refresh();
}


void CapSys::OnStateLogspecChanged(CapState* aState, map<TInt, TInt>& aLogSpec)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    // Remove current log first
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    smut.SetAttr(ENa_MutNode, MAE_Chromo::GetTName(ENt_State, aState->iState.InstName()));
    CAE_ChromoNode mutrm = smut.AddChild(ENt_MutRm);
    mutrm.SetAttr(ENa_Type, ENt_Logspec);
    // Then add logspec
    TInt data = aLogSpec[KBaseLe_Updated];
    if (data != NULL) {
	CAE_ChromoNode mutadd = smut.AddChild(ENt_MutAdd);
	CAE_ChromoNode nlsp = mutadd.AddChild(ENt_Logspec);
	nlsp.SetAttr(ENa_Logevent, "upd");
	if (data & KBaseDa_Curr) {
	    CAE_ChromoNode nlda = nlsp.AddChild(ENt_Logdata);
	    nlda.SetAttr(ENa_Id, "cur");
	}
	if (data & KBaseDa_New) {
	    CAE_ChromoNode nlda = nlsp.AddChild(ENt_Logdata);
	    nlda.SetAttr(ENa_Id, "new");
	}
    }
    iSys.Object().Mutate();
    Refresh();
}
