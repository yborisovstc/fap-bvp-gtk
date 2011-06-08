#include "cagimage.h"
#include "cagtoolbar.h"
#include "caglabel.h"

CagToolBar::CagToolBar(const string& aName): CagContainer(GTK_TYPE_TOOLBAR, aName)
{
}


void CagToolBar::Insert(CagToolItem* aItem, int aPos)
{
    gtk_toolbar_insert(GTK_TOOLBAR(iWidget), GTK_TOOL_ITEM(aItem->iWidget), aPos);
    Add(aItem, EFalse);
}

void CagToolBar::InsertSeparator(int aPos, TBool aAsLine)
{
    GtkToolItem* sep = gtk_separator_tool_item_new();
    gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(sep), aAsLine);
    gtk_widget_show(GTK_WIDGET(sep));
    gtk_toolbar_insert(GTK_TOOLBAR(iWidget), sep, aPos);
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

// Label Tool Item
CagLabelToolItem::CagLabelToolItem(const string& aName): CagToolItem(aName)
{
    iLabel = new CagLabel("Lbl");
    Add(iLabel);
    iLabel->Show();
}

void CagLabelToolItem::SetLabel(const string& aText)
{
    iLabel->SetText(aText);
}

// Tool button
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

void CagToolButton::SetLabel(const string& aLabel)
{
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(iWidget), aLabel.c_str());
}

void CagToolButton::SetIcon(CagWidget* aIcon)
{
    gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(iWidget), aIcon->iWidget);
}

void CagToolButton::SetImage(const string& aPath)
{
    CagEImage* img =  new CagEImage("Img");
    img->SetFromFile(aPath.c_str());
    Add(img);
    img->Show();
    SetIcon(img);
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

