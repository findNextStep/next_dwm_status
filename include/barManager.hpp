#pragma once

#include "barBase.hpp"

#include <vector>
#include <mutex>
#include <memory>
#include <string>

#include <X11/Xlib.h>

namespace nextDwmStatus {

class barManager {
private:
    std::mutex lock;
    std::vector<std::unique_ptr<barBase> > bar_list;
    std::thread send_thread;
    bool running;
    Display *dpy;
protected:
    void setstatus(Display *dpy, const std::string &str) {
        XStoreName(dpy, DefaultRootWindow(dpy), str.c_str());
        XSync(dpy, true);
    }
public:
    barManager(): dpy(XOpenDisplay(NULL)), running(true), send_thread([this] {
        std::cout << "start send thread" << std::endl;
        lock.lock();
        while(running) {
            lock.lock();
            std::string output;
            for(const auto &bar : bar_list) {
                output += bar->getStatus();
            }
            setstatus(dpy, output);
        }
        std::cout << "send thread end" << std::endl;
    }) {
        dpy = XOpenDisplay(NULL);
    }

    ~barManager() {
        running = false;
        requires_launch();
    }

    void requires_launch() {
        lock.unlock();
    }

    void add_bar(std::unique_ptr<barBase> &&bar) {
        bar_list.push_back(std::move(bar));
    }

};
}
