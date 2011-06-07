#ifndef __FAPBVP_GTK_CAPLOGDLG_H
#define __FAPBVP_GTK_CAPLOGDLG_H

#include "cagdialog.h"
#include "cagbox.h"

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
	void GetLogSpec(map<TInt, TInt>& aLogSpec);
    private:
	map<TInt, TInt> iLogSpec;
	CapLogDlgLine* iLineTrans;
	CapLogDlgLine* iLineUpdate;
	CapLogDlgLine* iLineCreat;
};

#endif
