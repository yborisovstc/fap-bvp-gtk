
#include "capopwnd.h"

CapOpWndToolbar::CapOpWndToolbar(const string& aName): CagToolBar(aName)
{
    // Button "Back"
    iBtnBack = new CagToolButton("BntBack", GTK_STOCK_GO_BACK);
    Insert(iBtnBack, 0);
    iBtnBack->Show();
    // Button "Up"
    iBtnUp = new CagToolButton("BntUp", GTK_STOCK_GO_UP);
    Insert(iBtnUp, 0);
    iBtnUp->Show();
}




CapOpWnd::CapOpWnd(const string& aName): CagWindow(aName), iSysWidget(NULL), iObs(NULL)
{
    Construct();
}

CapOpWnd::CapOpWnd(GtkWidget* aWidget, TBool aOwned): CagWindow(aWidget, aOwned), iSysWidget(NULL), iObs(NULL)
{
    Construct();
}

CapOpWnd::~CapOpWnd()
{
}

void CapOpWnd::Construct()
{
    iVbox = new CagVBox("Vbox");
    Add(iVbox);
    iVbox->Show();
    // Toolbar
    iToolbar = new CapOpWndToolbar("Toolbar");
//    Add(iToolbar);
    iVbox->PackStart(iToolbar, false, false, 1);
    iToolbar->Show();
}


void CapOpWnd::AddView(CagWidget* aView)
{
    iVbox->PackStart(aView, false, false, 1);
}

void CapOpWnd::RemoveView(CagWidget* aView)
{
    iVbox->Remove(aView);
}

void CapOpWnd::SetSys(CAE_Object::Ctrl* aObj)
{
    _FAP_ASSERT(iSysWidget == NULL);
    iSysWidget = new CapSys("System", *aObj, this);
    AddView(iSysWidget);
    iSysWidget->Show();
}

void CapOpWnd::UnsetSys(CAE_Object::Ctrl* aObj)
{
    _FAP_ASSERT(iSysWidget != NULL);
    RemoveView(iSysWidget);
    delete iSysWidget;
    iSysWidget = NULL;
}

void CapOpWnd::SetObserver(MOpWndObserver* aObs)
{
    _FAP_ASSERT(iObs == NULL);
    iObs = aObs;
}

void CapOpWnd::OnCompSelected(CAE_Object* aComp)
{
    if (iObs != NULL) {
	iObs->OnTurnToComp(aComp);
    }
}

void CapOpWnd::OnSystSelected(const string& aName)
{
    if (iObs != NULL) {
	iObs->OnTurnToSyst(aName);
    }
}

void CapOpWnd::OnClicked(CagToolButton* aBtn)
{
}

void* CapOpWnd::DoGetObj(const char *aName)
{
    if (strcmp(aName, MagToolButtonObserver::Type()) == 0)
	return (MagToolButtonObserver*) this;
    else if (strcmp(aName, Type()) == 0) 
	return this;
    else return CagWindow::DoGetObj(aName);
}
