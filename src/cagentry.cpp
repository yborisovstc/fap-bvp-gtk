#include "cagentry.h"


CagEntry::CagEntry(const string& aName): CagWidget(GTK_TYPE_ENTRY, aName)
{
}

void CagEntry::SetText(const string& aText)
{
    gtk_entry_set_text(GTK_ENTRY(iWidget), aText.c_str());
}

string CagEntry::GetText()
{
    return string((const char*) gtk_entry_get_text(GTK_ENTRY(iWidget)));
}

TInt CagEntry::GetTextLen()
{
    return gtk_entry_get_text_length(GTK_ENTRY(iWidget));
}

void CagEntry::SetHasFrame(TBool aHas)
{
    gtk_entry_set_has_frame(GTK_ENTRY(iWidget), aHas);
}

void CagEntry::SetInnerBorder(const GtkBorder *border)
{
    gtk_entry_set_inner_border(GTK_ENTRY(iWidget), border);
}

void CagEntry::SetWidthChars(TInt aNchars)
{
    gtk_entry_set_width_chars(GTK_ENTRY(iWidget), aNchars);
}

void CagEntry::SetEditable(TBool aEditable)
{
    gtk_editable_set_editable(GTK_EDITABLE(iWidget), aEditable);
}

void CagEntry::SetPosition(TInt aPos)
{
    gtk_editable_set_position(GTK_EDITABLE(iWidget), aPos);
}

