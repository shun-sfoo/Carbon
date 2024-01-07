#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <iostream>
#include <string>

static const std::string REQ_ACTIVE_WINDOW = "_NET_ACTIVE_WINDOW";
static const std::string REQ_CLIENT_LIST = "_NET_CLIENT_LIST";
static const std::string REQ_WM_STATE = "_NET_WM_STATE";
static Display *dpy = XOpenDisplay(0);
static Window rootWin = XDefaultRootWindow(dpy);

void run() {
  Atom reqAtom = XInternAtom(dpy, REQ_ACTIVE_WINDOW.c_str(), true);
  Atom retAtom;
  int format;
  unsigned long nItems;
  unsigned long bytesAfter;
  unsigned char *prop = 0;
  int status = XGetWindowProperty(dpy, rootWin, reqAtom, 0L, (~0L), false,
                                  AnyPropertyType, &retAtom, &format, &nItems,
                                  &bytesAfter, &prop);

  Window activeWin;
  if (status == Success && nItems) {
    char *atomName = XGetAtomName(dpy, retAtom);
    std::cout << "format: " << format << " nitems: " << nItems
              << " atom: " << atomName << " byteAfter: " << bytesAfter
              << std::endl;
    XFree(atomName);
    Window *windows = (Window *)prop;
    activeWin = windows[0];
    std::cout << "0x" << std::hex << activeWin << std::endl;
  }
  XFree(prop);
  reqAtom = XInternAtom(dpy, REQ_CLIENT_LIST.c_str(), true);
  status = XGetWindowProperty(dpy, rootWin, reqAtom, 0L, (~0L), false,
                              AnyPropertyType, &retAtom, &format, &nItems,
                              &bytesAfter, &prop);
  if (status == Success && nItems) {
    char *atomName = XGetAtomName(dpy, retAtom);
    std::cout << "format: " << format << " nitems: " << nItems
              << " atom: " << atomName << " byteAfter: " << bytesAfter
              << std::endl;
    Window *windows = (Window *)prop;
    for (auto i = 0; i < nItems; i++) {
      if (activeWin != None && windows[i] == activeWin) {
        std::cout << "active windows index is: " << i
                  << " window id: " << std::hex << windows[i] << std::endl;
      }
      // unsigned char *innerProp = 0;
      // reqAtom = XInternAtom(dpy, REQ_WM_STATE.c_str(), true);
      // status = XGetWindowProperty(dpy, windows[i], reqAtom, 0L, (~0L), false,
      // AnyPropertyType, &retAtom, &format, &nItems, &bytesAfter, &innerProp);
    }
    XFree(atomName);
  }
  XFree(prop);
}

// void getWindowStatu(Display *display, Window win)
// {
//   Atom atom = XInternAtom(win, "_NET_WM_STATE", true);
//   Atom actualType;
//   int format;
//   unsigned long numItems, bytesAfter;
//   unsigned char *data = 0;
//   int status =
//       XGetWindowProperty(display, win, atom, 0L, (~0L), false,
//       AnyPropertyType,
//                          &actualType, &format, &numItems, &bytesAfter,
//                          &data);

//   if (status >= Success && numItems)
//   {
//     // success - we have data: Format should always be 32:
//     std::cout << "format is " << format << std::endl;
//     // cast to proper format, and iterate through values:
//     Atom *array = (Atom *)data; // foramt is 20
//     for (auto k = 0; k < numItems; k++)
//     {
//       // get window Id:
//       Atom w = (Atom)array[k];
//       char *name = XGetAtomName(dpy, w);
//       std::cout << "name is " << name << std::endl;
//       XFree(name);
//     }
//     XFree(data);
//   }
// }

// void getWindowClassName(Display *display, Window win)
// {
//   Atom atom = XInternAtom(win, "WM_CLASS", true);
//   Atom actualType;
//   int format;
//   unsigned long numItems, bytesAfter;
//   unsigned char *data = 0;
//   int status =
//       XGetWindowProperty(display, win, atom, 0L, (~0L), false,
//       AnyPropertyType,
//                          &actualType, &format, &numItems, &bytesAfter,
//                          &data);

//   if (status >= Success && numItems)
//   {
//     // success - we have data: Format should always be 32:
//     std::cout << "format is " << format << std::endl;
//     // cast to proper format, and iterate through values:
//     char *array = (char *)data; // foramt is 20
//     for (auto k = 0; k < numItems / 2; k++)
//     {
//       // get window Id:
//       char w = (char)array[k];
//       printf("%c");
//     }
//     printf("\n");
//     XFree(data);
//   }
// }

int main() {
  // Display *dpy = XOpenDisplay(0);
  // Wiwndow rootWin = XDefauleRootWindow(dpy);
  // Atom atom = XInternAtom(rootWin, "_NET_CLIENT_LIST", true);
  // Atom actualType;

  // int format;
  // unsigned long numItems, bytesAfter;
  // unsigned char *data = 0;
  // int status = XGetWindowProperty(dpy, rootWin, atom, 0L, (~0L), false,
  //                                 AnyPropertyType, &actualType, &format,
  //                                 &numItems, &bytesAfter, &data);

  // if (status >= Success && numItems)
  // {
  //   // success - we have data: Format should always be 32:
  //   std::cout << "format is " << format << std::endl;
  //   // cast to proper format, and iterate through values:
  //   long *array = (long *)data; // foramt is 32
  //   for (auto k = 0; k < numItems; k++)
  //   {
  //     // get window Id:
  //     Window w = (Window)array[k];
  //     getWindowStatus(dpy, w);
  //     getWindowStatus(dpy, w);
  //     std::cout << "Scanned client window:" << w << std::endl;
  //   }

  //   XFree(data);
  // }
  run();
}
