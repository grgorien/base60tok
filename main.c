#include <gtk/gtk.h>
#include <stdio.h>

typedef struct {
  GtkWidget *label;
  guint timer_id;
  gint64 start_time;
  gint64 elapsed_time;
  gboolean is_running;
} Base60;
int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  Base60 app_data = {.label = NULL,
                     .timer_id = 0,
                     .start_time = 0,
                     .elapsed_time = 0,
                     .is_running = FALSE};

  app = gtk_application_new("com.base60tok.stopwatch",
                            G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), &app_data);

  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
