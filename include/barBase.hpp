#pragma once

#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>

namespace nextDwmStatus {

class barBase {
public:
    /**
     * @brief get bar element
     */
    virtual const std::string getStatus() const = 0;
};

class staticBar: public barBase {
    std::string status;
public:
    staticBar(const std::string &status_): status(status_) {}
    virtual const std::string getStatus()const {
        return status;
    }
};

}
