#ifndef MEMOCLIENT_CLIENTNETUTILS_H
#define MEMOCLIENT_CLIENTNETUTILS_H

#include "../JSONLib/json.hpp"
#include "exceptions.h"

nlohmann::json request(const nlohmann::json& para);
bool initSocket();

#endif //MEMOCLIENT_CLIENTNETUTILS_H
