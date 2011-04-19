#include "caglayout.h"


CagLayout::CagLayout(const string& aName): CagContainer(gtk_layout_new(NULL, NULL), aName)
{
}

CagLayout::~CagLayout()
{
}

GdkWindow* CagLayout::BinWnd()
{
    return gtk_layout_get_bin_window(GTK_LAYOUT(iWidget));
}

