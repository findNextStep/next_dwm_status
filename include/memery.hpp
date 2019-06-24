#pragma  once

#include <barPerSeconds.hpp>

#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>


namespace nextDwmStatus {

std::map<std::string, long long> getMemMsg(std::set<std::string>require) {
    std::map<std::string, long long> result;
    std::ifstream fs("/proc/meminfo");
    while(require.size() && fs) {
        std::string line ;
        std::getline(fs, line);
        auto it = line.find(":");
        if(it != line.npos) {
            std::string name = line.substr(0, it);
            if(require.find(name) != require.end()) {
                long long number;
                std::stringstream ss(line);
                ss >> line >> number;
                result[name] = number;
                require.erase(name);
            }
        }
    }
    return result;

}

class memery : public barPerSeconds {
    int usage;
protected:
    virtual void per_second_task() {
        auto mem_info = getMemMsg({"MemTotal", "MemFree", "MemAvailable", "Buffers", "Cached"});
        long long MemTotal = mem_info["MemTotal"],
                  MemFree = mem_info["MemFree"],
                  MemAvailable = mem_info["MemAvailable"],
                  Buffers = mem_info["Buffers"],
                  Cached = mem_info["Cached"];
        usage = (int)((MemTotal - MemFree - Buffers - Cached) / (double)MemTotal * 100);
    }
public:
    virtual const std::string getStatus()const {
        return "" + std::to_string(usage) + "%";
    }
};
}
