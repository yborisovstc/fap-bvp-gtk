#ifndef __FAPBVP_GTK_CAPLOGDLG_H
#define __FAPBVP_GTK_CAPLOGDLG_H

#include "cagdialog.h"
#include "cagbox.h"
#include "cagentry.h"
#include "cagmenu.h"
#include "capcommon.h"


class CapLogDlgLine: public CagHBox
{
    public:
	CapLogDlgLine(const string& aName, const string& aEvent);
	void AddDataCheckbox(const string& aLabel, TBool aChecked);
	TBool GetChecked(const string& aName);
};

class CapLogDlg: public CagDialog
{
    public:
	enum {
	    EActionCancel,
	    EActionOK
	};
    public:
	CapLogDlg(const string& aName, CAE_EBase& aElem);
	void GetLogSpecUpdate(map<TLeBase, TInt>& aUpdate);
    private:
	map<TLeBase, TInt> iLogSpec;
	CapLogDlgLine* iLineTrans;
	CapLogDlgLine* iLineUpdate;
	CapLogDlgLine* iLineCreat;
};



class CapSimpleDlgLine: public CagHBox
{
    public:
	CapSimpleDlgLine(const string& aName, const string& aLabel);
};

class CapEntryDlgLine: public CapSimpleDlgLine
{
    public:
	CapEntryDlgLine(const string& aName, const string& aLabel, const string& aData);
	void GetData(string& aData);
    private:
	CagEntry* iEntry;
};

class CapCboxEntryDlgLine: public CapSimpleDlgLine
{
    public:
	CapCboxEntryDlgLine(const string& aName, const string& aLabel, const vector<string>& aList, const string& aData);
	void GetData(string& aData);
    private:
	CagComboBoxListEntry* iEntry;
};

class CapCompDlg: public CagDialog
{
    public:
	enum {
	    EActionCancel,
	    EActionOK
	};
    public:
	CapCompDlg(const string& aName, const vector<string>& aTypes, TBool aObj, const string& aCompName, const string& aCompType);
	void GetName(string& aName);
	void GetType(string& aType);
    private:
	CapEntryDlgLine* iLineName;
	CapCboxEntryDlgLine* iLineType;
};

class CapMoveDlg: public CagDialog
{
    public:
	enum {
	    EActionCancel,
	    EActionOK
	};
    public:
	CapMoveDlg(const string& aName, const vector<string>& aNodes, const string& aInitSel);
	void GetSelNode(string& aName);
    private:
	CapCboxEntryDlgLine* iLineNode;
};

#endif
