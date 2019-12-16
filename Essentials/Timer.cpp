//
// Created by a9214 on 2019/11/3.
//

#include "Timer.h"
#include <thread>
#include <mutex>
#include "Schedule.h"
#include <windows.h>
#include <iostream>
#include "ClientDataBase.h"
void onTimer() {
    while(1) {
        time_t now = time(0);
        for (auto p : schedules) {
            if (!p->alerted && p->endTime <= now) {
				MessageBox(0, p->name, "Ê±¼äµ½£¡", 0);
                p->alerted = true;
                DataBase::editSchedule(p->sid, *p);
                if (loggedin) DataBase::sync_edit(p->sid, *p);
            }
        }
        Sleep(1000);
    }


}

void startTimer() {
    std::thread timer(onTimer);
    timer.detach();
}