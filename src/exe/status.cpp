#include <iostream>

#include <X11/Xlib.h>
#include "barBase.hpp"

using namespace std;

void
setstatus(Display *dpy, const string &str) {
    XStoreName(dpy, DefaultRootWindow(dpy), str.c_str());
    XSync(dpy, False);
}


int main() {
    ::nextDwmStatus::timecaller timer;
    std::thread thread([](){
        ::nextDwmStatus::barPerSeconds::main_thread();
    });
    if(auto dpy = XOpenDisplay(NULL); dpy != NULL) {
        while(true){
            setstatus(dpy,timer.getStatus());
        }
    } else {
        fprintf(stderr, "dwmstatus: cannot open display.\n");
        return 1;
    }
    return 0;
}