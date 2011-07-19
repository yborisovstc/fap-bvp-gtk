
#include <stdlib.h>
#include "capsys.h"
#include "cagproxy.h"
#include "capcomp.h"
#include "capstate.h"
#include "capcp.h"
#include "capcommon.h"
#include "capcterm.h"
#include "cagtextview.h"
#include "caplogdlg.h"
#include <dirent.h>

const char* KFapModulesPath = "file:/usr/share/fapws/modules/";


vector<string> CapSys::iModulesPaths;

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



CapSys::CapSys(const string& aName, CAE_Object::Ctrl& aSys, MCapSysObserver* aObserver): 
    CagLayout(aName), iSys(aSys), iObserver(aObserver), iCpPairObs(*this), iTrans(NULL)
{
    if (iModulesPaths.size() == 0) {
	iModulesPaths.push_back(KFapModulesPath);
    }
    Construct();
}

void CapSys::Construct()
{
    // Add states
    for (map<string, CAE_StateBase*>::iterator it = iSys.States().begin(); it != iSys.States().end(); it++) {
	CAE_StateBase* state = it->second;
	if (state != NULL) {
	    CapState* stw = new CapState("State~" + string(state->Name()), *state, iSys);
	    Add(stw);
	    iStates[state] = stw;
	    stw->SetObs(this);
	    stw->Show();
	}
    }
    // Add components
    for (vector<CAE_Object*>::iterator it = iSys.CompsOrd().begin(); it != iSys.CompsOrd().end(); it++) {
	CAE_Object* obj = *it;
	if (obj != NULL) {
	    CapComp* comp = new CapComp("Comp~" + string(obj->Name()), *obj);
	    Add(comp);
	    iComps.push_back(comp);
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
    if (!iSys.Trans().iETrans.empty()) {
	iTrans = new CapTran("Transs", iSys.Trans().iETrans);
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
    // Allocate inputs
    int inpb_x = aAllc->width - iInpReq.width, inpb_y = KViewCompGapHight;
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
    int outpb_x = 0, outpb_y = KViewCompGapHight;
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
    int tranb_x = (outp_w + aAllc->width - inp_w)/2, tranb_y = KViewCompGapHight;
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
    for (vector<CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	CapComp* comp = *it;
	GtkRequisition req; comp->SizeRequest(&req);
	int comp_body_center_x = comp->GetBodyCenterX();
	GtkAllocation allc = {compb_x - comp_body_center_x, compb_y, req.width, req.height};
	comp->SizeAllocate(&allc);
	compb_y += req.height + KViewCompGapHight;
    }
}

void CapSys::OnSizeRequest(GtkRequisition* aRequisition)
{
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
    for (vector<CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	CapComp* comp = *it;
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
    aRequisition->height = max(max(outp_h, comp_h + stat_h + tran_req.height + KViewExtCompGapWidth), inp_h);
}


CapComp* CapSys::Comp(CagWidget* aWidget)
{
    CapComp* res = NULL;
    for (vector<CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	if (*it == aWidget) {
	    res = *it; break;
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
    for (vector<CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	res = (*it)->GetCpPair(aPair);
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
    string dtext = (char*) gtk_selection_data_get_text(data);
    size_t act_end = dtext.find_first_of(":");
    string act = dtext.substr(0, act_end);
    if (act.compare("_new_object") == 0) 
	// TODO [YB] info received is incorrect for some reason. Consider
   // if (info == KTei_NewObject)
    {
	//gdk_drag_status(drag_context, GDK_ACTION_COPY, time);
	vector<string> sTypes;
	GetCompTypesAvailable(sTypes);
	CapCompDlg* dlg = new CapCompDlg("CompDlg", sTypes, ETrue, "", "");
	TInt res = dlg->Run();
	if (res == CapLogDlg::EActionOK) {
	    string sName, sType;
	    dlg->GetName(sName);
	    dlg->GetType(sType);
	    AddComponent(sName, sType);
	}
	delete dlg;
	gtk_drag_finish(drag_context, true, false, time);
    }
    else if (strcmp((char*) gtk_selection_data_get_text(data), "_new_state") == 0) 
    {
	vector<string> sTypes;
	GetStateTypesAvailable(sTypes);
	CapCompDlg* dlg = new CapCompDlg("CompDlg", sTypes, ETrue, "", "");
	TInt res = dlg->Run();
	if (res == CapLogDlg::EActionOK) {
	    string sName, sType;
	    dlg->GetName(sName);
	    dlg->GetType(sType);
	    AddState(sName, sType);
	}
	delete dlg;
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
    else if (act.compare("_move_comp") == 0) 
    {
	string cname = dtext.substr(act_end+1);
	MoveComp(cname, x, y);
	gtk_drag_finish(drag_context, true, false, time);
    }
}

void CapSys::AddComponent(const string& aName, const string& aType) 
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode comp = smutr.AddChild(ENt_Object);
    comp.SetAttr(ENa_Type, aType);
    char *name = (char*) malloc(100);
    sprintf(name, "noname%d", rand());
    comp.SetAttr(ENa_Id, aName);
    free(name);
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::AddState(const string& aName, const string& aType)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode comp = smutr.AddChild(ENt_State);
    comp.SetAttr(ENa_Type, aType);
    char *name = (char*) malloc(100);
    sprintf(name, "noname%d", rand());
    comp.SetAttr(ENa_Id, aName);
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
    if (iTrans != NULL) {
	Remove(iTrans);
    }
    for (map<CAE_StateBase*, CapState*>::iterator it = iStates.begin(); it != iStates.end(); it++) {
	Remove(it->second);
    }
    iStates.erase(iStates.begin(), iStates.end());
    for (vector<CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	Remove(*it);
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
    RenameNode(&aComp->iComp, aName);
}

void CapSys::OnStateNameChanged(CapState* aState, const string& aName)
{
    RenameNode(&aState->iState, aName);
}

void CapSys::OnStateTypeChanged(CapState* aState, const string& aTypeName)
{
    ChangeNodeAttr(&aState->iState, ENa_Type, aTypeName);
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
    CAE_Object* mutmgr = iSys.Object().FindMutableMangr();
    CAE_Object::ChromoPx* cpx = mutmgr->ChromoIface();
    CAE_ChromoNode smut = cpx->Mut().Root();
    CAE_ChromoNode mutrm = smut.AddChild(ENt_Rm);
    DesUri uri(&aState->iState, mutmgr);
    mutrm.SetAttr(ENa_MutNode, uri.GetUri());
    mutmgr->Mutate();
    Refresh();
}

void CapSys::DeleteComp(CapComp* aComp)
{
    CAE_Object* mutmgr = iSys.Object().FindMutableMangr();
    CAE_Object::ChromoPx* cpx = mutmgr->ChromoIface();
    CAE_ChromoNode smut = cpx->Mut().Root();
    CAE_ChromoNode mutrm = smut.AddChild(ENt_Rm);
    DesUri uri(&aComp->iComp, mutmgr);
    mutrm.SetAttr(ENa_MutNode, uri.GetUri());
    mutmgr->Mutate();
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
    smut.SetAttr(ENa_MutNode, MAE_Chromo::GetTName(ENt_State, aState->iState.Name()));
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
    RenameNode(&aCp->iCp, aName);
}

void CapSys::RenameNode(CAE_NBase* aNode, const string& aName)
{
    ChangeNodeAttr(aNode, ENa_Id, aName);
}

void CapSys::OnStateInpRenamed(CapState* aState, CapCp* aCp, const string& aName)
{
    RenameNode(&aCp->iCp, aName);
}

void CapSys::OnStateTransUpdated(CapState* aState, const string& aTrans)
{
    ChangeStateTrans(aState, aTrans);
}

void CapSys::OnStateInitUpdated(CapState* aState, const string& aInit)
{
    ChangeNodeAttr(&aState->iState, ENa_StInit, aInit);
}

void CapSys::ChangeNodeAttr(CAE_NBase* aNode, TNodeAttr aAttr, const string& aVal)
{
    CAE_Object* mutmgr = iSys.Object().FindMutableMangr();
    CAE_Object::ChromoPx* cpx = mutmgr->ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode chg = smutr.AddChild(ENt_Change);
    DesUri uri(aNode, mutmgr);
    chg.SetAttr(ENa_MutNode, uri.GetUri());
    chg.SetAttr(ENa_MutChgAttr, aAttr);
    chg.SetAttr(ENa_MutChgVal, aVal);
    mutmgr->Mutate();
    Refresh();
}

void CapSys::ChangeStateTrans(CapState* aState, const string& aTrans)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    smut.SetAttr(ENa_MutNode, MAE_Chromo::GetTName(ENt_State, aState->iState.Name()));
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
    TBool islocinp = iSys.Object().Inputs().count(aPairName) > 0;
    TBool islocoutp = iSys.Object().Outputs().count(aPairName) > 0;
    TBool islocext = islocinp || islocoutp;
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    if (islocext) {
	CAE_ChromoNode nd_add_subj = smutr.AddChild(ENt_Cext);
	// TODO [YB] To migrate to getting uri via NBase and base
	DesUri iduri(islocinp ? ENt_Stinp: ENt_Soutp, aPairName);
	nd_add_subj.SetAttr(ENa_Id, iduri.GetUri());
	DesUri puri(&aCp->iCp, &iSys.Object());
	nd_add_subj.SetAttr(ENa_ConnPair, puri.GetUri());
    }
    else {
	CAE_ChromoNode nd_add_subj = smutr.AddChild(ENt_Conn);
	TBool cpinp = aState->iInps.count(&(aCp->iCp)) > 0;
	DesUri iduri(&aCp->iCp, &iSys.Object());
	if (cpinp) {
	    nd_add_subj.SetAttr(ENa_Id, iduri.GetUri());
	    nd_add_subj.SetAttr(ENa_ConnPair, aPairName);
	}
	else {
	    nd_add_subj.SetAttr(ENa_Id, aPairName);
	    nd_add_subj.SetAttr(ENa_ConnPair, iduri.GetUri());
	}
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

void CapSys::OnStateCpDelPairRequested(CapState* aState, CapCp* aCp, CapCtermPair* aPair)
{
//    DelCpPair(MAE_Chromo::GetTName(ENt_State, string(aState->iState.Name())), aState->iInps.count(&(aCp->iCp)) > 0, aCp, aPairName);
    // Not completed yet. Waiting for fapws RmNode getting ready.
    DelCpPair_v1(&(aState->iState), aState->iInps.count(&(aCp->iCp)) > 0, aCp, aPair);
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


void CapSys::OnStateLogspecChanged(CapState* aState, map<TLeBase, TInt>& aLogSpec)
{
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_EBase& ebase = aState->iState;
    CAE_ChromoNode smutr = cpx->Mut().Root();
    for (map<TLeBase, TInt>::const_iterator it = aLogSpec.begin(); it != aLogSpec.end(); it++) {
	TLeBase event = it->first;
	TInt data = it->second;
	// Remove current log first
	if (ebase.LogSpec().count(DesUri::LeventName(event)) > 0) {
	    CAE_ChromoNode rm = smutr.AddChild(ENt_Rm);
	    DesUri uri(&aState->iState, &iSys.Object());
	    uri.AppendElem(ENt_Logspec, DesUri::LeventName(event));
	    rm.SetAttr(ENa_MutNode, uri.GetUri());
	}
	// Then add logspec
	if (data != KBaseDa_None) {
	    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
	    smut.SetAttr(ENa_MutNode, MAE_Chromo::GetTName(ENt_State, aState->iState.Name()));
	    CAE_ChromoNode mutadd = smut.AddChild(ENt_MutAdd);
	    CAE_ChromoNode nlsp = mutadd.AddChild(ENt_Logspec);
	    nlsp.SetAttr(ENa_Logevent, DesUri::LeventName(event));
	    if (event == KBaseLe_Updated) {
		if (data & KBaseDa_Curr) {
		    CAE_ChromoNode nlda = nlsp.AddChild(ENt_Logdata);
		    nlda.SetAttr(ENa_Id, "cur");
		}
		if (data & KBaseDa_New) {
		    CAE_ChromoNode nlda = nlsp.AddChild(ENt_Logdata);
		    nlda.SetAttr(ENa_Id, "new");
		}
	    }
	    else if (event == KBaseLe_Trans) {
		if (data & KBaseDa_Trex) {
		    CAE_ChromoNode nlda = nlsp.AddChild(ENt_Logdata);
		    nlda.SetAttr(ENa_Id, "trex");
		}
	    }
	    else if (event == KBaseLe_Creation) {
		if (data != KBaseDa_None) {
		    CAE_ChromoNode nlda = nlsp.AddChild(ENt_Logdata);
		    nlda.SetAttr(ENa_Id, "new");
		}
	    }
	}
    }
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnCompCpRenamed(CapComp* aComp, CapCp* aCp, const string& aName, TBool aIsOutp)
{
    // TODO [YB] Disable renaming comps connpoints
}

void CapSys::OnCompCpAddPairRequested(CapComp* aComp, CapCp* aCp, const string& aPairName)
{
    AddCompCpPair(aComp, aCp, aPairName);
}

void CapSys::AddCompCpPair(CapComp* aComp, CapCp* aCp, const string& aPairName)
{
    TBool islocinp = iSys.Object().Inputs().count(aPairName) > 0;
    TBool islocoutp = iSys.Object().Outputs().count(aPairName) > 0;
    TBool islocext = islocinp || islocoutp;
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode ndadd = smut.AddChild(ENt_MutAdd);
    if (islocext) {
	CAE_ChromoNode nd_add_subj = ndadd.AddChild(ENt_Cext);
	nd_add_subj.SetAttr(ENa_Id, aPairName);
	nd_add_subj.SetAttr(ENa_ConnPair, string(aComp->iComp.Name()) + "." + aCp->iCp.Name());
    }
    else {
	CAE_ChromoNode nd_add_subj = ndadd.AddChild(ENt_Conn);
	TBool cpinp = aComp->iInps.count(&(aCp->iCp)) > 0;
	string cpname = MAE_Chromo::GetTName(ENt_Object, string(aComp->iComp.Name()) + "." + aCp->iCp.Name());
	if (cpinp) {
	    nd_add_subj.SetAttr(ENa_Id, cpname);
	    nd_add_subj.SetAttr(ENa_ConnPair, aPairName);
	}
	else {
	    nd_add_subj.SetAttr(ENa_Id, aPairName);
	    nd_add_subj.SetAttr(ENa_ConnPair, cpname);
	}
    }
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnCompCpDelPairRequested(CapComp* aComp, CapCp* aCp, const string& aPairName)
{
    DelCpPair(MAE_Chromo::GetTName(ENt_Object, string(aComp->iComp.Name())), aComp->iInps.count(&(aCp->iCp)) > 0, aCp, aPairName);
}

void CapSys::DelCpPair_v1(CAE_EBase* aCpOwner, TBool aIsInp, CapCp* aCp, CapCtermPair* aPair)
{
    CAE_NBase* pair = aPair->Pair();
    TBool islocinp = iSys.Object().Inputs().count(pair->Name()) > 0;
    TBool islocoutp = iSys.Object().Outputs().count(pair->Name()) > 0;
    TBool islocext = islocinp || islocoutp;

    CAE_Object* mutmgr = iSys.Object().FindMutableMangr();
    CAE_Object::ChromoPx* cpx = mutmgr->ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode rm = smutr.AddChild(ENt_Rm);
    DesUri uri(&iSys.Object(), mutmgr);
    DesUri cpuri(&aCp->iCp, &(iSys.Object()));
    DesUri pruri(pair, mutmgr);
    if (islocext) {
	uri.AppendElem(ENt_Cext, "");
	uri.AppendQueryElem(DesUri::EQop_Unknown, ENa_Id, pruri.GetUri());
	uri.AppendQueryElem(DesUri::EQop_And, ENa_ConnPair, cpuri.GetUri());
    }
    else {
	uri.AppendElem(ENt_Conn, "");
	if (aIsInp) {
	    uri.AppendQueryElem(DesUri::EQop_Unknown, ENa_Id, cpuri.GetUri());
	    uri.AppendQueryElem(DesUri::EQop_And, ENa_ConnPair, pruri.GetUri());
	}
	else {
	    uri.AppendQueryElem(DesUri::EQop_Unknown, ENa_Id, pruri.GetUri());
	    uri.AppendQueryElem(DesUri::EQop_And, ENa_ConnPair, cpuri.GetUri());
	}
    }
    rm.SetAttr(ENa_MutNode, uri.GetUri());
    mutmgr->Mutate();
    Refresh();
}

void CapSys::DelCpPair(string aMansFullName, TBool aIsInp, CapCp* aCp, const string& aPairName)
{
    TBool islocinp = iSys.Object().Inputs().count(aPairName) > 0;
    TBool islocoutp = iSys.Object().Outputs().count(aPairName) > 0;
    TBool islocext = islocinp || islocoutp;
    string fname = aMansFullName + "." + aCp->iCp.Name();
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode rm = smut.AddChild(ENt_MutRm);
    if (islocext) {
	rm.SetAttr(ENa_Type, ENt_Cext);
	rm.SetAttr(ENa_MutChgAttr, "pair");
	rm.SetAttr(ENa_Id, aPairName);
	rm.SetAttr(ENa_MutChgVal, fname);
    }
    else {
	rm.SetAttr(ENa_Type, ENt_Conn);
	rm.SetAttr(ENa_MutChgAttr, "pair");
	if (aIsInp) {
	    rm.SetAttr(ENa_Id, fname);
	    rm.SetAttr(ENa_MutChgVal, aPairName);
	}
	else {
	    rm.SetAttr(ENa_Id, aPairName);
	    rm.SetAttr(ENa_MutChgVal, fname);
	}
    }
    iSys.Object().Mutate();
    Refresh();
}

void CapSys::OnCompDeleteRequested(CapComp* aComp)
{
    DeleteComp(aComp);
}

void CapSys::GetStateTypesAvailable(vector<string>& aList) const
{
    aList.push_back("StInt");
    aList.push_back("Vector");
}

void CapSys::GetCompTypesAvailable(vector<string>& aList) const
{
    aList.push_back("<none>");
    iSys.Object().AppendCompList(aList, NULL);
    for (map<string, CAE_Object*>::const_iterator it = iSys.Comps().begin(); it != iSys.Comps().end(); it++) 
    {
	aList.push_back(it->first);
    }
    AddCompTypesFromModPaths(aList);
}

void CapSys::AddCompTypesFromModPaths(vector<string>& aList) const
{
    for (vector<string>::const_iterator it = iModulesPaths.begin(); it != iModulesPaths.end(); it++) {
	const string& uristr = *it;
	Uri uri(uristr);
	if (uri.Scheme().compare("file") == 0 && uri.Auth().empty()) {
	    // Local host
	    AddCompTypesFromLocModPath(uristr, uri.Path(), aList);
	}
    }
}

int CapSys::FilterModulesDirEntries(const struct dirent *aEntry)
{
    string name = aEntry->d_name;
    size_t ppos = name.find_first_of(".");
    string suff = name.substr(ppos + 1);
    int res = suff.compare("xml"); 
    return (res == 0) ? 1 : 0;
}

void CapSys::AddCompTypesFromLocModPath(const string& aDirUri, const string& aPath, vector<string>& aList) const
{
    // List modules directory
    struct dirent **entlist;
    int n = scandir (aPath.c_str(), &entlist, FilterModulesDirEntries, alphasort);
    // Add root systems 
    for (int cnt = 0; cnt < n; ++cnt) {
	aList.push_back(aDirUri + entlist[cnt]->d_name);
    }
}

void CapSys::MoveComp(const string& aName, gint aX, gint aY)
{
    string targ;
    FindCompByPosY(targ, aY);
    CAE_Object::ChromoPx* cpx = iSys.Object().ChromoIface();
    CAE_ChromoNode smutr = cpx->Mut().Root();
    CAE_ChromoNode smut = smutr.AddChild(ENt_Mut);
    CAE_ChromoNode chnode = smut.AddChild(ENt_MutMove);
    chnode.SetAttr(ENa_Id, aName);
    if (!targ.empty()) {
	chnode.SetAttr(ENa_MutNode, targ);
    }
    iSys.Object().Mutate();
    Refresh();

}

void CapSys::FindCompByPosY(string& aCompName, gint aY)
{
    for (vector<CapComp*>::iterator it = iComps.begin(); it != iComps.end(); it++) {
	CapComp* comp = *it;
	GtkAllocation alloc;
	comp->Allocation(&alloc);
	if (aY >= alloc.y && aY < (alloc.y + alloc.height + KViewExtCompGapWidth)) {
	    aCompName = comp->iComp.Name();
	}
    }
}

void CapSys::OnCompChangeQuietRequested(CapComp* aComp, TBool aQuiet)
{
    ChangeNodeAttr(&aComp->iComp, ENa_ObjQuiet, aQuiet? "yes" : "no");
}

