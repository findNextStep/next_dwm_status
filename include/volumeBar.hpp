#pragma once
#include <barBase.hpp>
#include <barManager.hpp>

#include <volume.hpp>

#include <thread>
#include <memory>
#include <string>

namespace nextDwmStatus {
class volumeBar : public barBase {
private:
    int voice;
    mutable std::mutex voice_mutex;
    volume v;
    std::thread monit_thread;
    bool running;
public:
    volumeBar(): running(true), monit_thread([this] {
        voice = v.getVolume();
        while(running) {
            v.watch();
            voice_mutex.lock();
            voice = v.getVolume();
            voice_mutex.unlock();
        }
    }) {}
    ~volumeBar() {
        running = false;
        v.setVolume(v.getVolume());
    };
    virtual const std::string getStatus()const {
        voice_mutex.lock();
        auto result =  std::to_string(voice) + "%";
        voice_mutex.unlock();
        return result;
    }
};

}