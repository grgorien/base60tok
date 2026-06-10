#include <gtk/gtk.h>
#include <stdio.h>

typedef struct {
  GtkWidget *label;
  guint timer_id;
  gint64 start_time;
  gint64 elapsed_time;
  gboolean is_running;
} Base60;
static void on_activate(GtkApplication *app, gpointer user_data) {
  Base60 *app_data = (Base60 *)user_data;

  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Base60 Stopwatch");
  gtk_window_set_default_size(GTK_WINDOW(window), 250, 120);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  gtk_window_set_child(GTK_WINDOW(window), box);

  app_data->label = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(app_data->label),
                       "<span font='28' weight='bold'>00:00:00.00</span>");
  gtk_box_append(GTK_BOX(box), app_data->label);

  GtkWidget *help_label = gtk_label_new("[Space] Start/Stop • [Shift+C] Reset");
  gtk_box_append(GTK_BOX(box), help_label);

  GtkEventController *controller = gtk_event_controller_key_new();
  g_signal_connect(controller, "key-pressed", G_CALLBACK(on_key_pressed),
                   app_data);
  gtk_widget_add_controller(window, controller);

  gtk_window_present(GTK_WINDOW(window));
}

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
