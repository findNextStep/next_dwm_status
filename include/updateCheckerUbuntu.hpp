#pragma once

#include <fstream>
#include <thread>

#include <fileWatcher.hpp>
#include <barManager.hpp>

// get file motify time
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>

namespace nextDwmStatus {
auto get_file_last_change_time(const std::string &file) {
    struct stat attrib;
    ::stat(file.c_str(), &attrib);
    return std::make_pair(attrib.st_mtim.tv_sec, attrib.st_mtim.tv_nsec);
}

class updateCheckerUbuntu : public barBase {
private:
    const std::string update_file;
    std::string status;
    wmj::fileWatcher::fileWatcher watcher;
    std::thread watch_thread;
protected:
    static auto get_file_last_change_time_diff(const std::string &file) {
        return time(NULL) - get_file_last_change_time(file).first;
    }
    void check() {
        auto time = get_file_last_change_time_diff(update_file);
        int day = time / 24 / 60 / 60;
        status = "";
        if(day > 0) {
            status = "update in " + std::to_string(day) + "day ago";
        }
    }
public:
    virtual const std::string getStatus()const {
        return status;
    }

    updateCheckerUbuntu():
        update_file("/var/lib/apt/periodic/update-success-stamp"),
        watcher(update_file, [this]() {
        this->check();
        barManager::requires_launch();
    }), watch_thread([this]() {
        this->watcher.run();
    }) {
        this->watcher.startWatch();
        check();
    }

};

}
