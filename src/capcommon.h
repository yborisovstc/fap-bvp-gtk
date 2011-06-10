#ifndef __FAPBVP_GTK_COMMN_H
#define __FAPBVP_GTK_COMMN_H

#include <fapbase.h>
#include <gtk/gtk.h>

// Parameters of view
const TInt KViewNameLineHeight = 20;
const TInt KViewExtLineHeight = 20;
const TInt KViewExtAreaWidth = 90;
const TInt KViewCompAreaWidth = 200;
const TInt KViewCompGapHight = 20;
const TInt KViewStateWidth = 300;
const TInt KViewConnLineLen = 10;
const TInt KViewConnLabelGapWidth = 2;
const TInt KViewConnIdHeight = 16;
const TInt KViewConnIdWidth = 120;
const TInt KViewConnIdMaxNum = 1;
const TInt KViewConnGapWidth = 3;
const TInt KViewConnGapHeight = 1;
const TInt KViewTransWidth = 200;
const TInt KViewTransHeight = 100;
const TInt KViewStateGapTransInp = 20;
const TInt KViewCompInpOutpGapWidth = 20;
const TInt KViewCompCpGapHeight = 3;
const TInt KViewDectGapWidth = 0;
const TInt KViewExtCompGapWidth = 40;
const TInt KViewStateTransBorder = 1;
const TInt KViewStateTransIntBorder = 2;
const TInt KViewBtnHeightFromHintVar = 4;
const TInt KViewBtnBoxInnerBoard = 0;
const TInt KViewTransMinWidth = 30;
const TInt KViewBorder = 2;

// Targets entries for drag-drop
const TInt KTei_NewObject = 1;
const TInt KTei_NewState = 2;
const TInt KTei_Conn = 3; // Connection point
const TInt KTei_NewTrans = 4;
const TInt KTei_NewInp = 5;
const TInt KTei_NewOutp = 6;

static GtkTargetEntry KTe_NewObject[] = { 
    { (gchar*) "STRING", 0, KTei_NewObject } 
};
const TInt KTe_NewObject_Len = sizeof(KTe_NewObject)/sizeof(GtkTargetEntry);

static GtkTargetEntry KTe_NewState[] = { 
    { (gchar*) "STRING", 0, KTei_NewState } 
};
const TInt KTe_NewState_Len = sizeof(KTe_NewState)/sizeof(GtkTargetEntry);

static GtkTargetEntry KTe_NewTrans[] = { 
    { (gchar*) "STRING", 0, KTei_NewTrans } 
};
const TInt KTe_NewTrans_Len = sizeof(KTe_NewTrans)/sizeof(GtkTargetEntry);

static GtkTargetEntry KTe_Conn[] = { 
    { (gchar*) "STRING", 0, KTei_Conn } 
};
const TInt KTe_Conn_Len = sizeof(KTe_Conn)/sizeof(GtkTargetEntry);

static GtkTargetEntry KTe_NewInp[] = { 
    { (gchar*) "STRING", 0, KTei_NewInp } 
};
const TInt KTe_NewInp_Len = sizeof(KTe_NewInp)/sizeof(GtkTargetEntry);

static GtkTargetEntry KTe_NewOutp[] = { 
    { (gchar*) "STRING", 0, KTei_NewOutp } 
};
const TInt KTe_NewOutp_Len = sizeof(KTe_NewOutp)/sizeof(GtkTargetEntry);



struct TPmenuSpecElem {
    TPmenuSpecElem(const string& aName, const string& aLabel): iName(aName), iLabel(aLabel) {};
    string iName;
    string iLabel;
};




#endif
