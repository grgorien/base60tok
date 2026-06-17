#include <gtk/gtk.h>
#include <stdio.h>

typedef struct {
  GtkWidget *label;
  guint timer_id;
  gint64 start_time;
  gint64 elapsed_time;
  gboolean is_running;
} Base60;

static void update_label_text(Base60 *app) {
  gint64 total_us = app->elapsed_time;

  if (app->is_running) {
    total_us += g_get_monotonic_time() - app->start_time;
  }

  gint64 total_ms = total_us / 1000;

  int hours = (total_ms / 3600000);
  int minutes = (total_ms / 60000) % 60;
  int seconds = (total_ms / 1000) % 60;
  int centiseconds = (total_ms / 10) % 100;

  char time_string[32];
  char markup_buffer[128];

  snprintf(time_string, sizeof(time_string), "%02d:%02d:%02d.%02d", hours,
           minutes, seconds, centiseconds);
  snprintf(markup_buffer, sizeof(markup_buffer),
           "<span font='28' weight='bold'>%s</span>", time_string);
  gtk_label_set_markup(GTK_LABEL(app->label), markup_buffer);
}

static gboolean on_timeout_tick(gpointer user_data) {
  Base60 *app = (Base60 *)user_data;
  if (!app->is_running) {
    return G_SOURCE_REMOVE;
  }
  update_label_text(app);
  return G_SOURCE_CONTINUE;
}

static void stopwatch_toggle(Base60 *app) {
  if (app->is_running) {
    app->elapsed_time += g_get_monotonic_time() - app->start_time;
    app->is_running = FALSE;
    if (app->timer_id > 0) {
      g_source_remove(app->timer_id);
      app->timer_id = 0;
    }
  } else {
    app->start_time = g_get_monotonic_time();
    app->is_running = TRUE;
    app->timer_id = g_timeout_add(10, on_timeout_tick, app);
  }
  update_label_text(app);
}

static void stopwatch_reset(Base60 *app) {
  if (app->is_running) {
    app->is_running = FALSE;
    g_source_remove(app->timer_id);
    app->timer_id = 0;
  }
  app->elapsed_time = 0;
  update_label_text(app);
}

// G_GNUC_UNUSED clear compiler warning
static gboolean on_key_pressed(G_GNUC_UNUSED GtkEventControllerKey *controller,
                               guint keyval, G_GNUC_UNUSED guint keycode,
                               GdkModifierType state, gpointer user_data) {
  Base60 *app = (Base60 *)user_data;

  if (keyval == GDK_KEY_space) {
    stopwatch_toggle(app);
    return TRUE;
  }

  if ((keyval == GDK_KEY_C || keyval == GDK_KEY_c) &&
      (state & GDK_SHIFT_MASK)) {
    stopwatch_reset(app);
    return TRUE;
  }

  return FALSE;
}

static void on_activate(GtkApplication *app, gpointer user_data) {
  Base60 *app_data = (Base60 *)user_data;

  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Stopwatch");
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

  app =
      gtk_application_new("com.tiktok.stopwatch", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), &app_data);

  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
