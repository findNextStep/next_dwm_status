#pragma once

#include <barPerSeconds.hpp>

#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

namespace nextDwmStatus {
const std::string battery_file = "/sys/class/power_supply/";
const std::string power = "/current_now";
const std::string stat = "/status";
const std::string full = "/charge_full";
const std::string now = "/charge_now";
class battery : public barPerSeconds {
private:
    const std::string battery_name;
    const std::string power_file;
    const std::string stat_file;
    const std::string full_file;
    const std::string now_file;
    std::string message;
protected:
    static long long read_data(const std::string &file) {
        long long result;
        std::ifstream fs(file);
        fs >> result;
        return result;
    }
    virtual void per_second_task() {
        // copy from libqtile widget battery.py
        // see https://github.com/qtile/qtile/blob/v0.13.0/libqtile/widget/battery.py
        std::string stat;
        std::ifstream fs(stat_file);
        const std::vector<std::string> charging_icon = {
            "", "", "", "", "", "", "",
        };
        const std::vector<std::string> discharging_icon = {
            "", "", "", "", "", "", "", "", "", ""
        };
        const std::string unknow = "";
        fs >> stat;
        using namespace std;
        cout << stat << endl;
        if(stat == "Full") {
            // full charge
            message = "100%";
        } else if(stat == "Discharging" || stat == "Not charging") {
            // work in battery
            const double full = read_data(full_file),
                         now = read_data(now_file),
                         power = read_data(power_file);
            const double time = now / power,
                         perc = now / full;
            std::stringstream ss;
            ss << discharging_icon.at((perc - 0.001) * (discharging_icon.size() - 1)) << (int)(perc * 100) << "%" << (int)time << ":" << ((int)(time * 60) % 60);
            message = ss.str();
        } else if(stat == "Charging") {
            // charging
            const double full = read_data(full_file),
                         now = read_data(now_file),
                         power = read_data(power_file);
            const double time = (full - now) / power,
                         perc = now / full;
            std::stringstream ss;
            ss << charging_icon.at((perc - 0.001) * (charging_icon.size() - 1)) << (int)(perc * 100) << "%" << (int)time << ":" << ((int)(time * 60) % 60);
            message = ss.str();
        } else if(stat == "Unknown") {
            const double full = read_data(full_file),
                         now = read_data(now_file);
            const double perc = now / full;
            std::stringstream ss;
            ss << unknow << (int)(perc * 100) << "%";
            message = ss.str();
        } else {
            message = "unknow status "  + stat;
        }
    }
public:
    battery(): battery("BAT0") {}
    battery(const std::string &batteryName):
        battery_name(batteryName),
        power_file(battery_file + battery_name + power),
        stat_file(battery_file + battery_name + stat),
        full_file(battery_file + battery_name + full),
        now_file(battery_file + battery_name + now) {}
    virtual const std::string getStatus()const {
        return message;
    }
};
}
