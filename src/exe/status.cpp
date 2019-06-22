#include <iostream>

#include <X11/Xlib.h>

using namespace std;

void
setstatus(Display *dpy, const string &str) {
    XStoreName(dpy, DefaultRootWindow(dpy), str.c_str());
    XSync(dpy, False);
}


int main() {
    if(auto dpy = XOpenDisplay(NULL); dpy != NULL) {
        std::string status = "test";
        setstatus(dpy,status);
    } else {
        fprintf(stderr, "dwmstatus: cannot open display.\n");
        return 1;
    }
    return 0;
}
