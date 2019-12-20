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
	while (1) {
		time_t now = time(0);
		while (earliestSch && earliestSch->endTime <= now) {
			if (!earliestSch->alerted) {
				MessageBox(0, earliestSch->name, "时间到！", 0);
				earliestSch->alerted = true;//闹钟提醒
				DataBase::editSchedule(earliestSch->sid, *earliestSch);
				if (loggedin) DataBase::sync_edit(earliestSch->sid, *earliestSch);
			}
			selectEarliest();
		}
		Sleep(1000);
	}
}

void startTimer() {
    std::thread timer(onTimer);
    timer.detach();
}

void selectEarliest() {
	earliestSch = NULL;
	for (auto p : schedules)
		if (!p->alerted && (!earliestSch || p->endTime < earliestSch->endTime))
			earliestSch = p;
}