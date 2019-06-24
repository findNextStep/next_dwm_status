#pragma once

#include <barPerSeconds.hpp>
#include <barManager.hpp>

#include <array>
#include <fstream>

namespace nextDwmStatus {
class cpu : public barPerSeconds {
    std::array<long double, 7> last_num;
    int perc;
protected:
    virtual void per_second_task() {
        std::ifstream fs("/proc/stat");
        std::string skip;
        std::array<long double, 7> data;
        fs >> skip;
        for(auto &d : data) {
            fs >> d;
        }
        long double sum = 0;
        for(auto d : data) {
            sum -= d;
        }
        for(auto d : last_num) {
            sum += d;
        }
        perc = ((last_num[0] + last_num[1] + last_num[2] + last_num[5] + last_num[6]) -
                (data[0] + data[1] + data[2] + data[5] + data[6])) / sum * 100;
        last_num = data;
    }
public:
    cpu(): barPerSeconds() {}
    virtual const std::string getStatus()const {
        return "" + std::to_string(perc) + "%";
    }
};
}
