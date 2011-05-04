#include "cagimage.h"
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

void CagToolItem::SetImage(const string& aPath)
{
    CagEImage* img =  new CagEImage("Img");
    img->SetFromFile(aPath.c_str());
    Add(img);
    img->Show();
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

void CagToolButton::SetObserver(CagWidget* aObs)
{
    MagToolButtonObserver* obs = aObs->GetObj(obs);
    _FAP_ASSERT(obs != NULL);
    g_signal_connect(G_OBJECT(iWidget), "clicked", G_CALLBACK (handle_button_clicked), aObs);
}

void CagToolButton::handle_button_clicked(GtkToolButton *toolbutton, gpointer user_data)
{
    CagWidget* hler = (CagWidget*) user_data;
    MagToolButtonObserver* obs = hler->GetObj(obs);
    _FAP_ASSERT(obs != NULL);
    CagWidget* wid = hler->GetWidget(GTK_WIDGET(toolbutton));
    _FAP_ASSERT(wid != NULL);
    CagToolButton* btn = wid->GetObj(btn);
    return obs->OnClicked(btn);
}

