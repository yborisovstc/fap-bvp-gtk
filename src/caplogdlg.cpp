
#include "caplogdlg.h"
#include "caglabel.h"
#include "cagbutton.h"

CapLogDlgLine::CapLogDlgLine(const string& aName, const string& aEvent): CagHBox(aName)
{
    CagLabel* sEvent = new CagLabel("Event");
    sEvent->SetText(aEvent + "  :  ");
    sEvent->Show();
    PackStart(sEvent, false, false, 2);
}


void CapLogDlgLine::AddDataCheckbox(const string& aLabel, TBool aChecked)
{
    CagCheckButton* btn = new CagCheckButton(aLabel);
    btn->SetLabel(aLabel);
    btn->SetActive(aChecked);
    btn->Show();
    PackStart(btn, false, false, 2);
}

TBool CapLogDlgLine::GetChecked(const string& aName)
{
    CagWidget* btn = Child(aName);
    CagToggleButton* tbtn = btn->GetObj(tbtn);
    return tbtn->IsActive();
}




CapLogDlg::CapLogDlg(const string& aName, CAE_EBase& aElem): CagDialog(aName)
{
    // Get current logspec
    iLogSpec[KBaseLe_Trans] = aElem.GetLogSpecData(KBaseLe_Trans);
    iLogSpec[KBaseLe_Updated] = aElem.GetLogSpecData(KBaseLe_Updated);
    iLogSpec[KBaseLe_Creation] = aElem.GetLogSpecData(KBaseLe_Creation);
    // Buttons
    AddButton("Cancel", EActionCancel);
    AddButton("OK", EActionOK);
    // Line "Transition"
    iLineTrans = new CapLogDlgLine("Trans", "Transition");
    iLineTrans->AddDataCheckbox("Execution", iLogSpec[KBaseLe_Trans] & KBaseDa_Trex);
    iLineTrans->Show();
    AddContentWidget(iLineTrans, EFalse, ETrue, 1);
    // Line "Update"
    iLineUpdate = new CapLogDlgLine("Upd", "Update");
    iLineUpdate->AddDataCheckbox("Current", iLogSpec[KBaseLe_Updated] & KBaseDa_Curr);
    iLineUpdate->AddDataCheckbox("New", iLogSpec[KBaseLe_Updated] & KBaseDa_New);
    iLineUpdate->AddDataCheckbox("Dependencies", iLogSpec[KBaseLe_Updated] & KBaseDa_Dep);
    iLineUpdate->Show();
    AddContentWidget(iLineUpdate, EFalse, ETrue, 1);
    // Line "Creation"
    iLineCreat = new CapLogDlgLine("Creat", "Creation");
    iLineCreat->AddDataCheckbox("All", iLogSpec[KBaseLe_Creation] & KBaseDa_New);
    iLineCreat->Show();
    AddContentWidget(iLineCreat, EFalse, ETrue, 1);
}

void CapLogDlg::GetLogSpec(map<TInt, TInt>& aLogSpec)
{
    aLogSpec[KBaseLe_Trans] = iLineTrans->GetChecked("Execution") ? KBaseDa_Trex: 0;
    TInt upd_curr = iLineUpdate->GetChecked("Current") ? KBaseDa_Curr: 0;
    TInt upd_new = iLineUpdate->GetChecked("New") ? KBaseDa_New: 0;
    TInt upd_deps = iLineUpdate->GetChecked("Dependencies") ? KBaseDa_Dep: 0;
    aLogSpec[KBaseLe_Updated] = upd_curr | upd_new | upd_deps;
    aLogSpec[KBaseLe_Creation] = iLineCreat->GetChecked("All") ? KBaseDa_New: 0;
}

