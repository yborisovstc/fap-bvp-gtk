#ifndef __FAPBVP_GTK_BIN_H
#define __FAPBVP_GTK_BIN_H

#include "cagcontainer.h"

class CagBin: public CagContainer 
{
    public:
	CagBin(const string& aName);
	CagBin(GType aType, const string& aName);
	virtual ~CagBin();
    protected:
	CagBin(GtkWidget* aWidget, const string& aName, TBool aOwned = ETrue);
};

class CagEventBox: public CagBin 
{
    public:
	CagEventBox(const string& aName);
};

class CagItem: public CagBin
{
    public:
	CagItem(const string& aName);
};

class CagAlignment: public CagBin
{
    public: 
	CagAlignment(const string& aName, gfloat xalign, gfloat yalign, gfloat xscale, gfloat yscale);
};


class CagComboBox: public CagBin
{
    public:
	CagComboBox(const string& aName);
	CagComboBox(GType aType, const string& aName);
	void GetText(string& aText);
	void AppendText(const string& aText);
	void SetActiveItem(TInt aInd);
	void SetButtonSensitivity(GtkSensitivityType aSens);
    protected:
	CagComboBox(GtkWidget* aWidget, const string& aName);
};

class CagComboBoxEntry: public CagComboBox
{
    public:
	CagComboBoxEntry(const string& aName);
};

class CagComboBoxListEntry: public CagComboBoxEntry
{
    public:
	CagComboBoxListEntry(const string& aName, const vector<string>& aList, vector<string>::const_iterator aCurrent);
};

#endif 
