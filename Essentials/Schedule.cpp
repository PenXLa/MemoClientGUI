//
// Created by a9214 on 2019/10/31.
//

#include "Schedule.h"
#include <cstring>
#include <ctime>
#include <chrono>
std::list<Schedule*> schedules;

std::time_t getTimeStamp() {
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    std::time_t timestamp = tmp.count();
    return timestamp;
}

Schedule::Schedule() {
    sid = getTimeStamp();
}
Schedule::Schedule(std::string name):Schedule() {
    strcpy(this->name, name.c_str());
};
