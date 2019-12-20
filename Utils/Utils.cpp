#include "Utils.h"
#include <Windows.h>

nlohmann::json schedule2json(const Schedule &sch) {
    nlohmann::json json;
    json["endtime"] = sch.endTime;
    json["alerted"] = sch.alerted;
    json["sid"] = sch.sid;
    json["name"] = GbkToUtf8(sch.name);
    json["lastedit"] = sch.lastEdit;
    return json;
}

Schedule* json2Schedule(const nlohmann::json json) {
    Schedule* sch = new Schedule(Utf8ToGbk(json["name"].get<std::string>()));
    sch->sid = json["sid"].get<int>();
    sch->alerted = json["alerted"].get<bool>();
    sch->endTime = json["endtime"].get<time_t>();
    sch->lastEdit = json["lastedit"].get<time_t>();
    return sch;
}


std::string GbkToUtf8(std::string src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str.c_str(), -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	std::string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

std::string Utf8ToGbk(std::string src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}