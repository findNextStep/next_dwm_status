#include <iostream>

#include <X11/Xlib.h>
#include "barBase.hpp"
#include "barManager.hpp"

using namespace std;

int main() {
    ::nextDwmStatus::barManager bm;
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::timecaller()));
    for(;;) {
        auto start = std::chrono::system_clock::now();
        for(auto task : ::nextDwmStatus::barPerSeconds::task_list) {
            task();
        }
        auto end = std::chrono::system_clock::now();
        auto diff = std::chrono::milliseconds(500) - (end - start);
        std::this_thread::sleep_for(diff);
        bm.requires_launch();
    }
    return 0;
}
