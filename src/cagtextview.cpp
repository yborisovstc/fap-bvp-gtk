#include "cagtextview.h"

static void handle_clicked_event(GtkButton *button, gpointer user_data);

CagTextView::CagTextView(const string& aName): CagContainer(GTK_TYPE_TEXT_VIEW, aName)
{
}

void CagTextView::SetBuffer(GtkTextBuffer* aBuffer)
{
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(iWidget), aBuffer);
}

void CagTextView::SetEditable(TBool aEditable)
{
    gtk_text_view_set_editable(GTK_TEXT_VIEW(iWidget), aEditable);
}

