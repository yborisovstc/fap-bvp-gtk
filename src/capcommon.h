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
const TInt KViewBtnHeightFromHintVar = 4;
const TInt KViewBtnBoxInnerBoard = 0;
const TInt KViewTransMinWidth = 30;

// Targets entries for drag-drop
const TInt KTei_NewObject = 1;
const TInt KTei_NewState = 2;

static GtkTargetEntry KTe_NewObject[] = { 
    { (gchar*) "STRING", 0, KTei_NewObject } 
};
const TInt KTe_NewObject_Len = sizeof(KTe_NewObject)/sizeof(GtkTargetEntry);

static GtkTargetEntry KTe_NewState[] = { 
    { (gchar*) "STRING", 0, KTei_NewState } 
};
const TInt KTe_NewState_Len = sizeof(KTe_NewState)/sizeof(GtkTargetEntry);



#endif
