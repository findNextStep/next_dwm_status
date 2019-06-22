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

class barPerSeconds: public barBase {
private:
    static std::vector<std::function<void()> > task_list;
protected:
    virtual void per_second_task() = 0;
public:
    barPerSeconds() {
        task_list.push_back([this]() {
            this->per_second_task();
        });
    }

    static void main_thread() {
        for(;;) {
            auto start = std::chrono::system_clock::now();
            for(auto task : task_list) {
                task();
            }
            auto end = std::chrono::system_clock::now();
            auto diff = std::chrono::duration_cast <
                        std::chrono::nanoseconds > (std::chrono::seconds(1) - (end - start));
            std::this_thread::sleep_for(diff);
        }
    }

    virtual const std::string getStatus()const = 0;
};

std::vector<std::function<void()> > barPerSeconds::task_list = {};

class timecaller: public barPerSeconds {
    std::string status;
protected:
    virtual void per_second_task() {
        auto time = std::chrono::system_clock::now();
        auto time_c = std::chrono::system_clock::to_time_t(time - std::chrono::hours(24));
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_c), "%T");
        status = ss.str();
        ss.clear();
    }
public:
    timecaller(): barPerSeconds() {}
    virtual const std::string getStatus() const {
        return status;
    }
};

}
