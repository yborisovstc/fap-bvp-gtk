#ifndef __FAPBVP_GTK_TOOLBAR_H
#define __FAPBVP_GTK_TOOLBAR_H

#include "cagcontainer.h"
#include "cagbin.h"

class CagToolItem;
class CagToolBar: public CagContainer
{
    public:
	CagToolBar(const string& aName);
	void Insert(CagToolItem* aItem, int aPos);
	void InsertSeparator(int aPos, TBool aAsLine);
};

class CagToolItem: public CagBin
{
    public:
	CagToolItem(const string& aName);
	void SetImage(const string& aFileName);
    protected:
	CagToolItem(GType aType, const string& aName);
};

class CagLabel;
class CagLabelToolItem: public CagToolItem
{
    public:
	CagLabelToolItem(const string& aName);
	void SetLabel(const string& aText);
    private:
	CagLabel* iLabel;
};

class CagToolButton;
class MagToolButtonObserver
{
    public:
	static inline const char* Type() { return "MagToolButtonObserver";} ; 
	virtual void OnClicked(CagToolButton* aBtn) = 0;
};

class CagToolButton: public CagToolItem
{
    public:
	CagToolButton(const string& aName);
	CagToolButton(const string& aName, const string& aStockId);
	void SetStockId(const string& aStockId);
	void SetIcon(CagWidget* aIcon);
	void SetImage(const string& aPath);
	void SetLabel(const string& aLabel);
	void SetObserver(CagWidget* aObs);
    protected:
	CagToolButton(GType aType, const string& aName);
    private:
	static void handle_button_clicked(GtkToolButton *toolbutton, gpointer user_data);
};


#endif
