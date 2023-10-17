#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <iostream>

void getWindowStatu(Display *display, Window win) {
  Atom atom = XInternAtom(win, "_NET_WM_STATE", true);
  Atom actualType;
  int format;
  unsigned long numItems, bytesAfter;
  unsigned char *data = 0;
  int status =
      XGetWindowProperty(display, win, atom, 0L, (~0L), false, AnyPropertyType,
                         &actualType, &format, &numItems, &bytesAfter, &data);

  if (status >= Success && numItems) {
    // success - we have data: Format should always be 32:
    std::cout << "format is " << format << std::endl;
    // cast to proper format, and iterate through values:
    Atom *array = (Atom *)data; // foramt is 20
    for (auto k = 0; k < numItems; k++) {
      // get window Id:
      Atom w = (Atom)array[k];
      char *name = XGetAtomName(dpy, w);
      std::cout << "name is " << name << std::endl;
      XFree(name);
    }
    XFree(data);
  }
}

void getWindowClassName(Display *display, Window win) {
  Atom atom = XInternAtom(win, "WM_CLASS", true);
  Atom actualType;
  int format;
  unsigned long numItems, bytesAfter;
  unsigned char *data = 0;
  int status =
      XGetWindowProperty(display, win, atom, 0L, (~0L), false, AnyPropertyType,
                         &actualType, &format, &numItems, &bytesAfter, &data);

  if (status >= Success && numItems) {
    // success - we have data: Format should always be 32:
    std::cout << "format is " << format << std::endl;
    // cast to proper format, and iterate through values:
    char *array = (char *)data; // foramt is 20
    for (auto k = 0; k < numItems / 2; k++) {
      // get window Id:
      char w = (char)array[k];
      printf("%c");
    }
    printf("\n");
    XFree(data);
  }
}

int main() {
  Display *dpy = XOpenDisplay(0);
  Wiwndow rootWin = XDefauleRootWindow(dpy);
  Atom atom = XInternAtom(rootWin, "_NET_CLIENT_LIST", true);
  Atom actualType;

  int format;
  unsigned long numItems, bytesAfter;
  unsigned char *data = 0;
  int status = XGetWindowProperty(dpy, rootWin, atom, 0L, (~0L), false,
                                  AnyPropertyType, &actualType, &format,
                                  &numItems, &bytesAfter, &data);

  if (status >= Success && numItems) {
    // success - we have data: Format should always be 32:
    std::cout << "format is " << format << std::endl;
    // cast to proper format, and iterate through values:
    long *array = (long *)data; // foramt is 32
    for (auto k = 0; k < numItems; k++) {
      // get window Id:
      Window w = (Window)array[k];
      getWindowStatu(dpy, w);
      getWindowStatu(dpy, w);
      std::cout << "Scanned client window:" << w << std::endl;
    }

    XFree(data);
  }
}
