#ifndef MEMOCLIENT_CLIENTDATABASE_H
#define MEMOCLIENT_CLIENTDATABASE_H


#include "Schedule.h"
#include <list>
#include <functional>
#include <string>
extern int uid;
extern std::string passwd;
extern bool loggedin;



namespace DataBase {
    struct  schedule_not_found : public std::exception {
        const char * what () const throw () {
            return "file not found";
        }
    };


    void addSchedule(const Schedule &schedule);
    void removeSchedule(unsigned int sid);
    void editSchedule(unsigned int sid, const Schedule &schedule);
    Schedule findSchedule(unsigned int sid);
    void traverseSchedule(std::function<bool(const Schedule&)> func);
    void clearSchedule();

    void loadSchedules();
    void sync();
    void sync_add(const Schedule &schedule);
    void sync_remove(unsigned int sid);
    void sync_edit(unsigned int sid, const Schedule &schedule);
    void sync_clear();
}


#endif //MEMOCLIENT_CLIENTDATABASE_H
