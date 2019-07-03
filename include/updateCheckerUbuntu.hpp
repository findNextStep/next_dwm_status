#pragma once

#include <fstream>
#include <thread>

// #include <fileWatcher/fileWatcher.h>

// get file motify time
#include <sys/stat.h>
#include <stdio.h>

namespace nextDwmStatus {
auto get_file_last_change_time(const std::string &file) {
    struct stat attrib;
    ::stat(file.c_str(), &attrib);
    return std::make_pair(attrib.st_mtim.tv_sec, attrib.st_mtim.tv_nsec);
}

class updateCheckerUbuntu : public barPerSeconds {
private:
    const std::string update_file;
    std::string status;
protected:
    static auto get_file_last_change_time_diff(const std::string &file) {
        return time(NULL) - get_file_last_change_time(file).first;
    }
    void check() {
        auto time = get_file_last_change_time_diff(update_file);
        int day = time/24/60/60;
        status = "";
        if (day > 0){
            status = "update in "+std::to_string(time) + "second ago";
        }
    }

    virtual void per_second_task() {
        check();
    }
public:
    virtual const std::string getStatus()const {
        return status;
    }

    updateCheckerUbuntu():update_file("/var/lib/apt/periodic/update-success-stamp"){
        check();
    }

};

}
