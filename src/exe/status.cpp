#include <iostream>

#include <X11/Xlib.h>
#include "barBase.hpp"
#include "barManager.hpp"
#include "barPerSeconds.hpp"
#include "volumeBar.hpp"
#include "network.hpp"
#include "cpu.hpp"
#include "memery.hpp"

using namespace std;

int main() {
    ::nextDwmStatus::barManager bm;
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::staticBar("")));
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::cpu()));
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::memery()));
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::netUpload()));
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::netDownload()));
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::staticBar("")));
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::volumeBar()));
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::timecaller()));
    bm.add_bar(std::unique_ptr<::nextDwmStatus::barBase>(new ::nextDwmStatus::staticBar("")));
    ::nextDwmStatus::barPerSeconds::main_thread();
    return 0;
}
