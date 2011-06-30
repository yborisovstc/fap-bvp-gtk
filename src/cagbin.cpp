#include "cagbin.h"


CagBin::CagBin(const string& aName): CagContainer(GTK_TYPE_BIN, aName)
{
}

CagBin::CagBin(GType aType, const string& aName): CagContainer(aType, aName)
{
}

CagBin::CagBin(GtkWidget* aWidget, const string& aName, TBool aOwned): CagContainer(aWidget, aName, aOwned)
{
}

CagBin::~CagBin()
{
}


CagEventBox::CagEventBox(const string& aName): CagBin(GTK_TYPE_EVENT_BOX, aName)
{
}


CagItem::CagItem(const string& aName): CagBin(GTK_TYPE_ITEM, aName)
{
}

CagAlignment::CagAlignment(const string& aName, gfloat xalign, gfloat yalign, gfloat xscale, gfloat yscale): CagBin(GTK_TYPE_ALIGNMENT, aName) 
{
    gtk_alignment_set(GTK_ALIGNMENT(iWidget), xalign, yalign, xscale, yscale);
}

CagComboBox::CagComboBox(const string& aName): CagBin(gtk_combo_box_new_text(), aName)
{
}

CagComboBox::CagComboBox(GType aType, const string& aName): CagBin(aType, aName)
{
}

CagComboBox::CagComboBox(GtkWidget* aWidget, const string& aName): CagBin(aWidget, aName)
{
}

void CagComboBox::GetText(string& aText)
{
    aText = (const char*) gtk_combo_box_get_active_text(GTK_COMBO_BOX(iWidget));
}

void CagComboBox::AppendText(const string& aText)
{
    gtk_combo_box_append_text(GTK_COMBO_BOX(iWidget), aText.c_str());
}

void CagComboBox::SetActiveItem(TInt aInd)
{
    gtk_combo_box_set_active(GTK_COMBO_BOX(iWidget), aInd);
}

void CagComboBox::SetButtonSensitivity(GtkSensitivityType aSens)
{
    gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(iWidget), aSens);
}


CagComboBoxEntry::CagComboBoxEntry(const string& aName): CagComboBox(gtk_combo_box_entry_new_text(), aName)
{
}

CagComboBoxListEntry::CagComboBoxListEntry(const string& aName, const vector<string>& aList, vector<string>::const_iterator aCurrent):
    CagComboBoxEntry(aName)
{
    for (vector<string>::const_iterator it = aList.begin(); it != aList.end(); it++) {
	AppendText(*it);
    }
    SetActiveItem(0);
}

