#include "cagtoolbar.h"

CagToolBar::CagToolBar(const string& aName): CagContainer(GTK_TYPE_TOOLBAR, aName)
{
}


void CagToolBar::Insert(CagToolItem* aItem, int aPos)
{
    gtk_toolbar_insert(GTK_TOOLBAR(iWidget), GTK_TOOL_ITEM(aItem->iWidget), aPos);
    Add(aItem, EFalse);
}




CagToolItem::CagToolItem(const string& aName): CagBin(GTK_TYPE_TOOL_ITEM, aName)
{
}

CagToolItem::CagToolItem(GType aType, const string& aName): CagBin(aType, aName)
{
}


CagToolButton::CagToolButton(const string& aName): CagToolItem(GTK_TYPE_TOOL_BUTTON, aName)
{
}

CagToolButton::CagToolButton(const string& aName, const string& aStockId): CagToolItem(GTK_TYPE_TOOL_BUTTON, aName)
{
    SetStockId(aStockId);
}

CagToolButton::CagToolButton(GType aType, const string& aName): CagToolItem(aType, aName)
{
}

void CagToolButton::SetStockId(const string& aStockId)
{
    gtk_tool_button_set_stock_id(GTK_TOOL_BUTTON(iWidget), aStockId.c_str());
}


