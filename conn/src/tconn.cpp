#include "tconn.h"
#include <cstdio>

static guint t_conn_signal;

struct _TConn {
  GObject parent;
  Conn value;
};

G_DEFINE_TYPE(TConn, t_conn, G_TYPE_OBJECT)

static void print_value(TConn *self) {
  printf("eventId: %d, ctx: %s\n", self->value.eventId, self->value.ctx);
}

static void t_conn_class_init(TConnClass *kclass) {
  t_conn_signal = g_signal_new_class_handler(
      "conn", G_TYPE_FROM_CLASS(kclass),
      static_cast<GSignalFlags>(G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE |
                                G_SIGNAL_NO_HOOKS),
      G_CALLBACK(print_value), nullptr /* accumulator */,
      nullptr /* accumulator data */, nullptr /* C marshaller */,
      G_TYPE_NONE /* return_type */, 0 /* n_params */
  );
}

static void t_conn_init(TConn *self) {}

/* getter and setter */
gboolean t_double_get_value(TConn *self, Conn *value) {
  g_return_val_if_fail(T_IS_CONN(self), FALSE);
  *value = self->value;
  return TRUE;
}

void t_conn_set_value(TConn *self, Conn value) {
  g_return_if_fail(T_IS_CONN(self));
  self->value = value;
  g_signal_emit(self, t_conn_signal, 0);
}

TConn *t_conn_new(Conn value) {
  TConn *d;
  d = static_cast<TConn *>(g_object_new(T_TYPE_CONN, nullptr));
  d->value = value;
  return d;
}
