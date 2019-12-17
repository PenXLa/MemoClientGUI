//
// Created by a9214 on 2019/10/31.
//

#include "ClientDataBase.h"
#include "../JSONLib/json.hpp"
#include "../Net/ClientNetUtils.h"
#include "../Utils/Utils.h"
#include "../stdafx.h"
#include "../Mem.h"

int uid = 0;
std::string passwd;
bool loggedin = false;

void DataBase::addSchedule(const Schedule &schedule)
{
    FILE *fp;
    if( (fp = fopen ("Schedules.txt","ab+") ) == NULL ){
        printf("CANNOT OPEN");
        exit(0);
    }
    fwrite(&schedule, sizeof(Schedule), 1, fp);
    fclose(fp);
}
void DataBase::removeSchedule(unsigned sid)
{   Schedule *schedule;
    schedule=(Schedule*)malloc(sizeof(Schedule));
    FILE *fp1,*fp2;
    if( (fp1 = fopen ("Schedules.txt","rb+") ) == NULL ){
        printf("CANNOT OPEN");
        exit(0);
    }
    if( (fp2 = fopen ("data1.txt","wb") ) == NULL ){
        printf("CANNOT OPEN");
        exit(0);
    }

    while(fread(schedule, sizeof(Schedule), 1, fp1)){
        if(schedule->sid!=sid)
            fwrite(schedule, sizeof(Schedule), 1, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    remove("Schedules.txt");
    rename("data1.txt","Schedules.txt");
}

void DataBase::editSchedule(unsigned sid, const Schedule &schedule) {
    Schedule *p;
    p=(Schedule *)malloc(sizeof(Schedule));
    FILE *fp1,*fp2;
    if( (fp1 = fopen ("Schedules.txt","rb+") ) == NULL ) {
        printf("CANNOT OPEN");
        exit(0);
    }
    fp2 = fopen("data1.txt", "wb");
    while(fread(p, sizeof(Schedule), 1, fp1)) {
        if(p->sid!=sid)
            fwrite(p, sizeof(Schedule), 1, fp2);
        else
            fwrite(&schedule, sizeof(Schedule), 1, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    remove("Schedules.txt");
    rename("data1.txt","Schedules.txt");
    free(p);
}


Schedule DataBase::findSchedule(unsigned int sid)
{   Schedule *p;
    p=(Schedule *)malloc(sizeof(Schedule));
    FILE *fp;
    if( (fp = fopen ("Schedules.txt","rb") ) == NULL ) {
        printf("CANNOT OPEN");
        exit(0);
    }
    do {
        fread(p, sizeof(class Schedule), 1, fp);
        if(p->sid==sid) {
            fclose(fp);
            free(p);
            return *p;
        }
    }while(!feof(fp));
    fclose(fp);
    free(p);
    throw schedule_not_found();
}

void DataBase::traverseSchedule(std::function<bool(const Schedule&)> func) {
    Schedule *p;
    p=(Schedule *)malloc(sizeof(Schedule));
    FILE *fp;
    if( (fp = fopen ("Schedules.txt","rb") ) ) {
        while(fread(p, sizeof(class Schedule), 1, fp)) func(*p);
        free(p);
        fclose(fp);
    }
}

void DataBase::clearSchedule() {
    FILE *fp;
    fp = fopen ("Schedules.txt","wb+");
    fclose(fp);
}

void DataBase::loadSchedules() {
	schedules.clear();
    DataBase::traverseSchedule([](const Schedule& sch){
        Schedule *sch2 = new Schedule(sch);
        schedules.push_back(sch2);
        return true;
    });
}

//双向同步，互相填补对方没有的schedule
void DataBase::sync() {
    nlohmann::json req;
    req["name"] = "sync";
    req["uid"] = uid;
    req["pwd"] = passwd;
    req["schs"] = {};
    for (Schedule *sch:schedules)
        req["schs"].push_back(schedule2json(*sch));//发送本地的事项

    auto res = request(req);
    for (nlohmann::json &jsch:res["schs"]) {
        Schedule *local_sch= nullptr;
        unsigned int sid = jsch["sid"].get<unsigned int>();

        //在本地寻找相同sid的事项
        for (Schedule* sch : schedules)
            if (sch->sid == sid){
                local_sch = sch;
                break;
            }

        Schedule *remote_sch = json2Schedule(jsch);
        //找到了
        if (local_sch) {
            if (local_sch->lastEdit < remote_sch->lastEdit || (local_sch->lastEdit == remote_sch->lastEdit && remote_sch->alerted && !local_sch->alerted)) {
                *local_sch = *remote_sch;
                DataBase::editSchedule(sid, *remote_sch);
            }
            delete remote_sch;//直接在原来的基础上赋值，这个就可以释放了。如果没找到相同sid的，就不能释放，因为要用
        } else {//没找到
            schedules.push_back(remote_sch);
			DataBase::addSchedule(*remote_sch);
        }
    }
}

void DataBase::sync_add(const Schedule &schedule) {
    nlohmann::json req;
    req["name"] = "addSchedule";
    req["uid"] = uid;
    req["pwd"] = passwd;
    req["schedule"] = schedule2json(schedule);
    request(req);
}

void DataBase::sync_remove(unsigned int sid) {
    nlohmann::json req;
    req["name"] = "removeSchedule";
    req["uid"] = uid;
    req["pwd"] = passwd;
    req["sid"] = sid;
    request(req);
}
void DataBase::sync_edit(unsigned sid, const Schedule &schedule) {
    nlohmann::json req;
    req["name"] = "editSchedule";
    req["uid"] = uid;
    req["pwd"] = passwd;
    req["schedule"] = schedule2json(schedule);
    req["sid"] = sid;
    request(req);
}

void DataBase::sync_clear() {
    nlohmann::json req;
    req["name"] = "clearSchedule";
    req["uid"] = uid;
    req["pwd"] = passwd;
    request(req);
}