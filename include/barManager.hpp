#pragma once

#include "barBase.hpp"

#include <vector>
#include <mutex>
#include <memory>
#include <string>
#include <thread>

#include <X11/Xlib.h>

namespace nextDwmStatus {

class _barManager {
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
    _barManager(): dpy(XOpenDisplay(NULL)), running(true), send_thread([this] {
        lock.lock();
        while(running) {
            lock.lock();
            std::string output;
            bool first = true;
            for(const auto &bar : bar_list) {
                if(first) {
                    first = false;
                } else {
                    output += " ";
                }
                output += bar->getStatus();
            }
            setstatus(dpy, output);
        }
    }) {
        dpy = XOpenDisplay(NULL);
    }
    ~_barManager() {
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

class barManager {
private:
    static _barManager bm;
public:
    static void requires_launch() {
        bm.requires_launch();
    }

    static void add_bar(std::unique_ptr<barBase> &&bar) {
        bm.add_bar(std::move(bar));
    }
};
_barManager barManager::bm;
}
