extern "C" {
#include <volume.h>
}

#include <vector>
#include <string>
#include <iostream>
int main(const int argc, const char **argv) {
    const std::vector<std::string> args(argv, argv + argc);
    if(args.size() > 1) {
        std::string command = args.at(1);
        if(command == "get") {
            long volume = 0;
            audio_volume(AUDIO_VOLUME_GET, &volume);
            std::cout << "volume == " << volume << std::endl;
        } else if(command == "set") {
            long volume = 0;
            if(args.size() == 3) {
                volume = std::stoi(args.at(2));
            } else {
                std::cin >> volume;
            }
            audio_volume(AUDIO_VOLUME_SET, &volume);
        } else if(command == "plus") {
            long volume = 0;
            if(args.size() == 3) {
                volume = std::stoi(args.at(2));
            } else {
                std::cin >> volume;
            }
            audio_volume(AUDIO_VOLUME_PLUS, &volume);
        }
    } else {
        std::string command = "";
        while(std::cin >> command) {
            if(command == "get") {
                long volume = 0;
                audio_volume(AUDIO_VOLUME_GET, &volume);
                std::cout << "volume == " << volume << std::endl;
            } else if(command == "set") {
                long volume = 0;
                std::cin >> volume;
                audio_volume(AUDIO_VOLUME_SET, &volume);
            } else if(command == "plus") {
                long volume = 0;
                std::cin >> volume;
                audio_volume(AUDIO_VOLUME_PLUS, &volume);
            }
        }
    }
}