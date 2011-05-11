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

string CagTextView::GetBuffer()
{
    GtkTextBuffer* buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(iWidget));
    GtkTextIter start; gtk_text_buffer_get_start_iter(buf, &start);
    GtkTextIter end; gtk_text_buffer_get_end_iter(buf, &end);
    gchar* text = gtk_text_buffer_get_text(buf, &start, &end, FALSE);
    return string((char*) text);
}

