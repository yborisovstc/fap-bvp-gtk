
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

void CapLogDlg::GetLogSpecUpdate(map<TLeBase, TInt>& aUpdate)
{
    TInt trans = iLineTrans->GetChecked("Execution") ? KBaseDa_Trex: 0;
    if (iLogSpec[KBaseLe_Trans] != trans) 
	aUpdate[KBaseLe_Trans] = trans;
    TInt upd_curr = iLineUpdate->GetChecked("Current") ? KBaseDa_Curr: 0;
    TInt upd_new = iLineUpdate->GetChecked("New") ? KBaseDa_New: 0;
    TInt upd_deps = iLineUpdate->GetChecked("Dependencies") ? KBaseDa_Dep: 0;
    TInt upd = upd_curr | upd_new | upd_deps;
    if (iLogSpec[KBaseLe_Updated] != upd)
	aUpdate[KBaseLe_Updated] = upd;
    TInt cre = iLineCreat->GetChecked("All") ? KBaseDa_New: 0;
    if (iLogSpec[KBaseLe_Creation] != cre)
	aUpdate[KBaseLe_Creation] = cre;
}


// Comp dialog
CapSimpleDlgLine::CapSimpleDlgLine(const string& aName, const string& aLabel): CagHBox(aName)
{
    CagLabel* sLab = new CagLabel("Lab");
    sLab->SetText(aLabel + "  :  ");
    sLab->Show();
    PackStart(sLab, false, false, 2);
}

CapEntryDlgLine::CapEntryDlgLine(const string& aName, const string& aLabel, const string& aData):
    CapSimpleDlgLine(aName, aLabel)
{
    iEntry = new CagEntry("Entry");
    iEntry->SetText(aData.c_str());
    iEntry->Show();
    PackStart(iEntry, false, false, 2);
}

void CapEntryDlgLine::GetData(string& aData)
{
    iEntry->GetText(aData);
}

CapCboxEntryDlgLine::CapCboxEntryDlgLine(const string& aName, const string& aLabel, const vector<string>& aList, const string& aData):
    CapSimpleDlgLine(aName, aLabel)
{
    iEntry = new CagComboBoxListEntry("Entry", aList, aList.begin());
    iEntry->SetButtonSensitivity(GTK_SENSITIVITY_AUTO);
    iEntry->Show();
    PackStart(iEntry, false, false, 2);
}

void CapCboxEntryDlgLine::GetData(string& aData)
{
    iEntry->GetText(aData);
}



CapCompDlg::CapCompDlg(const string& aName, const vector<string>& aTypes, TBool aObj, const string& aCompName, const string& aCompType): 
    CagDialog(aName)
{
    // Buttons
    AddButton("Cancel", EActionCancel);
    AddButton("OK", EActionOK);
    // Line "Name"
    iLineName = new CapEntryDlgLine("Name", "Name", aCompName);
    iLineName->Show();
    AddContentWidget(iLineName, EFalse, ETrue, 1);
    // Line "Type"
    iLineType = new CapCboxEntryDlgLine("Type", "Type", aTypes, aCompType);
    iLineType->Show();
    AddContentWidget(iLineType, EFalse, ETrue, 1);
}

void CapCompDlg::GetName(string& aName)
{
    iLineName->GetData(aName);
}

void CapCompDlg::GetType(string& aType)
{
    iLineType->GetData(aType);
    if (aType.compare("<none>") == 0) {
	aType = "none";
    }
}

// Move dialog
CapMoveDlg::CapMoveDlg(const string& aName, const vector<string>& aNodes, const string& aInitSel): CagDialog(aName)
{
    // Buttons
    AddButton("Cancel", EActionCancel);
    AddButton("OK", EActionOK);
    // Line "Nodes"
    iLineNode = new CapCboxEntryDlgLine("Nodes", "Nodes", aNodes, aInitSel);
    iLineNode->Show();
    AddContentWidget(iLineNode, EFalse, ETrue, 1);
}

void CapMoveDlg::GetSelNode(string& aName)
{
    iLineNode->GetData(aName);
}
