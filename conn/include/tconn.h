#pragma once
#include <glib-object.h>

#define T_TYPE_CONN (t_conn_get_type())
G_DECLARE_FINAL_TYPE(TConn, t_conn, T, CONN, GObject)

typedef struct _Conn {
  int eventId;
  const char *ctx;
} Conn;

/* getter and setter */
gboolean t_conn_get_value(TConn *self, Conn value);

void t_conn_set_value(TConn *self, Conn value);

/* create a new TConn instance */
TConn *t_conn_new(Conn value);
