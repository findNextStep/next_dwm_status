#pragma once

#include <string>
#include <functional>
#include <string.h>
#include <stdio.h>

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


const std::string
run_command(const std::string command) {
    char *p;
    FILE *fp;
    char buf[1024];
    if(!(fp = popen(command.c_str(), "r"))) {
        return "";
    }
    p = fgets(buf, sizeof(buf) - 1, fp);
    if(pclose(fp) < 0) {
        return "";
    }
    if(!p) {
        return "";
    }
    if((p = strrchr(buf, '\n'))) {
        p[0] = '\0';
    }
    return buf[0] ? buf : NULL;
}

}
