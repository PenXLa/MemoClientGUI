//
// Created by a9214 on 2019/11/3.
//

#ifndef MEMOCLIENT_UTILS_H
#define MEMOCLIENT_UTILS_H



#include "../JSONLib/json.hpp"
#include "../Essentials/Schedule.h"
std::time_t getTimeStamp();
nlohmann::json schedule2json(const Schedule &sch);
Schedule* json2Schedule(const nlohmann::json json);

#endif //MEMOCLIENT_UTILS_H
