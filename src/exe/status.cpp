#include <iostream>

#include <X11/Xlib.h>
#include "barBase.hpp"
#include "barManager.hpp"
#include "barPerSeconds.hpp"

using namespace std;

int main() {
    ::nextDwmStatus::barManager bm;
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::timecaller()));
    ::nextDwmStatus::barPerSeconds::main_thread();
    return 0;
}
