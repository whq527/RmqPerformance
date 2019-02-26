// RmqPerformanceClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <mutex>
#include "cpack.h"
#include "ClibRmq.h"
#include "zlib.h"

#pragma comment(lib, "psapi.lib") // Added to support GetProcessMemoryInfo()
using namespace std;

void test_by_librmq(const char* _key, long _count, long _rate, int _delay, const char* _user, const char* _psw, const char* _ip, const char* _exchange, const char* _type, int _compress)
{
	ClibRmq rmq(10);
	if (rmq.Init(_user, _psw, _ip, 5672, _exchange, _type, 1, 2, _compress))
	{
		rmq.SetRate(_rate);
		rmq.Start();
		printf("librmq product ok send to %s : %s %ld/%ld\n", _exchange, _key, _rate, _count);
	}

	Sleep(10);
	ULONGLONG tk = GetTickCount64();
	bool first = true;
	while (true)
	{
		if (first || _delay > 0)
		{
			auto tp = std::chrono::system_clock::now();
			time_t t = std::chrono::system_clock::to_time_t(tp);
			struct tm now_time;
			localtime_s(&now_time, &t);
			char time[256] = { 0 };
			strftime(time, 256, "%Y-%m-%d %H:%M:%S", &now_time);
			printf("%s %s 发 %ld 条 循环延迟%d秒 start\n", time, _key, _count, _delay / 1000);
			for (size_t i = 1; i <= _count; i++)
			{
				//int sizea = sizeof(ST_CPACK);
				//ST_CPACK one = { 0 };
				//one.head.retCode = _count;
				//one.head.recCount = i;
				//one.head.RequestType = 1123;
				//auto tp2 = std::chrono::system_clock::now();
				//one.head.userdata = std::chrono::system_clock::to_time_t(tp2);//包放入队列的时间
				//strcpy_s(one.pack.vsvarstr0, "my msg");
				string str = "哈哈哈";
				st_rmq_msg_header header;
				header.type = "msg";
				header.id = "123";
				header.struct_name = "string";
				header.routekey = _key;
				rmq.Push_Msg((void*)str.data(), str.length(), &header);
			}
			first = false;
			tp = std::chrono::system_clock::now();
			t = std::chrono::system_clock::to_time_t(tp);
			localtime_s(&now_time, &t);
			strftime(time, 256, "%Y-%m-%d %H:%M:%S", &now_time);
			printf("%s %s 发 %ld 条 循环延迟%d秒 fin\n", time, _key, _count, _delay / 1000);
		}

		if (_delay == 0)
		{
			Sleep(10000);
		}
		else
		{
			Sleep(_delay);
		}
		//ST_CPACK one = { 0 };
		//one.head.RequestType = 222;
		//strcpy_s(one.pack.vsvarstr0, "my heart /10s");
		//rmq.Push_Msg((void*)&one, sizeof(ST_CPACK), "cpack", "123", _key);
	}
}

int main(int argc, char*argv[])
{
	//test_by_SocketThread();

	if (argc == 11)
	{
		test_by_librmq(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5], argv[6], argv[7], argv[8], argv[9], atoi(argv[10]));
	}
	else
	{
		test_by_librmq("a.b.c", 10, 5000, 0, "client", "client", "192.168.3.200", "test", "topic", 1);
	}
}