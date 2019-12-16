#include "Terminal.h"
#include <iostream>
#include <string>
#include "../Essentials/Schedule.h"
#include "../Essentials/ClientDataBase.h"
#include "../Net/ClientNetUtils.h"
#include "../JSONLib/json.hpp"
using namespace std;
void tui() {
    puts("1.Add schedule\n2.Remove schedule\n3.Schedule list\n4.Reset schedule time\n5.Register\n6.Login");
    int cmd;
    while(1) {
        cin >> cmd;
        if (cmd == 1) {
            tm ddl; Schedule *sch = new Schedule;//内存中创建新事项
            printf("Input schedule name:"); cin >> sch->name;
            printf("Input deadline(year month day hour minute separated by whitespace):");
            cin >> ddl.tm_year >> ddl.tm_mon >> ddl.tm_mday >> ddl.tm_hour >> ddl.tm_min;
            ddl.tm_year -= 1900; --ddl.tm_mon; ddl.tm_sec = 0;
            sch->endTime = mktime(&ddl);
            sch->lastEdit = time(0);
            schedules.push_back(sch);
            DataBase::addSchedule(*sch);//放到文件中
            if (loggedin) DataBase::sync_add(*sch);//服务器同步
        } else if (cmd == 2) {
            unsigned int sid; cin >> sid;
            for (auto p=schedules.begin(); p!=schedules.end(); ++p) {
                if ((*p)->sid == sid) {
                    DataBase::removeSchedule(sid);//数据库清除
                    delete(*p);//内存释放
                    schedules.erase(p);//链表中除名
                    break;
                }
            }
            if(loggedin)DataBase::sync_remove(sid);//同步
        } else if (cmd == 3) {
            for (auto p : schedules) {
                cout << p->name <<
                    "\nEndTime:" << p->endTime <<
                    "\nLastEditTime:" << p->lastEdit <<
                    "\nsid:" << p->sid <<
                    "\nAlerted:" << p->alerted <<"\n\n";
            }
        } else if (cmd == 4) {
            tm t; unsigned int sid;
            cin >> sid >> t.tm_year >> t.tm_mon >> t.tm_mday >> t.tm_hour >> t.tm_min;
            t.tm_year -= 1900; --t.tm_mon; t.tm_sec = 0;
            time_t tt = mktime(&t);
            for (auto p:schedules) {
                if(p->sid == sid){
                    p->endTime = tt;
                    p->alerted = false;
                    p->lastEdit = time(0);
                    DataBase::editSchedule(sid, *p);//更新数据库
                    if (loggedin) DataBase::sync_edit(sid, *p);//同步
                    break;
                }
            }

        } else if (cmd == 5) {
            string pwd;
            cout << "Input password:"; cin >> pwd;
            nlohmann::json req;
            req["name"] = "register";
            req["pwd"] = pwd;
            auto res = request(req);
            auto a=res.dump();
            cout << "Register completed. Your id is " << res["uid"].get<int>() << '\n';
        } else if (cmd == 6) {
            cout << "Input your id:"; cin >> uid;
            cout << "Input password:"; cin >> passwd;
            nlohmann::json req;
            req["name"] = "login";
            req["pwd"] = passwd;
            req["uid"] = uid;
            auto res = request(req);
            if (res["success"].get<bool>()) {
                cout << "login successfully\n";
                loggedin = true;
            } else {
                cout << res["reason"].get<std::string>() << '\n';
            }
        }

        else if (cmd == -1) {
            cout << time(0) << '\n';
        } else if (cmd == -2) {
            DataBase::clearSchedule();
            schedules.clear();
            if (loggedin) DataBase::sync_clear();
        } else if (cmd == -3) {
            DataBase::sync();
        }
    }
}