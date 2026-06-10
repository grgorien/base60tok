#include <gtk/gtk.h>
#include <stdio.h>

typedef struct {
  GtkWidget *label;
  guint timer_id;
  gint64 start_time;
  gint64 elapsed_time;
  gboolean is_running;
} Base60;
