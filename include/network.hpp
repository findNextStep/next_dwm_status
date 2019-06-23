#pragma once
#include <barManager.hpp>
#include <barPerSeconds.hpp>

#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>

namespace nextDwmStatus {

class netSpeed : public barPerSeconds {
private:
    double last_upload;
    std::chrono::system_clock::time_point last_time;
    std::string net_file;
protected:
    double speed;
    void per_second_task() {
        std::ifstream fs(net_file);
        double upload;
        fs >> upload;
        auto now = std::chrono::system_clock::now();
        speed = (upload - last_upload) / std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time).count() * 1000;
        last_time = now;
        last_upload = upload;
    }
    void init(const std::string &net, const std::string &file) {
        net_file = "/sys/class/net/" + net + "/statistics/" + file;
        per_second_task();
        per_second_task();
    }
public:
    netSpeed(const std::string &net, const std::string &file) {
        init(net, file);
    }
    netSpeed(const std::string &file) {
        init(run_command("nmcli d | grep wifi | awk \'{print $1}\'"), file);
    }
};

class netUpload : public netSpeed {
public:
    netUpload(): netSpeed("tx_bytes") {}
    netUpload(const std::string &net): netSpeed(net, "tx_bytes") {}
    virtual const std::string getStatus()const {
        std::stringstream ss;
        ss << "";
        ss << std::setprecision(3) << std::ios::fixed;
        if(speed > (1 << 30)) {
            ss << speed / (1 << 30) << "GB/s";
        } else if(speed > (1 << 20)) {
            ss << speed / (1 << 20) << "MB/s";
        } else if(speed > (1 << 10)) {
            ss << speed / (1 << 10) << "kB/s";
        } else {
            ss << speed << "bits";
        }
        return ss.str();
    }
};

class netDownload : public netSpeed {
public:
    netDownload(): netSpeed("rx_bytes") {}
    netDownload(const std::string &net): netSpeed(net, "rx_bytes") {}
    virtual const std::string getStatus()const {
        std::stringstream ss;
        ss << "";
        ss << std::setprecision(3) << std::ios::fixed;
        if(speed > (1 << 30)) {
            ss << speed / (1 << 30) << "GB/s";
        } else if(speed > (1 << 20)) {
            ss << speed / (1 << 20) << "MB/s";
        } else if(speed > (1 << 10)) {
            ss << speed / (1 << 10) << "kB/s";
        } else {
            ss << speed << "bits";
        }
        return ss.str();
    }
};
}
