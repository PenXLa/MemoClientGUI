#include "ClientNetUtils.h"
#include <winsock2.h>
#include "../Config/config.h"
#include <thread>
#include <queue>
#include "../Config/config.h"
SOCKET server;


bool initSocket() {
    WSADATA Wsd;
    return (WSAStartup(MAKEWORD(2, 2), &Wsd) == 0);
}


bool connect2server() {
    server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN addr;

    // 要连接的基础信息
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);

    int ret = connect(server, (struct sockaddr*)&addr, sizeof(addr));

    if (ret == SOCKET_ERROR) {
        WSACleanup();
        return false;
    }
    return true;
}



nlohmann::json receive() {
    std::queue<char> que;
    char buf[1024];
    int packSize = 0;
    while(true) {
        int size = recv(server,buf,sizeof buf,0);
        if(size > 0){
            for (int i=0; i<size; ++i) que.emplace(buf[i]);
        } else {
            //连接关闭
            throw lost_connection();
        }

        while(que.size()>=sizeof(int) && packSize==0 || packSize!=0 && que.size()>=packSize) {
            if (packSize == 0) {
                if (que.size() >= sizeof(int)) {
                    for (int i=0; i< sizeof(int); ++i) {
                        packSize <<= 8;
                        packSize |= (unsigned char)que.front();
                        que.pop();
                    }
                }
            }
            if (packSize != 0) {
                if (que.size()>=packSize) {
                    std::string str;
                    for (int i=0; i<packSize; ++i) {
                        str.push_back(que.front());
                        que.pop();
                    }
                    str.push_back(0);
                    auto json = nlohmann::json::parse(str);

                    return json;
                }
            }
        }

    }
}

void sendPack(const nlohmann::json &json) {
    std::string str = json.dump();
    int len = str.length(), sum;

    char *buf = new char[len+sizeof(int)];
    for (int i= sizeof(int)-1, size = len; i>=0; --i) {
        buf[i] = size & 0xFF;
        size >>= 8;
    }
    for (int i=sizeof(int); i<len+sizeof(int); ++i) {
        buf[i] = str[i-sizeof(int)];
    }

    len += sizeof(int);
    sum = len;

    while(sum > 0) {
        int res = send(server, buf+len-sum, sum, 0);
        if (res > 0) sum-=res;
        else {
            MessageBoxA(NULL, "发送错误", "", MB_OK);
        }
    }

    delete[] buf;
}


nlohmann::json request(const nlohmann::json& para) {
    if (!connect2server()) throw unable_to_connect();
    try {
        sendPack(para);
        nlohmann::json response = receive();
        closesocket(server);
        return response;
    } catch (lost_connection lc) {
        throw lc;
    }

}