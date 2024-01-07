#include "tconn.h"
#include <thread>

#include <glib-object.h>

int main(int argc, char **argv) {

  std::thread([] {
    TConn *d1, *d2;

    Conn c1{
        .eventId = 1,
        .ctx = "t1hello",
    };

    Conn c2{
        .eventId = 2,
        .ctx = "t1world",
    };

    d1 = t_conn_new(c1);
    d2 = t_conn_new(c2);

    t_conn_set_value(d2, c1);

    g_object_unref(d1);
    g_object_unref(d2);
  }).detach();

  std::thread([] {
    TConn *d1, *d2;

    Conn c1{
        .eventId = 3,
        .ctx = "t2hello",
    };

    Conn c2{
        .eventId = 4,
        .ctx = "t2world",
    };

    d1 = t_conn_new(c1);
    d2 = t_conn_new(c2);

    t_conn_set_value(d2, c1);

    g_object_unref(d1);
    g_object_unref(d2);
  }).detach();

  sleep(1);
  return 0;
}
